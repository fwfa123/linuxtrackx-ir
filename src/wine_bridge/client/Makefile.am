noinst_SCRIPTS = 
if WINE_PLUGIN
  noinst_SCRIPTS += NPClient.dll check_data.exe
if WINE64
  noinst_SCRIPTS += NPClient64.dll
endif #WINE64
endif #WINE_PLUGIN

if DARWIN
  LDFLAGS += -Wl,-no_arch_warnings
else
  LDFLAGS += -Wl,--no-warn-search-mismatch
endif

# Use MinGW compilers when available, otherwise fall back to winegcc
CC = $(MINGW32_CC)
CXX = $(MINGW32_CXX)

SUFFIXES = .o .cpp .c .rc 64.o

.cpp.o :
	$(CXX) -c $(CXXFLAGS_PRE) $(CXXFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.c.o :
	$(CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.cpp64.o :
	$(MINGW64_CXX) -c $(CXXFLAGS_PRE) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.c64.o :
	$(MINGW64_CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -o $@ $<

.rc.o :
	$(MINGW32_WINDRES) -o $@ $(RCFLAGS) $<

CXXFLAGS_PRE = -g -DHAVE_CONFIG_H -I../../.. -I. -I@srcdir@/../.. -I@top_builddir@
CFLAGS_PRE = -g -I../.. -I../../.. -DHAVE_CONFIG_H -I@srcdir@/../.. -I@top_builddir@
RCFLAGS = -I @srcdir@
#VPATH = .:../..:@srcdir@/../..:@top_builddir@:@srcdir@
vpath %.h @srcdir@/../..
vpath %.h @top_builddir@
vpath %.c @srcdir@
vpath %.c @srcdir@/../..

check_data.exe : check_data.o rest.o
	$(CC) $(WINE_LIBS) $(LDFLAGS) -m32 -Wall -Wextra -g -o $@ $^

NPClient.dll : NPClient.def NPClient_main.o linuxtrack.o rest.o
	$(CC) $(WINE_LIBS) $(LDFLAGS) -m32 -Wall -Wextra -g -o $@ -shared NPClient_main.o linuxtrack.o rest.o NPClient.def

NPClient64.dll : NPClient64.def NPClient_main64.o linuxtrack64.o rest64.o
	$(MINGW64_CC) $(WINE64_LIBS) $(LDFLAGS) -Wall -Wextra -g -o $@ -shared NPClient_main64.o linuxtrack64.o rest64.o NPClient64.def

check_data.o : check_data.c rest.h

NPClient_main.o : NPClient_main.c NPClient_dll.h

NPClient_main64.o : NPClient_main.c NPClient_dll.h
	$(MINGW64_CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -o $@ $<

linuxtrack.o : linuxtrack.c config.h

linuxtrack64.o : linuxtrack.c config.h
	$(MINGW64_CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -o $@ ./../../linuxtrack.c

rest.o : rest.c rest.h config.h

rest64.o : rest.c rest.h config.h
	$(MINGW64_CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -o $@ rest.c

clean-local: clean-local-check
.PHONY: clean-local-check
clean-local-check:
	rm -f *.exe* *.dll* *.sh *.o

distclean-local: distclean-local-check
.PHONY: distclean-local-check
distclean-local-check:
	rm -f *.exe* *.dll* *.sh *.o

EXTRA_DIST = NPClient_dll.h NPClient_main.c rest.c \
NPClient.h NPClient.spec NPClient.def NPClient64.def rest.h check_data.c

