#ifndef LTR_NATIVE_CFB_H
#define LTR_NATIVE_CFB_H

#include <stdint.h>

/* Extract the unique regular CFB stream with the expected size. */
int ltr_cfb_extract_stream_by_size(const char *input_path,
                                   uint64_t expected_size,
                                   const char *output_path);

#endif
