// LWZipCodePage.h
// LWZip: Global codepage override for archive filename display

#ifndef LWZIP_CODEPAGE_H
#define LWZIP_CODEPAGE_H

#include "../../C/7zTypes.h"

// Global codepage override. When set (non-zero), archive handlers
// use this codepage instead of their default for filename decoding.
extern UInt32 g_LWZip_CodePage;
extern bool g_LWZip_ForceCodePage;

#endif
