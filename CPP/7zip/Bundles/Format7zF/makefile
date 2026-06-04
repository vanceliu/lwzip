PROG = 7z.dll
# USE_C_LZFINDOPT = 1
DEF_FILE = ../../Archive/Archive2.def
CFLAGS = $(CFLAGS) \
  -DZ7_EXTERNAL_CODECS \

!IFNDEF UNDER_CE
!ENDIF

!include "Arc.mak"

COMPRESS_OBJS = $(COMPRESS_OBJS) \
  $O\CodecExports.obj \

AR_OBJS = $(AR_OBJS) \
  $O\ArchiveExports.obj \
  $O\DllExports2.obj \

LWZIP_OBJS = \
  $O\LWZipCodePage.obj \

CURRENT_OBJS = $(LWZIP_OBJS)

!include "../../7zip.mak"

$O\LWZipCodePage.obj: ../../LWZipCodePage.cpp
	$(COMPL_O2)
