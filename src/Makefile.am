# QMake-based GUI integration (ltr_gui.pro generated from ltr_gui.pro.in by configure)
# Keep Automake conservative: delegate GUI build to qmake without trying to compile C sources here.
ACLOCAL_AMFLAGS = -I m4
# Install-time library path define used by runtime to locate drivers
AM_CPPFLAGS = -DLIB_PATH=\"$(pkglibdir)/\"

# Flex/Bison settings for preferences parser
AM_YFLAGS = -d --name-prefix ltr_int_parser_
AM_LFLAGS = -i
BUILT_SOURCES = pref_flex.cpp pref_bison.cpp pref_bison.hpp

# Binaries to install
bin_PROGRAMS = \
  ltr_server1 \
  ltr_pipe \
  ltr_extractor \
  ltr_recenter

# Core and driver libraries installed under $(libdir)/linuxtrack
pkglib_LTLIBRARIES = \
  liblinuxtrack.la \
  libltr.la \
  libtir.la \
  libjoy.la \
  libltusb1.la

# Conditionally add the webcam driver
if WEBCAM_SUPPORT
pkglib_LTLIBRARIES += libwc.la
endif

# liblinuxtrack: small public shim used by client utilities
liblinuxtrack_la_SOURCES = \
  ltlib.c ltlib.h \
  utils.c utils.h \
  ipc_utils.c ipc_utils.h \
  linuxtrack.h
liblinuxtrack_la_LIBADD =
liblinuxtrack_la_LDFLAGS = -export-symbols "${srcdir}/liblt.sym"

# libltr: main engine
libltr_la_SOURCES = \
  cal.c cal.h \
  list.c list.h \
  dyn_load.c dyn_load.h \
  math_utils.c math_utils.h \
  pose.c pose.h \
  pref.cpp pref.hpp pref.h pref_bison.cpp pref_flex.cpp pref_global.c pref_global.h \
  utils.c utils.h \
  image_process.c image_process.h \
  tracking.c tracking.h \
  ltlib_int.c ltlib_int.h \
  spline.c spline.h \
  axis.c axis.h \
  wii_driver_prefs.c wii_driver_prefs.h \
  tir_driver_prefs.c tir_driver_prefs.h \
  wc_driver_prefs.c wc_driver_prefs.h \
  joy_driver_prefs.c joy_driver_prefs.h \
  ipc_utils.c ipc_utils.h \
  com_proc.c com_proc.h \
  wii_com.c wii_com.h \
  ps3_prefs.c ps3_prefs.h
libltr_la_LIBADD = -lm -lpthread -ldl
libltr_la_LDFLAGS = -export-symbols-regex '^ltr_int'

# libwc: V4L2 webcam/face-tracking driver
if WEBCAM_SUPPORT
libwc_la_SOURCES = \
  webcam_driver.c \
  runloop.c
libwc_la_LIBADD = libltr.la -lv4l2
libwc_la_LDFLAGS = -export-symbols "${srcdir}/webcam_driver.sym"
else
libwc_la_SOURCES =
libwc_la_LIBADD =
libwc_la_LDFLAGS =
endif

# libtir: TrackIR USB driver
libtir_la_SOURCES = \
  tir_hw.c \
  tir_img.c \
  tir_driver.c \
  runloop.c
libtir_la_LIBADD = libltr.la -lz
libtir_la_LDFLAGS = -export-symbols "${srcdir}/tir.sym"

# libjoy: Joystick helper
libjoy_la_SOURCES = \
  runloop.c \
  joy.c
libjoy_la_LIBADD =
libjoy_la_LDFLAGS = -export-symbols "${srcdir}/joy_driver.sym"

# libltusb1: USB interface helper used by TrackIR driver
libltusb1_la_SOURCES = \
  libusb_ifc.c
libltusb1_la_LIBADD = libltr.la -lusb-1.0
libltusb1_la_LDFLAGS =

# ltr_server1 daemon (link against libltr)
ltr_server1_SOURCES = \
  ltr_srv_comm.c ltr_srv_comm.h \
  ltr_srv_slave.c ltr_srv_slave.h \
  ltr_srv_master.cpp ltr_srv_master.h \
  ltr_server1.c
ltr_server1_LDADD = libltr.la -lpthread

# ltr_pipe CLI (link against liblinuxtrack + libltr)
ltr_pipe_SOURCES = \
  ltr_pipe.c \
  linuxtrack.c linuxtrack.h
ltr_pipe_LDADD = liblinuxtrack.la libltr.la -ldl

# ltr_extractor CLI (uses internal digest) — link utils via libs
ltr_extractor_SOURCES = \
  hashing.c \
  game_data.c game_data.h \
  extract.c extract.h \
  digest.c digest.h
