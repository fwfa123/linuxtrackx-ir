CC ?= cc
CPPFLAGS += -D_FILE_OFFSET_BITS=64 -D_POSIX_C_SOURCE=200809L -DHAVE_FSEEKO=1
CFLAGS ?= -O2
CFLAGS += -std=c99 -Wall -Wextra -Wpedantic
LDLIBS += -lz

MSPACK := third_party/libmspack
SOURCES := main.c native_installer.c cfb.c sha256.c \
           $(MSPACK)/system.c $(MSPACK)/cabd.c $(MSPACK)/lzxd.c \
           $(MSPACK)/mszipd.c $(MSPACK)/qtmd.c
OBJECTS := $(SOURCES:.c=.o)

.PHONY: all clean

all: ltr_installer_extractor

ltr_installer_extractor: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -I$(MSPACK) -c -o $@ $<

clean:
	$(RM) ltr_installer_extractor $(OBJECTS)
