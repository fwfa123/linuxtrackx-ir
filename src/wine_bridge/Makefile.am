# Wine Bridge Build Configuration
# 
# This build system uses MinGW cross-compilation instead of winegcc because:
# - MinGW produces native Windows PE binaries (.exe/.dll) that work on both Windows and Wine
# - winegcc produces Linux ELF binaries with Wine wrappers (.exe.so/.dll.so) that only work with Wine
# - MinGW is the industry standard for cross-compilation and is more future-proof
# - MinGW has fewer dependencies and simpler build process
#
# The resulting binaries are genuine Windows executables that can be distributed
# to Windows users and work perfectly with Wine on Linux.

if WINE_PLUGIN

nodist_pkgdata_DATA = linuxtrack-wine.exe

# Use MinGW compilers directly when available, otherwise use winegcc
# These variables are set by configure.ac based on availability
WINE_CC = $(MINGW32_CC)
WINE_CXX = $(MINGW32_CXX)
WINE_RC = $(MINGW32_WINDRES)
WINE_EXT = .exe
WINE_DLL_EXT = .dll

WINE64_CC = $(MINGW64_CC)
WINE64_CXX = $(MINGW64_CXX)
WINE64_RC = $(MINGW64_WINDRES)
WINE64_EXT = .exe
WINE64_DLL_EXT = .dll

if DARWIN
  MYLDFLAGS = -Wl,-no_arch_warnings
  linuxtrack-wine.exe : ltr_wine.nsi tester/Tester$(WINE_EXT) controller/Controller$(WINE_EXT) \
             client/NPClient$(WINE_DLL_EXT) ft_client/FreeTrackClient$(WINE_DLL_EXT) ft_tester/ftc$(WINE_EXT) \
             client/check_data$(WINE_EXT) views/TrackIR$(WINE_EXT)
	install_name_tool -change $(WINE_PREFIX)/lib/libwine.1.dylib libwine.1.dylib tester/Tester.exe.so
	install_name_tool -change $(WINE_PREFIX)/lib/libwine.1.dylib libwine.1.dylib controller/Controller.exe.so 
	install_name_tool -change $(WINE_PREFIX)/lib/libwine.1.dylib libwine.1.dylib client/NPClient.dll.so
	install_name_tool -change $(WINE_PREFIX)/lib/libwine.1.dylib libwine.1.dylib ft_client/FreeTrackClient.dll.so
	install_name_tool -change $(WINE_PREFIX)/lib/libwine.1.dylib libwine.1.dylib ft_tester/ftc.exe.so
	install_name_tool -change $(WINE_PREFIX)/lib/libwine.1.dylib libwine.1.dylib client/check_data.exe.so
	install_name_tool -change $(WINE_PREFIX)/lib/libwine.1.dylib libwine.1.dylib views/TrackIR.exe.so
	makensis $<


else
  MYLDFLAGS = -Wl,--no-warn-search-mismatch

if WINE64
  linuxtrack-wine.exe : ltr_wine64.nsi tester/Tester$(WINE_EXT) tester/Tester64$(WINE64_EXT) \
                        controller/Controller$(WINE_EXT) client/NPClient$(WINE_DLL_EXT) client/NPClient64$(WINE64_DLL_EXT) \
                        ft_client/FreeTrackClient$(WINE_DLL_EXT) ft_tester/ftc$(WINE_EXT) client/check_data$(WINE_EXT) \
			views/TrackIR$(WINE_EXT)
	makensis $<
else
  linuxtrack-wine.exe : ltr_wine.nsi tester/Tester$(WINE_EXT) controller/Controller$(WINE_EXT) client/NPClient$(WINE_DLL_EXT) \
                        ft_client/FreeTrackClient$(WINE_DLL_EXT) ft_tester/ftc$(WINE_EXT) client/check_data$(WINE_EXT) \
			views/TrackIR$(WINE_EXT)
	makensis $<
endif #WINE64

endif #DARWIN


clean-local: clean-local-check
.PHONY: clean-local-check
clean-local-check:
	rm -f *.exe

distclean-local: distclean-local-check
.PHONY: distclean-local-check
distclean-local-check:
	rm -f *.exe

endif

dist_pkgdata_DATA = README.wine
EXTRA_DIST = linuxtrack.ico linuxtrack1.ico