ltr_extractor_LDADD = liblinuxtrack.la libltr.la -lmxml

# ltr_recenter helper — link to libraries for comms
ltr_recenter_SOURCES = \
  ltr_recenter.c \
  ltr_srv_comm.c ltr_srv_comm.h
ltr_recenter_LDADD = liblinuxtrack.la libltr.la -ldl

# Existing qmake-based GUI
noinst_PROGRAMS =

GUI_DIR = qt_gui
GUI_PRO = $(GUI_DIR)/ltr_gui.pro
GUI_MAKEFILE = $(GUI_DIR)/Makefile
QMAKE = $(QT5_QMAKE)

# Mickey (Qt) via qmake
MICKEY_DIR = mickey
MICKEY_PRO = $(MICKEY_DIR)/mickey.pro
MICKEY_MAKEFILE = $(MICKEY_DIR)/Makefile

# Wii server (Qt) via qmake
WII_DIR = wii_server
WII_PRO = $(WII_DIR)/wii_server.pro
WII_MAKEFILE = $(WII_DIR)/Makefile

# Generate qmake Makefiles then build as part of 'all'
if CWIID
all-local: $(GUI_MAKEFILE) $(MICKEY_MAKEFILE) $(WII_MAKEFILE)
	$(MAKE) -C $(GUI_DIR)
	$(MAKE) -C $(MICKEY_DIR)
	$(MAKE) -C $(WII_DIR)
else
all-local: $(GUI_MAKEFILE) $(MICKEY_MAKEFILE)
	$(MAKE) -C $(GUI_DIR)
	$(MAKE) -C $(MICKEY_DIR)
endif

$(GUI_MAKEFILE): $(GUI_PRO)
	cd $(GUI_DIR) && $(QMAKE) -spec linux-g++ "LIBDIR=$(libdir)/linuxtrack" ltr_gui.pro

$(MICKEY_MAKEFILE): $(MICKEY_PRO)
	cd $(MICKEY_DIR) && $(QMAKE) -spec linux-g++ "LIBDIR=$(libdir)/linuxtrack" mickey.pro

$(WII_MAKEFILE): $(WII_PRO)
	cd $(WII_DIR) && $(QMAKE) -spec linux-g++ "LIBDIR=$(libdir)/linuxtrack" wii_server.pro

# Install qmake-built apps alongside CLI tools; support DESTDIR
install-exec-local: all-local
	$(MAKE) -C $(GUI_DIR) INSTALL_ROOT="$(DESTDIR)" install
	$(MAKE) -C $(MICKEY_DIR) INSTALL_ROOT="$(DESTDIR)" install
if CWIID
	$(MAKE) -C $(WII_DIR) INSTALL_ROOT="$(DESTDIR)" install
endif

# Data files installed under $(datadir)/linuxtrack for runtime use
pkgdatadir = $(datadir)/linuxtrack
dist_pkgdata_DATA = \
  linuxtrack1.conf \
  linuxtrack.c \
  linuxtrack_hello_world.c \
  linuxtrack_hello_world_adv.c \
  99-TIR.rules \
  99-Mickey.rules \
  haarcascade_frontalface_alt2.xml

# Desktop launcher entries (XDG .desktop files)
# Install to the system applications directory so environments can discover them
desktopdir = /usr/share/applications
dist_desktop_DATA = \
  linuxtrack.desktop \
  mickey.desktop
if CWIID
dist_desktop_DATA += linuxtrack-wii.desktop
endif

# Refresh desktop database after installation (best-effort)
install-data-hook:
	- update-desktop-database $(desktopdir) 2>/dev/null || true

# Uninstall qmake-built apps
uninstall-local:
	- $(MAKE) -C $(GUI_DIR) uninstall || true
	- $(MAKE) -C $(MICKEY_DIR) uninstall || true
if CWIID
	- $(MAKE) -C $(WII_DIR) uninstall || true
endif

clean-local:
	- test -f $(GUI_MAKEFILE) && $(MAKE) -C $(GUI_DIR) clean || true
	- test -f $(MICKEY_MAKEFILE) && $(MAKE) -C $(MICKEY_DIR) clean || true
if CWIID
	- test -f $(WII_MAKEFILE) && $(MAKE) -C $(WII_DIR) clean || true
endif

distclean-local: clean-local
	- rm -f $(GUI_MAKEFILE) $(MICKEY_MAKEFILE)
if CWIID
	- rm -f $(WII_MAKEFILE)
endif

