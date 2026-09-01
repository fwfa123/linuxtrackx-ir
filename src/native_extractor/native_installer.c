#define _FILE_OFFSET_BITS 64
#define _POSIX_C_SOURCE 200809L

#include "native_installer.h"

#include "cfb.h"
#include "sha256.h"

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <mspack.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <zlib.h>

struct carved_output {
  const char *output_name;
  uint64_t offset;
  uint64_t length;
  const char *sha256;
  bool gzip;
};

enum member_action {
  MEMBER_CARVE,
  MEMBER_COPY,
  MEMBER_GAME_DATA
};

struct member_output {
  const char *output_name;
  uint64_t member_size;
  const char *member_sha256;
  enum member_action action;
  uint64_t offset;
  uint64_t length;
  const char *output_sha256;
};

struct installer_version {
  const char *description;
  const char *installer_sha256;
  uint64_t attached_cab_offset;
  uint64_t attached_cab_size;
  const char *msi_member;
  uint64_t msi_size;
  const char *msi_sha256;
  uint64_t inner_cab_size;
  const char *inner_cab_sha256;
  const char *program_member;
  uint64_t program_size;
  const char *program_sha256;
  const struct carved_output *carved;
  size_t carved_count;
  const struct member_output *members;
  size_t member_count;
};

static const struct carved_output trackir_553_carved[] = {
  {"poem2.txt",    UINT64_C(4699280), UINT64_C(98),
   "b2408dd94dd0552eb83b1f594b2f2c20a409f566cfd801accc73947f7e6f3c5f", false},
  {"tir4.fw.gz",   UINT64_C(5154208), UINT64_C(72743),
   "b9a86beac9d93dcbd5a09cf9b5be30e7afc3e4418b4a98e0842de2ec83d2a490", true},
  {"sn4.fw.gz",    UINT64_C(5299712), UINT64_C(169289),
   "8add157e4080541586ed7099da23cbcf53ab0946ed323693514bf5a9ef750439", true},
  {"tir5.fw.gz",   UINT64_C(5469008), UINT64_C(36076),
   "5fd3028f8556edd6a5309acc95220e3375c6d58a8f2515f37b963cc430ff79a7", true},
  {"tir5v2.fw.gz", UINT64_C(9766584), UINT64_C(36145),
   "8e64e7693c7b99fd1e91d93e67755f62a9851c3a0d6627973c942fffa83146f3", true}
};

static const struct member_output trackir_553_members[] = {
  {"poem1.txt", UINT64_C(57504),
   "584027b565f1af62b78dc9e5e2c807733d84eeb18f57b0fff53cab1f953efafa",
   MEMBER_CARVE, UINT64_C(18640), UINT64_C(106),
   "6c38bbcd0261d306a2cc7dd5d9eff3c85730f279a6913807367ddab642d8f09a"},
  {"TIRViews.dll", UINT64_C(122016),
   "5837c42948dfa78a98a45e4aec70d0f238d1f877baf73c7d0c56ecc1c2c8eff5",
   MEMBER_COPY, UINT64_C(0), UINT64_C(122016),
   "5837c42948dfa78a98a45e4aec70d0f238d1f877baf73c7d0c56ecc1c2c8eff5"},
  {"gamedata.txt", UINT64_C(93519),
   "c5206ec0b9dfd4326e1103afde27309fdc8ce02b01499d29ccbd32bc8017457f",
   MEMBER_GAME_DATA, UINT64_C(0), UINT64_C(0),
   "2b554b51dacf4b263604d6e6b6a6378efcb623910c6b9dca300c54703e79c1fd"}
};

static const struct installer_version supported_versions[] = {
  {
    "TrackIR 5.5.3.317",
    "d1b00c9321a010069673764cef0fe26a5aac5ea973eb1d3884263616a77c6adb",
    UINT64_C(1522304), UINT64_C(105131101),
    "a2", UINT64_C(90673152),
    "f34981c21d63f5eb01fb27043cad5d5f3b7c8755b4bdfdf92c8784e8fd917f82",
    UINT64_C(89884669),
    "7ef499ff12248db74c9398da0068573797f247a4bd5e0a43f05d4aa625b435ba",
    "trackir5.exe", UINT64_C(78778528),
    "7546b481f2e864bca4a4b6442c1ce050199811763f33384d001304ef3cdfcbb1",
    trackir_553_carved,
    sizeof(trackir_553_carved) / sizeof(trackir_553_carved[0]),
    trackir_553_members,
    sizeof(trackir_553_members) / sizeof(trackir_553_members[0])
  }
};

