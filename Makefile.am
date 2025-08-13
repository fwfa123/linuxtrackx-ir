SUBDIRS = src src/mac doc src/osc_server_mac #src/tests 
#if ! DARWIN
#SUBDIRS += src/mickey
#if X64
#if LTR32ON64
#  SUBDIRS += src/helper
#endif
#endif
#endif

if WINE_PLUGIN
  SUBDIRS +=src/wine_bridge/client src/wine_bridge/controller src/wine_bridge/tester \
  src/wine_bridge/ft_client src/wine_bridge/ft_tester src/wine_bridge/views src/wine_bridge
endif


dist_pkgdata_DATA = README.md Legacy_README.devel Legacy_README.xplane docs/progress/TODO_FUTURE_DEVELOPMENT.md
EXTRA_DIST = README.md Readme.rtf deploy mac_build pkgconf_extract mac_m4/fake_pkg_conf.m4 LICENSE.md
AM_DISTCHECK_CONFIGURE_FLAGS = --with-xplane-sdk=$(HOME)/devel/SDK/CHeaders \
  --with-wine-libs="-L/usr/lib/i386-linux-gnu -L/usr/lib/i386-linux-gnu/wine" \
  --enable-ltr-32lib-on-x64

# Convenience target to verify 32-bit runtime presence after install
.PHONY: verify-32bit
verify-32bit:
	@echo "Verifying 32-bit linuxtrack runtime..."; \
	if [ -f /usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0 ]; then \
	  file /usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0; \
	  echo "OK: 32-bit liblinuxtrack present"; \
	else \
	  echo "WARNING: 32-bit liblinuxtrack not found (Tester.exe may fail)."; \
	  echo "On Debian/Ubuntu run: ./configure --prefix=/usr --with-lib32-dir=i386-linux-gnu --enable-ltr-32lib-on-x64 && make && sudo make install"; \
	fi