# Configure library path for dynamic loading
if LDCONFIG_SUPPORT
install-exec-hook:
	@echo "Configuring LinuxTrack library path..."
	@mkdir -p $(DESTDIR)/etc/ld.so.conf.d
	@if [ ! -f $(DESTDIR)/etc/ld.so.conf.d/linuxtrack.conf ]; then \
	  echo "$(pkglibdir)" > $(DESTDIR)/etc/ld.so.conf.d/linuxtrack.conf; \
	  echo "Created $(DESTDIR)/etc/ld.so.conf.d/linuxtrack.conf"; \
	fi
	@if [ -z "$(DESTDIR)" ]; then \
	  echo "Running ldconfig to update library cache..."; \
	  ldconfig || echo "Warning: ldconfig failed - you may need to run 'sudo ldconfig' manually"; \
	  sleep 1; \
	  if ldconfig -p | grep -q "liblinuxtrack.so.0"; then \
	    echo "✅ Core libraries installed successfully"; \
	  else \
	    echo "❌ Core library installation verification failed"; \
	    exit 1; \
	  fi; \
	  if ldconfig -p | grep -q "libwc.so.0"; then \
	    echo "✅ Webcam libraries installed successfully"; \
	  else \
	    echo "ℹ️  Webcam libraries not installed (minimal build)"; \
	  fi; \
	  echo "LinuxTrack installation completed successfully"; \
	else \
	  echo "ℹ️  Staging installation - ldconfig will be run by package manager"; \
	  echo "✅ Library configuration files installed to staging directory"; \
	fi

# Clean up library configuration on uninstall
uninstall-hook:
	@if [ -f $(DESTDIR)/etc/ld.so.conf.d/linuxtrack.conf ]; then \
	  rm -f $(DESTDIR)/etc/ld.so.conf.d/linuxtrack.conf; \
	  echo "Removed $(DESTDIR)/etc/ld.so.conf.d/linuxtrack.conf"; \
	  if [ -z "$(DESTDIR)" ]; then \
	    ldconfig; \
	  fi; \
	fi
else
install-exec-hook:
	@echo "ℹ️  ldconfig configuration disabled - package manager will handle library paths"

uninstall-hook:
	@echo "ℹ️  ldconfig configuration disabled - no cleanup needed"
endif

if DARWIN
  SLEOPARD_LDFLAG = -headerpad_max_install_names
  DARWINFLAGS = -undefined dynamic_lookup
  SYSFLAGS = -DIBM=0 -DAPL=1 -DLIN=0
  XLINUXTRACK_CFLAGS = -arch i386 -arch x86_64
  XLINUXTRACK_LDFLAGS = -Wl,-no_arch_warnings -arch i386 -arch x86_64
else
  LINUXFLAGS = '-DLIB_PATH="$(pkglibdir)/"'
  LINUXLDLFAGS = '-Wl,-rpath,$(pkglibdir)'
  SYSFLAGS = -DIBM=0 -DAPL=0 -DLIN=1
  XLINUXTRACK_CFLAGS =
  XLINUXTRACK_LDFLAGS =  -L/lib32 -L/usr/lib32 -Wl,--no-warn-search-mismatch
endif

if XPLANE_PLUGIN
pkglib_LTLIBRARIES += xlinuxtrack9.la
if LTR32ON64
  pkglib_LTLIBRARIES += xlinuxtrack9_32.la
  xlinuxtrack9_32_la_SOURCES = xlinuxtrack9.c linuxtrack.c linuxtrack.h utils.c utils.h
  xlinuxtrack9_32_la_CFLAGS = ${XPL_CPPFLAGS} $(SYSFLAGS) \
    -fPIC -m32 $(DARWINCFLAGS) ${AM_CFLAGS} $(LINUXFLAGS) ${XLINUXTRACK_CFLAGS}
  xlinuxtrack9_32_la_LDFLAGS = -module -shared -fPIC -m32 $(DARWINFLAGS) -dynamiclib \
    -export-symbols "${srcdir}/xlinuxtrack.sym" $(LINUXLDFLAGS) ${SLEOPARD_LDFLAG} ${XLINUXTRACK_LDFLAGS}
  xlinuxtrack9_32_la_LIBADD = -lm -ldl
endif
xlinuxtrack9_la_SOURCES = xlinuxtrack9.c linuxtrack.c linuxtrack.h utils.c utils.h
xlinuxtrack9_la_CFLAGS = ${XPL_CPPFLAGS} $(SYSFLAGS) \
  -fPIC $(DARWINCFLAGS) ${AM_CFLAGS} $(LINUXFLAGS) ${XLINUXTRACK_CFLAGS}
xlinuxtrack9_la_LDFLAGS = -module -shared -fPIC $(DARWINFLAGS) -dynamiclib \
  -export-symbols "${srcdir}/xlinuxtrack.sym" $(LINUXLDFLAGS) ${SLEOPARD_LDFLAG} ${XLINUXTRACK_LDFLAGS}
xlinuxtrack9_la_LIBADD = -lm -ldl
endif
