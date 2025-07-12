noinst_SCRIPTS = 
if WINE_PLUGIN
  noinst_SCRIPTS += Controller.exe.so
endif #WINE_PLUGIN

if DARWIN
  LDFLAGS += -Wl,-no_arch_warnings
else
  LDFLAGS += -Wl,--no-warn-search-mismatch
endif

# Use winegcc compilers
WINE_CC = winegcc
WINE_CXX = wineg++
WINE_RC = wrc

.cpp.o :
	$(WINE_CXX) -c $(CXXFLAGS_PRE) $(CXXFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.c.o :
	$(WINE_CC) -c $(CFLAGS_PRE) $(CFLAGS) $(CPPFLAGS) -m32 -o $@ $<

.rc.o :
	$(WINE_RC) -o $@ $<

CXXFLAGS_PRE = -DHAVE_CONFIG_H \
     -I../../.. -I. -I@srcdir@/../.. -I@top_builddir@

CFLAGS_PRE = -I../.. -I../../.. -DHAVE_CONFIG_H \
-I@srcdir@/../.. -I@top_builddir@

#VPATH = ../..:@srcdir@/../..:@top_builddir@:@srcdir@
vpath %.h @srcdir@/../..
vpath %.h @top_builddir@
vpath %.c @srcdir@
vpath %.c @srcdir@/../..

Controller.exe.so : main.o resources.o dinput.o linuxtrack.o
	winegcc -o Controller.exe.so $(LDFLAGS) -m32 \
           $^ -ldxguid -ldinput -ldinput8 -lstdc++ -lshell32

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