static int make_path(char output[PATH_MAX], const char *directory,
                     const char *name)
{
  int length = snprintf(output, PATH_MAX, "%s/%s", directory, name);
  return length >= 0 && length < PATH_MAX ? 0 : -1;
}

static int digest_matches(const unsigned char actual[LTR_SHA256_SIZE],
                          const char *expected_hex)
{
  unsigned char expected[LTR_SHA256_SIZE];
  return ltr_sha256_parse(expected_hex, expected) == 0 &&
         memcmp(actual, expected, sizeof(expected)) == 0;
}

static int verify_file(const char *path, uint64_t expected_size,
                       const char *expected_sha256, const char *description)
{
  struct stat status;
  unsigned char digest[LTR_SHA256_SIZE];
  char actual_hex[65];
  if (stat(path, &status) != 0 || status.st_size < 0 ||
      (uint64_t)status.st_size != expected_size) {
    fprintf(stderr, "%s has an unexpected size\n", description);
    return -1;
  }
  if (ltr_sha256_file(path, digest) != 0) {
    fprintf(stderr, "Could not hash %s: %s\n", description, strerror(errno));
    return -1;
  }
  if (!digest_matches(digest, expected_sha256)) {
    ltr_sha256_hex(digest, actual_hex);
    fprintf(stderr, "%s SHA-256 mismatch\n  expected: %s\n  actual:   %s\n",
            description, expected_sha256, actual_hex);
    return -1;
  }
  return 0;
}

static const struct installer_version *identify_installer(const char *path)
{
  unsigned char digest[LTR_SHA256_SIZE];
  size_t i;
  char hex[65];
  if (ltr_sha256_file(path, digest) != 0) {
    fprintf(stderr, "Could not read installer '%s': %s\n", path, strerror(errno));
    return NULL;
  }
  for (i = 0; i < sizeof(supported_versions) / sizeof(supported_versions[0]); ++i) {
    if (digest_matches(digest, supported_versions[i].installer_sha256)) {
      return &supported_versions[i];
    }
  }
  ltr_sha256_hex(digest, hex);
  fprintf(stderr, "Unsupported TrackIR installer\n  SHA-256: %s\n", hex);
  return NULL;
}

static int extract_msi(const char *installer_path,
                       const struct installer_version *version,
                       const char *output_path)
{
  struct mscab_decompressor *decompressor = NULL;
  struct mscabd_cabinet *cabinets = NULL;
  struct mscabd_cabinet *cabinet;
  struct mscabd_file *member;
  int selftest;
  int result = -1;

  MSPACK_SYS_SELFTEST(selftest);
  if (selftest != MSPACK_ERR_OK) {
    fprintf(stderr, "libmspack file-offset self-test failed (%d)\n", selftest);
    return -1;
  }
  decompressor = mspack_create_cab_decompressor(NULL);
  if (decompressor == NULL) {
    fprintf(stderr, "Could not initialize CAB decompression\n");
    return -1;
  }
  cabinets = decompressor->search(decompressor, installer_path);
  if (cabinets == NULL) {
    fprintf(stderr, "Could not find Burn CAB containers (error %d)\n",
            decompressor->last_error(decompressor));
    goto done;
  }
  for (cabinet = cabinets; cabinet != NULL; cabinet = cabinet->next) {
    if ((uint64_t)cabinet->base_offset != version->attached_cab_offset ||
        (uint64_t)cabinet->length != version->attached_cab_size) continue;
    for (member = cabinet->files; member != NULL; member = member->next) {
      if (strcmp(member->filename, version->msi_member) == 0 &&
          (uint64_t)member->length == version->msi_size) {
        int error = decompressor->extract(decompressor, member, output_path);
        if (error != MSPACK_ERR_OK) {
          fprintf(stderr, "Could not decompress MSI payload (error %d)\n", error);
          goto done;
        }
        result = 0;
        goto done;
      }
    }
  }
  fprintf(stderr, "The expected attached CAB or MSI payload was not found\n");

done:
  if (cabinets != NULL) decompressor->close(decompressor, cabinets);
  mspack_destroy_cab_decompressor(decompressor);
  return result;
}

