# QMake-based GUI integration (ltr_gui.pro generated from ltr_gui.pro.in by configure)
# Keep Automake conservative: delegate GUI build to qmake without trying to compile C sources here.

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

# Core libraries installed under $(libdir)/linuxtrack
pkglib_LTLIBRARIES = \
  liblinuxtrack.la \
  libltr.la

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
all-local: $(GUI_MAKEFILE) $(MICKEY_MAKEFILE) $(WII_MAKEFILE)
	$(MAKE) -C $(GUI_DIR)
	$(MAKE) -C $(MICKEY_DIR)
	$(MAKE) -C $(WII_DIR)

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
	$(MAKE) -C $(WII_DIR) INSTALL_ROOT="$(DESTDIR)" install

# Uninstall qmake-built apps
uninstall-local:
	- $(MAKE) -C $(GUI_DIR) uninstall || true
	- $(MAKE) -C $(MICKEY_DIR) uninstall || true
	- $(MAKE) -C $(WII_DIR) uninstall || true

clean-local:
	- test -f $(GUI_MAKEFILE) && $(MAKE) -C $(GUI_DIR) clean || true
	- test -f $(MICKEY_MAKEFILE) && $(MAKE) -C $(MICKEY_DIR) clean || true
	- test -f $(WII_MAKEFILE) && $(MAKE) -C $(WII_DIR) clean || true

distclean-local: clean-local
	- rm -f $(GUI_MAKEFILE) $(MICKEY_MAKEFILE) $(WII_MAKEFILE)