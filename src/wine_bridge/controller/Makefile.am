noinst_SCRIPTS = 
if WINE_PLUGIN
if WINE_USE_MINGW
  noinst_SCRIPTS += Controller.exe
else
  noinst_SCRIPTS += Controller.exe.so
endif
endif #WINE_PLUGIN

if DARWIN
  LDFLAGS += -Wl,-no_arch_warnings
else
  LDFLAGS += -Wl,--no-warn-search-mismatch
endif

if WINE_USE_MINGW
  WINE_CC = i686-w64-mingw32-gcc
  WINE_CXX = i686-w64-mingw32-g++
  WINE_RC = i686-w64-mingw32-windres
  MINGW_LIBS = -lkernel32 -luser32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
else
  WINE_CC = winegcc
  WINE_CXX = wineg++
  WINE_RC = wrc
endif

.cpp.o :
	$(WINE_CXX) -c $(CXXFLAGS_PRE) $(CXXFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.c.o :
	$(WINE_CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.rc.o :
	$(WINE_RC) -o $@ $<

if WINE_USE_MINGW
  CXXFLAGS_PRE = -DWIN32 -D_WINDOWS -DHAVE_CONFIG_H \
	   -I../../.. -I. -I@srcdir@/../.. -I@top_builddir@

  CFLAGS_PRE = -DWIN32 -D_WINDOWS -I../.. -I../../.. -DHAVE_CONFIG_H \
	-I@srcdir@/../.. -I@top_builddir@
else
  CXXFLAGS_PRE = -isystem/usr/include/wine/windows -DHAVE_CONFIG_H \
	   -I../../.. -I. -I@srcdir@/../.. -I@top_builddir@

  CFLAGS_PRE = -I../.. -I../../.. -DHAVE_CONFIG_H \
	-I@srcdir@/../.. -I@top_builddir@
endif

#VPATH = ../..:@srcdir@/../..:@top_builddir@:@srcdir@
vpath %.h @srcdir@/../..
vpath %.h @top_builddir@
vpath %.c @srcdir@
vpath %.c @srcdir@/../..

if WINE_USE_MINGW
Controller.exe : main.o resources.o dinput.o linuxtrack.o
	$(WINE_CXX) -o Controller.exe $(LDFLAGS) -m32 -static-libgcc -static-libstdc++ \
           $^ $(MINGW_LIBS) -ldxguid -ldinput -ldinput8
else
Controller.exe.so : main.o resources.o dinput.o linuxtrack.o
	$(WINE_CXX) -o Controller.exe.so   $(LDFLAGS) $(WINE_LIBS) -m32 \
           $^ -lshell32 -ldxguid -ldinput -ldinput8 -luuid -ldl
endif

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

