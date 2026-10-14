#define _FILE_OFFSET_BITS 64
#define _POSIX_C_SOURCE 200809L

#include "cfb.h"

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define CFB_FREESECT 0xffffffffU
#define CFB_ENDOFCHAIN 0xfffffffeU
#define CFB_FATSECT 0xfffffffdU
#define CFB_DIFSECT 0xfffffffcU

struct cfb_reader {
  FILE *file;
  uint64_t file_size;
  uint32_t sector_size;
  uint32_t sector_count;
  uint32_t *fat;
  size_t fat_entries;
};

static uint16_t read_u16(const unsigned char *p)
{
  return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}

static uint32_t read_u32(const unsigned char *p)
{
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
         ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint64_t read_u64(const unsigned char *p)
{
  return (uint64_t)read_u32(p) | ((uint64_t)read_u32(p + 4) << 32);
}

static int read_exact(FILE *file, void *buffer, size_t length)
{
  return fread(buffer, 1, length, file) == length ? 0 : -1;
}

static int seek_absolute(FILE *file, uint64_t offset)
{
  if (offset > (uint64_t)INT64_MAX) return -1;
  return fseeko(file, (off_t)offset, SEEK_SET);
}

static int read_sector(const struct cfb_reader *cfb, uint32_t sector,
                       unsigned char *buffer)
{
  uint64_t offset;
  if (sector >= cfb->sector_count) return -1;
  offset = ((uint64_t)sector + 1U) * cfb->sector_size;
  if (offset > cfb->file_size || cfb->file_size - offset < cfb->sector_size) return -1;
  if (seek_absolute(cfb->file, offset) != 0) return -1;
  return read_exact(cfb->file, buffer, cfb->sector_size);
}

static int valid_chain_sector(const struct cfb_reader *cfb, uint32_t sector)
{
  return sector < cfb->sector_count && sector < cfb->fat_entries;
}

static int load_fat(struct cfb_reader *cfb, const unsigned char header[512],
                    uint32_t num_fat, uint32_t first_difat, uint32_t num_difat)
{
  uint32_t *fat_sectors = NULL;
  unsigned char *sector_data = NULL;
  unsigned char *seen_difat = NULL;
  size_t per_difat = cfb->sector_size / 4U - 1U;
  size_t count = 0;
  uint32_t current = first_difat;
  uint32_t i;
  int result = -1;

  if (num_fat == 0 || num_fat > cfb->sector_count) return -1;
  fat_sectors = (uint32_t *)calloc(num_fat, sizeof(*fat_sectors));
  sector_data = (unsigned char *)malloc(cfb->sector_size);
  seen_difat = (unsigned char *)calloc(cfb->sector_count, 1);
  if (fat_sectors == NULL || sector_data == NULL || seen_difat == NULL) goto done;

  for (i = 0; i < 109 && count < num_fat; ++i) {
    uint32_t value = read_u32(header + 76U + (i * 4U));
    if (value != CFB_FREESECT) fat_sectors[count++] = value;
  }

  for (i = 0; count < num_fat && i < num_difat; ++i) {
    size_t j;
    if (current == CFB_ENDOFCHAIN || current == CFB_FREESECT ||
        current >= cfb->sector_count || seen_difat[current]) goto done;
    seen_difat[current] = 1;
    if (read_sector(cfb, current, sector_data) != 0) goto done;
    for (j = 0; j < per_difat && count < num_fat; ++j) {
      uint32_t value = read_u32(sector_data + (j * 4U));
      if (value != CFB_FREESECT) fat_sectors[count++] = value;
    }
    current = read_u32(sector_data + (per_difat * 4U));
  }
  if (count != num_fat) goto done;

  cfb->fat_entries = (size_t)num_fat * (cfb->sector_size / 4U);
  cfb->fat = (uint32_t *)malloc(cfb->fat_entries * sizeof(*cfb->fat));
  if (cfb->fat == NULL) goto done;
  for (i = 0; i < num_fat; ++i) {
    size_t j;
    if (fat_sectors[i] >= cfb->sector_count ||
        read_sector(cfb, fat_sectors[i], sector_data) != 0) goto done;
    for (j = 0; j < cfb->sector_size / 4U; ++j) {
      cfb->fat[(size_t)i * (cfb->sector_size / 4U) + j] =
        read_u32(sector_data + (j * 4U));
    }
  }
  result = 0;

done:
  if (result != 0) {
    free(cfb->fat);
    cfb->fat = NULL;
    cfb->fat_entries = 0;
  }
  free(fat_sectors);
  free(sector_data);
  free(seen_difat);
  return result;
}

static int find_stream(const struct cfb_reader *cfb, uint32_t first_directory,
                       uint16_t major_version, uint64_t expected_size,
                       uint32_t *start_sector)
{
  unsigned char *sector_data = NULL;
  unsigned char *seen = NULL;
  uint32_t current = first_directory;
  unsigned int matches = 0;
  int result = -1;

  sector_data = (unsigned char *)malloc(cfb->sector_size);
  seen = (unsigned char *)calloc(cfb->sector_count, 1);
  if (sector_data == NULL || seen == NULL) goto done;

  while (current != CFB_ENDOFCHAIN) {
    size_t offset;
    if (!valid_chain_sector(cfb, current) || seen[current]) goto done;
    seen[current] = 1;
    if (read_sector(cfb, current, sector_data) != 0) goto done;
    for (offset = 0; offset + 128U <= cfb->sector_size; offset += 128U) {
      const unsigned char *entry = sector_data + offset;
      uint16_t name_bytes = read_u16(entry + 64);
      unsigned int type = entry[66];
      uint64_t size = read_u64(entry + 120);
      if (major_version == 3) size &= UINT64_C(0xffffffff);
      if (type == 2 && name_bytes >= 2 && name_bytes <= 64 && size == expected_size) {
        *start_sector = read_u32(entry + 116);
        ++matches;
      }
    }
    current = cfb->fat[current];
  }
  if (matches != 1) {
    fprintf(stderr, "MSI contains %u streams of expected size %" PRIu64 " (wanted one)\n",
            matches, expected_size);
    goto done;
  }
  result = 0;

done:
  free(sector_data);
  free(seen);
  return result;
}

static int write_stream(const struct cfb_reader *cfb, uint32_t first_sector,
                        uint64_t size, const char *output_path)
{
  unsigned char *sector_data = NULL;
  unsigned char *seen = NULL;
  FILE *output = NULL;
  uint32_t current = first_sector;
  uint64_t remaining = size;
  int result = -1;

  sector_data = (unsigned char *)malloc(cfb->sector_size);
  seen = (unsigned char *)calloc(cfb->sector_count, 1);
  output = fopen(output_path, "wb");
  if (sector_data == NULL || seen == NULL || output == NULL) goto done;

  while (remaining != 0) {
    size_t amount = remaining < cfb->sector_size ? (size_t)remaining : cfb->sector_size;
    if (!valid_chain_sector(cfb, current) || seen[current]) goto done;
    seen[current] = 1;
    if (read_sector(cfb, current, sector_data) != 0) goto done;
    if (fwrite(sector_data, 1, amount, output) != amount) goto done;
    remaining -= amount;
    current = cfb->fat[current];
  }
  if (current != CFB_ENDOFCHAIN && current != CFB_FREESECT) goto done;
  if (fflush(output) != 0 || fclose(output) != 0) {
    output = NULL;
    goto done;
  }
  output = NULL;
  result = 0;

done:
  if (output != NULL) fclose(output);
  if (result != 0) remove(output_path);
  free(sector_data);
  free(seen);
  return result;
}

int ltr_cfb_extract_stream_by_size(const char *input_path,
                                   uint64_t expected_size,
                                   const char *output_path)
{
  static const unsigned char signature[8] =
    {0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1};
  unsigned char header[512];
  struct stat status;
  struct cfb_reader cfb;
  uint16_t major_version;
  uint16_t sector_shift;
  uint32_t first_directory;
  uint32_t stream_sector;
  int result = -1;

  memset(&cfb, 0, sizeof(cfb));
  if (stat(input_path, &status) != 0 || status.st_size < 512) return -1;
  cfb.file_size = (uint64_t)status.st_size;
  cfb.file = fopen(input_path, "rb");
  if (cfb.file == NULL) return -1;
  if (read_exact(cfb.file, header, sizeof(header)) != 0 ||
      memcmp(header, signature, sizeof(signature)) != 0 ||
      read_u16(header + 28) != 0xfffeU) {
    fprintf(stderr, "MSI is not a supported Compound File Binary document\n");
    goto done;
  }

  major_version = read_u16(header + 26);
  sector_shift = read_u16(header + 30);
  if (!((major_version == 3 && sector_shift == 9) ||
        (major_version == 4 && sector_shift == 12))) {
    fprintf(stderr, "Unsupported CFB version or sector size\n");
    goto done;
  }
  cfb.sector_size = UINT32_C(1) << sector_shift;
  if (cfb.file_size < cfb.sector_size ||
      (cfb.file_size - cfb.sector_size) % cfb.sector_size != 0) goto done;
  if ((cfb.file_size - cfb.sector_size) / cfb.sector_size > UINT32_MAX) goto done;
  cfb.sector_count = (uint32_t)((cfb.file_size - cfb.sector_size) / cfb.sector_size);
  first_directory = read_u32(header + 48);

  if (load_fat(&cfb, header, read_u32(header + 44),
               read_u32(header + 68), read_u32(header + 72)) != 0) {
    fprintf(stderr, "Could not read the MSI allocation table\n");
    goto done;
  }
  if (find_stream(&cfb, first_directory, major_version, expected_size,
                  &stream_sector) != 0) goto done;
  if (expected_size < read_u32(header + 56)) {
    fprintf(stderr, "Expected cabinet unexpectedly uses the CFB mini stream\n");
    goto done;
  }
  if (write_stream(&cfb, stream_sector, expected_size, output_path) != 0) {
    fprintf(stderr, "Could not extract the embedded cabinet stream\n");
    goto done;
  }
  result = 0;

done:
  free(cfb.fat);
  if (cfb.file != NULL) fclose(cfb.file);
  return result;
}
