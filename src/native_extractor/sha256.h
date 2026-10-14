#ifndef LTR_NATIVE_SHA256_H
#define LTR_NATIVE_SHA256_H

#include <stddef.h>
#include <stdint.h>

#define LTR_SHA256_SIZE 32

struct ltr_sha256 {
  uint32_t state[8];
  uint64_t bytes;
  unsigned char block[64];
  size_t used;
};

void ltr_sha256_init(struct ltr_sha256 *ctx);
void ltr_sha256_update(struct ltr_sha256 *ctx, const void *data, size_t length);
void ltr_sha256_final(struct ltr_sha256 *ctx, unsigned char digest[LTR_SHA256_SIZE]);
int ltr_sha256_file(const char *path, unsigned char digest[LTR_SHA256_SIZE]);
int ltr_sha256_parse(const char *hex, unsigned char digest[LTR_SHA256_SIZE]);
void ltr_sha256_hex(const unsigned char digest[LTR_SHA256_SIZE], char hex[65]);

#endif