static int extract_program(const char *cab_path,
                           const struct installer_version *version,
                           const char *output_path)
{
  struct mscab_decompressor *decompressor = NULL;
  struct mscabd_cabinet *cabinet = NULL;
  struct mscabd_file *member;
  int result = -1;

  decompressor = mspack_create_cab_decompressor(NULL);
  if (decompressor == NULL) return -1;
  cabinet = decompressor->open(decompressor, cab_path);
  if (cabinet == NULL) {
    fprintf(stderr, "Could not open embedded MSI cabinet (error %d)\n",
            decompressor->last_error(decompressor));
    goto done;
  }
  for (member = cabinet->files; member != NULL; member = member->next) {
    if (strcmp(member->filename, version->program_member) == 0 &&
        (uint64_t)member->length == version->program_size) {
      int error = decompressor->extract(decompressor, member, output_path);
      if (error != MSPACK_ERR_OK) {
        fprintf(stderr, "Could not decompress %s (error %d)\n",
                version->program_member, error);
        goto done;
      }
      result = 0;
      goto done;
    }
  }
  fprintf(stderr, "%s was not found in the MSI cabinet\n", version->program_member);

done:
  if (cabinet != NULL) decompressor->close(decompressor, cabinet);
  mspack_destroy_cab_decompressor(decompressor);
  return result;
}

static int ensure_destination(const char *path)
{
  struct stat status;
  if (mkdir(path, 0700) == 0) return 0;
  if (errno != EEXIST || stat(path, &status) != 0 || !S_ISDIR(status.st_mode)) {
    fprintf(stderr, "Could not create destination '%s': %s\n", path, strerror(errno));
    return -1;
  }
  return 0;
}

static int output_exists(const char *destination, const char *name)
{
  char path[PATH_MAX];
  struct stat status;
  if (make_path(path, destination, name) != 0) return 1;
  if (lstat(path, &status) == 0) {
    fprintf(stderr, "Refusing to overwrite '%s'\n", path);
    return 1;
  }
  return errno == ENOENT ? 0 : 1;
}

static int write_carved_output(const char *program_path, const char *destination,
                               const struct carved_output *slice)
{
  unsigned char buffer[65536];
  unsigned char digest[LTR_SHA256_SIZE];
  struct ltr_sha256 hash;
  char target[PATH_MAX];
  char temporary[PATH_MAX];
  FILE *input = NULL;
  FILE *raw_output = NULL;
  gzFile output = NULL;
  uint64_t remaining = slice->length;
  int fd = -1;
  int result = -1;

  if (make_path(target, destination, slice->output_name) != 0 ||
      snprintf(temporary, sizeof(temporary), "%s/.%s.XXXXXX",
               destination, slice->output_name) >= (int)sizeof(temporary)) return -1;
  fd = mkstemp(temporary);
  if (fd < 0) goto done;
  if (fchmod(fd, 0600) != 0 || close(fd) != 0) {
    fd = -1;
    goto done;
  }
  fd = -1;
  input = fopen(program_path, "rb");
  if (input == NULL || slice->offset > (uint64_t)INT64_MAX ||
      fseeko(input, (off_t)slice->offset, SEEK_SET) != 0) goto done;
  if (slice->gzip) {
    output = gzopen(temporary, "wb9");
    if (output == NULL || gzbuffer(output, sizeof(buffer)) != 0) goto done;
  } else {
    raw_output = fopen(temporary, "wb");
    if (raw_output == NULL) goto done;
  }
  ltr_sha256_init(&hash);
  while (remaining != 0) {
    size_t amount = remaining < sizeof(buffer) ? (size_t)remaining : sizeof(buffer);
    if (fread(buffer, 1, amount, input) != amount) goto done;
    ltr_sha256_update(&hash, buffer, amount);
    if (slice->gzip) {
      if (gzwrite(output, buffer, (unsigned int)amount) != (int)amount) goto done;
    } else if (fwrite(buffer, 1, amount, raw_output) != amount) {
      goto done;
    }
    remaining -= amount;
  }
  ltr_sha256_final(&hash, digest);
  if (!digest_matches(digest, slice->sha256)) {
    char actual[65];
    ltr_sha256_hex(digest, actual);
    fprintf(stderr, "%s payload SHA-256 mismatch\n  expected: %s\n  actual:   %s\n",
            slice->output_name, slice->sha256, actual);
    goto done;
  }
  if (slice->gzip) {
    if (gzclose(output) != Z_OK) {
      output = NULL;
      goto done;
    }
    output = NULL;
  } else {
    if (fclose(raw_output) != 0) {
      raw_output = NULL;
      goto done;
    }
    raw_output = NULL;
  }
  if (fclose(input) != 0) {
    input = NULL;
    goto done;
  }
  input = NULL;
  if (rename(temporary, target) != 0) goto done;
  printf("Extracted %s\n", target);
  result = 0;

done:
  if (output != NULL) gzclose(output);
  if (raw_output != NULL) fclose(raw_output);
  if (input != NULL) fclose(input);
  if (fd >= 0) close(fd);
  if (result != 0) remove(temporary);
  return result;
}

