if WINE_PLUGIN

nodist_pkgdata_DATA = linuxtrack-wine.exe

# Set toolchain based on build method
if WINE_USE_MINGW
  WINE_CC = i686-w64-mingw32-gcc
  WINE_CXX = i686-w64-mingw32-g++
  WINE_RC = i686-w64-mingw32-windres
  WINE_EXT = .exe
  WINE_DLL_EXT = .dll
else
  WINE_CC = winegcc
  WINE_CXX = wineg++
  WINE_RC = wrc
  WINE_EXT = .exe.so
  WINE_DLL_EXT = .dll.so
endif

if WINE64_USE_MINGW
  WINE64_CC = x86_64-w64-mingw32-gcc
  WINE64_CXX = x86_64-w64-mingw32-g++
  WINE64_RC = x86_64-w64-mingw32-windres
  WINE64_EXT = .exe
  WINE64_DLL_EXT = .dll
else
  WINE64_CC = winegcc
  WINE64_CXX = wineg++
  WINE64_RC = wrc
  WINE64_EXT = .exe.so
  WINE64_DLL_EXT = .dll.so
endif

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

