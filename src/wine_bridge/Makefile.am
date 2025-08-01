# Wine Bridge Build Configuration
# 
# This build system uses winegcc for Wine compatibility
# winegcc produces Linux ELF binaries with Wine wrappers (.exe.so/.dll.so)

if WINE_PLUGIN

nodist_pkgdata_DATA = linuxtrack-wine.exe

# Use winegcc compilers
WINE_CC = winegcc
WINE_CXX = wineg++
WINE_RC = wrc
WINE_EXT = .exe.so
WINE_DLL_EXT = .dll.so

WINE64_CC = winegcc
WINE64_CXX = wineg++
WINE64_RC = wrc
WINE64_EXT = .exe.so
WINE64_DLL_EXT = .dll.so

# Set MYLDFLAGS based on platform
if DARWIN
  MYLDFLAGS = -Wl,-no_arch_warnings
else
  MYLDFLAGS = -Wl,--no-warn-search-mismatch
endif

# Define targets based on platform and WINE64
if DARWIN
  linuxtrack-wine.exe : ltr_wine.nsi tester/Tester$(WINE_EXT) controller/Controller$(WINE_EXT) \
             client/NPClient$(WINE_DLL_EXT) ft_client/FreeTrackClient$(WINE_DLL_EXT) ft_tester/ftc$(WINE_EXT) \
             client/check_data$(WINE_EXT) views/TrackIR$(WINE_EXT)
	makensis $<
else
if WINE64
  # Use 64-bit installer which includes both 32-bit and 64-bit components
  linuxtrack-wine.exe : ltr_wine64.nsi tester/Tester$(WINE_EXT) tester/Tester64$(WINE_EXT) controller/Controller$(WINE_EXT) client/NPClient$(WINE_DLL_EXT) client/NPClient64$(WINE_DLL_EXT) \
                        ft_client/FreeTrackClient$(WINE_DLL_EXT) ft_tester/ftc$(WINE_EXT) client/check_data$(WINE_EXT) \
                        views/TrackIR$(WINE_EXT)
	makensis $<
else
  # Fallback to 32-bit installer only
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