static int extract_inner_member_by_hash(const char *cab_path,
                                        const struct member_output *wanted,
                                        const char *output_path)
{
  struct mscab_decompressor *decompressor = NULL;
  struct mscabd_cabinet *cabinet = NULL;
  struct mscabd_file *member;
  int result = -1;

  decompressor = mspack_create_cab_decompressor(NULL);
  if (decompressor == NULL) return -1;
  cabinet = decompressor->open(decompressor, cab_path);
  if (cabinet == NULL) {
    fprintf(stderr, "Could not open embedded MSI cabinet (error %d)\n",
            decompressor->last_error(decompressor));
    goto done;
  }
  for (member = cabinet->files; member != NULL; member = member->next) {
    if ((uint64_t)member->length != wanted->member_size) continue;
    remove(output_path);
    if (decompressor->extract(decompressor, member, output_path) != MSPACK_ERR_OK) {
      continue;
    }
    if (verify_file(output_path, wanted->member_size, wanted->member_sha256,
                    "inner MSI payload") == 0) {
      result = 0;
      break;
    }
  }
  if (result != 0) {
    fprintf(stderr, "The payload for %s was not found by SHA-256\n",
            wanted->output_name);
  }

done:
  if (cabinet != NULL) decompressor->close(decompressor, cabinet);
  mspack_destroy_cab_decompressor(decompressor);
  return result;
}

static int copy_file_verified(const char *source_path, const char *destination,
                              const struct member_output *wanted)
{
  unsigned char buffer[65536];
  struct ltr_sha256 hash;
  unsigned char digest[LTR_SHA256_SIZE];
  char target[PATH_MAX];
  char temporary[PATH_MAX];
  FILE *input = NULL;
  FILE *output = NULL;
  size_t count;
  int fd = -1;
  int result = -1;

  if (make_path(target, destination, wanted->output_name) != 0 ||
      snprintf(temporary, sizeof(temporary), "%s/.%s.XXXXXX",
               destination, wanted->output_name) >= (int)sizeof(temporary)) return -1;
  fd = mkstemp(temporary);
  if (fd < 0) goto done;
  if (fchmod(fd, 0600) != 0) goto done;
  output = fdopen(fd, "wb");
  if (output == NULL) goto done;
  fd = -1;
  input = fopen(source_path, "rb");
  if (input == NULL) goto done;
  ltr_sha256_init(&hash);
  while ((count = fread(buffer, 1, sizeof(buffer), input)) != 0) {
    ltr_sha256_update(&hash, buffer, count);
    if (fwrite(buffer, 1, count, output) != count) goto done;
  }
  if (ferror(input)) goto done;
  ltr_sha256_final(&hash, digest);
  if (!digest_matches(digest, wanted->output_sha256)) goto done;
  if (fclose(input) != 0) {
    input = NULL;
    goto done;
  }
  input = NULL;
  if (fflush(output) != 0 || fclose(output) != 0) {
    output = NULL;
    goto done;
  }
  output = NULL;
  if (rename(temporary, target) != 0) goto done;
  printf("Extracted %s\n", target);
  result = 0;

done:
  if (input != NULL) fclose(input);
  if (output != NULL) fclose(output);
  if (fd >= 0) close(fd);
  if (result != 0) remove(temporary);
  return result;
}

