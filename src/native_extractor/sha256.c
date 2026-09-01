#include "sha256.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static const uint32_t round_constants[64] = {
  0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U,
  0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U,
  0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U,
  0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U,
  0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU,
  0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
  0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U,
  0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U,
  0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U,
  0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U,
  0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U,
  0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
  0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U,
  0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U,
  0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U,
  0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U
};

static uint32_t rotate_right(uint32_t value, unsigned int amount)
{
  return (value >> amount) | (value << (32U - amount));
}

static uint32_t load_be32(const unsigned char *p)
{
  return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
         ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static void store_be32(unsigned char *p, uint32_t value)
{
  p[0] = (unsigned char)(value >> 24);
  p[1] = (unsigned char)(value >> 16);
  p[2] = (unsigned char)(value >> 8);
  p[3] = (unsigned char)value;
}

static void transform(struct ltr_sha256 *ctx, const unsigned char block[64])
{
  uint32_t words[64];
  uint32_t a, b, c, d, e, f, g, h;
  unsigned int i;

  for (i = 0; i < 16; ++i) words[i] = load_be32(block + (i * 4U));
  for (; i < 64; ++i) {
    uint32_t s0 = rotate_right(words[i - 15], 7) ^
                  rotate_right(words[i - 15], 18) ^ (words[i - 15] >> 3);
    uint32_t s1 = rotate_right(words[i - 2], 17) ^
                  rotate_right(words[i - 2], 19) ^ (words[i - 2] >> 10);
    words[i] = words[i - 16] + s0 + words[i - 7] + s1;
  }

  a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];
  e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];
  for (i = 0; i < 64; ++i) {
    uint32_t sum1 = rotate_right(e, 6) ^ rotate_right(e, 11) ^ rotate_right(e, 25);
    uint32_t choice = (e & f) ^ ((~e) & g);
    uint32_t t1 = h + sum1 + choice + round_constants[i] + words[i];
    uint32_t sum0 = rotate_right(a, 2) ^ rotate_right(a, 13) ^ rotate_right(a, 22);
    uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
    uint32_t t2 = sum0 + majority;
    h = g; g = f; f = e; e = d + t1;
    d = c; c = b; b = a; a = t1 + t2;
  }
  ctx->state[0] += a; ctx->state[1] += b; ctx->state[2] += c; ctx->state[3] += d;
  ctx->state[4] += e; ctx->state[5] += f; ctx->state[6] += g; ctx->state[7] += h;
}

void ltr_sha256_init(struct ltr_sha256 *ctx)
{
  static const uint32_t initial[8] = {
    0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU,
    0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U
  };
  memcpy(ctx->state, initial, sizeof(initial));
  ctx->bytes = 0;
  ctx->used = 0;
}

void ltr_sha256_update(struct ltr_sha256 *ctx, const void *data, size_t length)
{
  const unsigned char *input = (const unsigned char *)data;
  ctx->bytes += length;
  while (length != 0) {
    size_t room = sizeof(ctx->block) - ctx->used;
    size_t take = length < room ? length : room;
    memcpy(ctx->block + ctx->used, input, take);
    ctx->used += take;
    input += take;
    length -= take;
    if (ctx->used == sizeof(ctx->block)) {
      transform(ctx, ctx->block);
      ctx->used = 0;
    }
  }
}

void ltr_sha256_final(struct ltr_sha256 *ctx, unsigned char digest[LTR_SHA256_SIZE])
{
  uint64_t bits = ctx->bytes * 8U;
  unsigned int i;
  ctx->block[ctx->used++] = 0x80;
  if (ctx->used > 56) {
    memset(ctx->block + ctx->used, 0, sizeof(ctx->block) - ctx->used);
    transform(ctx, ctx->block);
    ctx->used = 0;
  }
  memset(ctx->block + ctx->used, 0, 56 - ctx->used);
  for (i = 0; i < 8; ++i) ctx->block[63U - i] = (unsigned char)(bits >> (i * 8U));
  transform(ctx, ctx->block);
  for (i = 0; i < 8; ++i) store_be32(digest + (i * 4U), ctx->state[i]);
  memset(ctx, 0, sizeof(*ctx));
}

int ltr_sha256_file(const char *path, unsigned char digest[LTR_SHA256_SIZE])
{
  unsigned char buffer[65536];
  struct ltr_sha256 ctx;
  FILE *file = fopen(path, "rb");
  size_t count;
  if (file == NULL) return -1;
  ltr_sha256_init(&ctx);
  while ((count = fread(buffer, 1, sizeof(buffer), file)) != 0) {
    ltr_sha256_update(&ctx, buffer, count);
  }
  if (ferror(file)) {
    int saved = errno;
    fclose(file);
    errno = saved;
    return -1;
  }
  if (fclose(file) != 0) return -1;
  ltr_sha256_final(&ctx, digest);
  return 0;
}

static int hex_value(char c)
{
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

int ltr_sha256_parse(const char *hex, unsigned char digest[LTR_SHA256_SIZE])
{
  size_t i;
  if (hex == NULL || strlen(hex) != LTR_SHA256_SIZE * 2U) return -1;
  for (i = 0; i < LTR_SHA256_SIZE; ++i) {
    int high = hex_value(hex[i * 2U]);
    int low = hex_value(hex[i * 2U + 1U]);
    if (high < 0 || low < 0) return -1;
    digest[i] = (unsigned char)((high << 4) | low);
  }
  return 0;
}

void ltr_sha256_hex(const unsigned char digest[LTR_SHA256_SIZE], char hex[65])
{
  static const char digits[] = "0123456789abcdef";
  size_t i;
  for (i = 0; i < LTR_SHA256_SIZE; ++i) {
    hex[i * 2U] = digits[digest[i] >> 4];
    hex[i * 2U + 1U] = digits[digest[i] & 15U];
  }
  hex[64] = '\0';
}
