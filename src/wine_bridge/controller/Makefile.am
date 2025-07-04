noinst_SCRIPTS = 
if WINE_PLUGIN
  noinst_SCRIPTS += Controller.exe
endif #WINE_PLUGIN

if DARWIN
  LDFLAGS += -Wl,-no_arch_warnings
else
  LDFLAGS += -Wl,--no-warn-search-mismatch
endif

# Use MinGW compilers when available, otherwise fall back to winegcc
WINE_CC = $(MINGW32_CC)
WINE_CXX = $(MINGW32_CXX)
WINE_RC = $(MINGW32_WINDRES)
MINGW_LIBS = -lkernel32 -luser32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32

.cpp.o :
	$(WINE_CXX) -c $(CXXFLAGS_PRE) $(CXXFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.c.o :
	$(WINE_CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.rc.o :
	$(WINE_RC) -o $@ $<

# Use MinGW-style flags when MinGW compilers are available
CXXFLAGS_PRE = -DWIN32 -D_WINDOWS -DHAVE_CONFIG_H \
     -I../../.. -I. -I@srcdir@/../.. -I@top_builddir@

CFLAGS_PRE = -DWIN32 -D_WINDOWS -I../.. -I../../.. -DHAVE_CONFIG_H \
-I@srcdir@/../.. -I@top_builddir@

#VPATH = ../..:@srcdir@/../..:@top_builddir@:@srcdir@
vpath %.h @srcdir@/../..
vpath %.h @top_builddir@
vpath %.c @srcdir@
vpath %.c @srcdir@/../..

Controller.exe : main.o resources.o dinput.o linuxtrack.o
	$(WINE_CXX) -o Controller.exe $(LDFLAGS) -m32 -static-libgcc -static-libstdc++ \
           $^ $(MINGW_LIBS) -ldxguid -ldinput -ldinput8

main.o : main.cpp @top_builddir@/config.h

linuxtrack.o : linuxtrack.c linuxtrack.h config.h 

dinput.o : dinput.cpp kbi_interface.h

resources.o : resources.rc


clean-local: clean-local-check
.PHONY: clean-local-check
clean-local-check:
	rm -f *.exe* *.dll* *.sh *.o

distclean-local: distclean-local-check
.PHONY: distclean-local-check
distclean-local-check:
	rm -f *.exe* *.dll* *.sh *.o

EXTRA_DIST = linuxtrack1.ico linuxtrack.ico dinput.cpp kbi_interface.h \
main.cpp resource.h resources.rc