static int read_bounded_file(const char *path, unsigned char **data,
                             size_t *length, size_t maximum)
{
  struct stat status;
  FILE *input = NULL;
  unsigned char *buffer = NULL;
  if (stat(path, &status) != 0 || status.st_size < 0 ||
      (uint64_t)status.st_size > maximum) return -1;
  *length = (size_t)status.st_size;
  if (*length == SIZE_MAX) return -1;
  buffer = (unsigned char *)malloc(*length + 1U);
  if (buffer == NULL) return -1;
  input = fopen(path, "rb");
  if (input == NULL || fread(buffer, 1, *length, input) != *length) {
    if (input != NULL) fclose(input);
    free(buffer);
    return -1;
  }
  if (fclose(input) != 0) {
    free(buffer);
    return -1;
  }
  buffer[*length] = '\0';
  *data = buffer;
  return 0;
}

static void rc4_decode(unsigned char *data, size_t length)
{
  static const unsigned char key[16] = {
    0x0e, 0x9a, 0x63, 0x71, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  unsigned char state[256];
  unsigned int i;
  unsigned int j = 0;
  unsigned int stream_i = 0;
  unsigned int stream_j = 0;
  for (i = 0; i < 256; ++i) state[i] = (unsigned char)i;
  for (i = 0; i < 256; ++i) {
    unsigned char temporary;
    j = (j + state[i] + key[i % sizeof(key)]) & 255U;
    temporary = state[i]; state[i] = state[j]; state[j] = temporary;
  }
  for (i = 0; i < length; ++i) {
    unsigned char temporary;
    stream_i = (stream_i + 1U) & 255U;
    stream_j = (stream_j + state[stream_i]) & 255U;
    temporary = state[stream_i]; state[stream_i] = state[stream_j]; state[stream_j] = temporary;
    data[i] ^= state[(state[stream_i] + state[stream_j]) & 255U];
  }
}

static int append_utf8(unsigned char *output, size_t capacity, size_t *used,
                       unsigned int codepoint)
{
  if (codepoint <= 0x7fU) {
    if (*used + 1 >= capacity) return -1;
    output[(*used)++] = (unsigned char)codepoint;
  } else if (codepoint <= 0x7ffU) {
    if (*used + 2 >= capacity) return -1;
    output[(*used)++] = (unsigned char)(0xc0U | (codepoint >> 6));
    output[(*used)++] = (unsigned char)(0x80U | (codepoint & 0x3fU));
  } else if (codepoint <= 0xffffU) {
    if (*used + 3 >= capacity) return -1;
    output[(*used)++] = (unsigned char)(0xe0U | (codepoint >> 12));
    output[(*used)++] = (unsigned char)(0x80U | ((codepoint >> 6) & 0x3fU));
    output[(*used)++] = (unsigned char)(0x80U | (codepoint & 0x3fU));
  } else if (codepoint <= 0x10ffffU) {
    if (*used + 4 >= capacity) return -1;
    output[(*used)++] = (unsigned char)(0xf0U | (codepoint >> 18));
    output[(*used)++] = (unsigned char)(0x80U | ((codepoint >> 12) & 0x3fU));
    output[(*used)++] = (unsigned char)(0x80U | ((codepoint >> 6) & 0x3fU));
    output[(*used)++] = (unsigned char)(0x80U | (codepoint & 0x3fU));
  } else {
    return -1;
  }
  return 0;
}

static int xml_unescape(const unsigned char *input, size_t length,
                        unsigned char *output, size_t capacity, size_t *used)
{
  size_t i = 0;
  *used = 0;
  while (i < length) {
    unsigned int codepoint = 0;
    size_t entity_length = 0;
    if (input[i] != '&') {
      if (*used + 1 >= capacity) return -1;
      output[(*used)++] = input[i++];
      continue;
    }
    if (i + 6 <= length && memcmp(input + i, "&quot;", 6) == 0) {
      codepoint = '"'; entity_length = 6;
    } else if (i + 6 <= length && memcmp(input + i, "&apos;", 6) == 0) {
      codepoint = '\''; entity_length = 6;
    } else if (i + 4 <= length && memcmp(input + i, "&lt;", 4) == 0) {
      codepoint = '<'; entity_length = 4;
    } else if (i + 4 <= length && memcmp(input + i, "&gt;", 4) == 0) {
      codepoint = '>'; entity_length = 4;
    } else if (i + 5 <= length && memcmp(input + i, "&amp;", 5) == 0) {
      codepoint = '&'; entity_length = 5;
    } else if (i + 3 < length && input[i + 1] == '#') {
      size_t j = i + 2;
      unsigned int base = 10;
      if (j < length && (input[j] == 'x' || input[j] == 'X')) {
        base = 16; ++j;
      }
      if (j < length) {
        size_t digits = j;
        while (j < length && input[j] != ';') {
          int value;
          if (base == 16) {
            if (input[j] >= '0' && input[j] <= '9') value = input[j] - '0';
            else if (input[j] >= 'a' && input[j] <= 'f') value = input[j] - 'a' + 10;
            else if (input[j] >= 'A' && input[j] <= 'F') value = input[j] - 'A' + 10;
            else value = -1;
          } else {
            value = (input[j] >= '0' && input[j] <= '9') ? input[j] - '0' : -1;
          }
          if (value < 0) { digits = j; break; }
          codepoint = codepoint * base + (unsigned int)value;
          ++j;
        }
        if (digits != j || j >= length || input[j] != ';') {
          codepoint = 0; entity_length = 0;
        } else {
          entity_length = j - i + 1;
        }
      }
    }
    if (entity_length == 0) {
      if (*used + 1 >= capacity) return -1;
      output[(*used)++] = input[i++];
    } else {
      if (append_utf8(output, capacity, used, codepoint) != 0) return -1;
      i += entity_length;
    }
  }
  output[*used] = '\0';
  return 0;
}

static int xml_attribute(const unsigned char *begin, const unsigned char *end,
                         const char *wanted, unsigned char *output,
                         size_t capacity)
{
  size_t wanted_length = strlen(wanted);
  const unsigned char *p = begin;
  while (p < end) {
    const unsigned char *name;
    const unsigned char *value_begin;
    const unsigned char *value_end;
    unsigned char quote;
    size_t output_length;

    while (p < end && !(isalnum((unsigned char)*p) || *p == '_' ||
                        *p == ':' || *p == '-')) ++p;
    name = p;
    while (p < end && (isalnum((unsigned char)*p) || *p == '_' ||
                       *p == ':' || *p == '-')) ++p;
    if (p == name) continue;
    if ((size_t)(p - name) != wanted_length ||
        memcmp(name, wanted, wanted_length) != 0) continue;
    while (p < end && isspace((unsigned char)*p)) ++p;
    if (p >= end || *p != '=') continue;
    ++p;
    while (p < end && isspace((unsigned char)*p)) ++p;
    if (p >= end || (*p != '\'' && *p != '"')) continue;
    quote = *p++;
    value_begin = p;
    while (p < end && *p != quote) ++p;
    if (p >= end) return -1;
    value_end = p;
    if (xml_unescape(value_begin, (size_t)(value_end - value_begin),
                     output, capacity, &output_length) == 0) return 0;
  }
  return -1;
}

static void trim_ascii(unsigned char *text, size_t *length)
{
  size_t begin = 0;
  while (begin < *length && isspace((unsigned char)text[begin])) ++begin;
  while (*length > begin && isspace((unsigned char)text[*length - 1])) --(*length);
  if (begin != 0) memmove(text, text + begin, *length - begin);
  *length -= begin;
  text[*length] = '\0';
}

static int write_game_data(const char *source_path, const char *destination,
                           const struct member_output *wanted)
{
  unsigned char *xml = NULL;
  unsigned char *output_data = NULL;
  size_t xml_length = 0;
  size_t output_capacity;
  size_t output_length = 0;
  char target[PATH_MAX];
  char temporary[PATH_MAX] = {0};
  FILE *output = NULL;
  unsigned char digest[LTR_SHA256_SIZE];
  int result = -1;

  if (read_bounded_file(source_path, &xml, &xml_length, UINT64_C(16) * 1024U * 1024U) != 0) goto done;
  rc4_decode(xml, xml_length);
  output_capacity = xml_length * 4U + 1U;
  if (output_capacity < xml_length || output_capacity > SIZE_MAX / 2U) goto done;
  output_data = (unsigned char *)malloc(output_capacity);
  if (output_data == NULL) goto done;

  {
    const unsigned char *cursor = xml;
    const unsigned char *end = xml + xml_length;
    while (cursor < end) {
      const unsigned char *game = (const unsigned char *)strstr((const char *)cursor, "<Game");
      const unsigned char *open_end;
      const unsigned char *close;
      unsigned char id[1024];
      unsigned char name[8192];
      unsigned char application_id[1024];
      size_t id_length = 0;
      size_t name_length = 0;
      size_t app_length = 0;
      if (game == NULL || game >= end) break;
      if (game + 5 < end && (isalnum(game[5]) || game[5] == '_' || game[5] == '-')) {
        cursor = game + 5;
        continue;
      }
      open_end = (const unsigned char *)memchr(game, '>', (size_t)(end - game));
      if (open_end == NULL) break;
      close = (const unsigned char *)strstr((const char *)(open_end + 1), "</Game>");
      if (close == NULL || close > end) break;
      if (xml_attribute(game + 5, open_end, "Id", id, sizeof(id)) != 0 ||
          xml_attribute(game + 5, open_end, "Name", name, sizeof(name)) != 0) {
        cursor = close + 7;
        continue;
      }
      id_length = strlen((const char *)id);
      name_length = strlen((const char *)name);
      {
        const unsigned char *app = (const unsigned char *)strstr((const char *)(open_end + 1), "<ApplicationID");
        if (app != NULL && app < close) {
          const unsigned char *app_end = (const unsigned char *)memchr(app, '>', (size_t)(close - app));
          if (app_end != NULL && app_end > app && app_end[-1] != '/') {
            const unsigned char *app_close = (const unsigned char *)strstr((const char *)(app_end + 1), "</ApplicationID>");
            if (app_close != NULL && app_close < close) {
              size_t raw_length = (size_t)(app_close - app_end - 1);
              if (xml_unescape(app_end + 1, raw_length, application_id,
                               sizeof(application_id), &app_length) == 0) {
                size_t p = 0;
                size_t q = 0;
                while (p < app_length) {
                  if (application_id[p] == '<') {
                    while (p < app_length && application_id[p] != '>') ++p;
                    if (p < app_length) ++p;
                  } else {
                    application_id[q++] = application_id[p++];
                  }
                }
                app_length = q;
                application_id[app_length] = '\0';
                trim_ascii(application_id, &app_length);
              }
            }
          }
        }
      }
      if (output_length + id_length + name_length + app_length + 8U >= output_capacity) goto done;
      output_length += (size_t)snprintf((char *)output_data + output_length,
                                        output_capacity - output_length,
                                        "%s \"%s\"", id, name);
      if (app_length != 0) {
        output_length += (size_t)snprintf((char *)output_data + output_length,
                                          output_capacity - output_length,
                                          " (%s)", application_id);
      }
      output_data[output_length++] = '\n';
      output_data[output_length] = '\0';
      cursor = close + 7;
    }
  }

  if (make_path(target, destination, wanted->output_name) != 0 ||
      snprintf(temporary, sizeof(temporary), "%s/.%s.XXXXXX",
               destination, wanted->output_name) >= (int)sizeof(temporary)) goto done;
  {
    int fd = mkstemp(temporary);
    if (fd < 0) goto done;
    if (fchmod(fd, 0600) != 0) { close(fd); goto done; }
    output = fdopen(fd, "wb");
    if (output == NULL) { close(fd); goto done; }
  }
  if (fwrite(output_data, 1, output_length, output) != output_length ||
      fflush(output) != 0 || fclose(output) != 0) {
    output = NULL;
    goto done;
  }
  output = NULL;
  if (ltr_sha256_file(temporary, digest) != 0 ||
      !digest_matches(digest, wanted->output_sha256) ||
      rename(temporary, target) != 0) goto done;
  printf("Extracted %s\n", target);
  result = 0;

done:
  if (output != NULL) fclose(output);
  if (result != 0 && temporary[0] != '\0') remove(temporary);
  free(xml);
  free(output_data);
  return result;
}

int ltr_native_extract_installer(const char *installer_path,
                                 const char *destination,
                                 bool list_only)
{
  const struct installer_version *version;
  char temp_template[] = "/tmp/ltr-extractor.XXXXXX";
  char msi_path[PATH_MAX] = {0};
  char cab_path[PATH_MAX] = {0};
  char program_path[PATH_MAX] = {0};
  char member_path[PATH_MAX] = {0};
  char *temp_directory = NULL;
  size_t i;
  int result = -1;

  if (installer_path == NULL || (destination == NULL && !list_only)) {
    fprintf(stderr, "Installer and destination are required\n");
    return -1;
  }
  version = identify_installer(installer_path);
  if (version == NULL) return -1;
  printf("Recognized %s\n", version->description);
  for (i = 0; i < version->carved_count; ++i) {
    printf("  %s: raw size %" PRIu64 ", SHA-256 %s\n",
           version->carved[i].output_name,
           version->carved[i].length,
           version->carved[i].sha256);
  }
  for (i = 0; i < version->member_count; ++i) {
    printf("  %s: source size %" PRIu64 ", output SHA-256 %s\n",
           version->members[i].output_name,
           version->members[i].member_size,
           version->members[i].output_sha256);
  }
  if (list_only) return 0;

  if (ensure_destination(destination) != 0) return -1;
  for (i = 0; i < version->carved_count; ++i) {
    if (output_exists(destination, version->carved[i].output_name)) return -1;
  }
  for (i = 0; i < version->member_count; ++i) {
    if (output_exists(destination, version->members[i].output_name)) return -1;
  }

  temp_directory = mkdtemp(temp_template);
  if (temp_directory == NULL ||
      make_path(msi_path, temp_directory, "installer.msi") != 0 ||
      make_path(cab_path, temp_directory, "payload.cab") != 0 ||
      make_path(program_path, temp_directory, "trackir5.exe") != 0 ||
      make_path(member_path, temp_directory, "member.bin") != 0) {
    fprintf(stderr, "Could not create private temporary storage: %s\n", strerror(errno));
    goto done;
  }

  printf("Reading WiX Burn CAB containers...\n");
  if (extract_msi(installer_path, version, msi_path) != 0 ||
      verify_file(msi_path, version->msi_size, version->msi_sha256,
                  "MSI payload") != 0) goto done;

  printf("Reading the embedded MSI cabinet...\n");
  if (ltr_cfb_extract_stream_by_size(msi_path, version->inner_cab_size,
                                     cab_path) != 0 ||
      verify_file(cab_path, version->inner_cab_size, version->inner_cab_sha256,
                  "embedded MSI cabinet") != 0) goto done;

  printf("Decompressing %s...\n", version->program_member);
  if (extract_program(cab_path, version, program_path) != 0 ||
      verify_file(program_path, version->program_size, version->program_sha256,
                  version->program_member) != 0) goto done;

  for (i = 0; i < version->member_count; ++i) {
    const struct member_output *member = &version->members[i];
    if (extract_inner_member_by_hash(cab_path, member, member_path) != 0) goto done;
    if (member->action == MEMBER_COPY) {
      if (copy_file_verified(member_path, destination, member) != 0) goto done;
    } else if (member->action == MEMBER_GAME_DATA) {
      if (write_game_data(member_path, destination, member) != 0) goto done;
    } else {
      struct carved_output slice = {
        member->output_name, member->offset, member->length,
        member->output_sha256, false
      };
      if (write_carved_output(member_path, destination, &slice) != 0) goto done;
    }
    remove(member_path);
  }

  for (i = 0; i < version->carved_count; ++i) {
    if (write_carved_output(program_path, destination, &version->carved[i]) != 0) goto done;
  }
  result = 0;

done:
  if (member_path[0] != '\0') remove(member_path);
  if (program_path[0] != '\0') remove(program_path);
  if (cab_path[0] != '\0') remove(cab_path);
  if (msi_path[0] != '\0') remove(msi_path);
  if (temp_directory != NULL) rmdir(temp_directory);
  return result;
}
