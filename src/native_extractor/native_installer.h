#ifndef LTR_NATIVE_INSTALLER_H
#define LTR_NATIVE_INSTALLER_H

#include <stdbool.h>

int ltr_native_extract_installer(const char *installer_path,
                                 const char *destination,
                                 bool list_only);

#endif
