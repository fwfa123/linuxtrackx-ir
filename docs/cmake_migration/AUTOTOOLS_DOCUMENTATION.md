# Autotools Build System Documentation

This document catalogs all Autotools components in the linuxtrackx-ir project to guide the CMake migration.

## Main Configuration File: configure.ac

**Location**: `/configure.ac`  
**Lines**: 660  
**Purpose**: Main Autotools configuration script

### Key Features:

1. **Project Definition**
   - Name: LinuxTrack
   - Version: 0.99.27
   - Default prefix: /opt

2. **Compiler Configuration**
   - C and C++ compilers
   - Objective-C support (for macOS)
   - Debug vs Release flags
   - Security flags: PIE, stack protector, fortify source

3. **Tool Detection**
   - bison (required)
   - flex (required)
   - qmake (Qt5)
   - winegcc/wineg++ (Wine bridge)
   - makensis (NSIS installer)

4. **Library Detection**
   - Required: libm, libpthread, libmxml, libz, libusb-1.0
   - Optional: libcwiid, liblo, libv4l2, opencv
   - Multi-distribution path detection (Arch, Debian, Fedora)

5. **Feature Flags**
   - `--enable-webcam` (default: no)
   - `--enable-ltr-32lib-on-x64` (default: yes)
   - `--disable-pie` (default: enabled)
   - `--disable-ldconfig` (default: enabled)
   - `--with-xplane-sdk` (optional path)
   - `--with-wine-libs` (optional path)
   - `--with-wine64-libs` (optional path)
   - `--with-lib32-dir` (multiarch directory)

6. **Conditional Builds**
   - WEBCAM_SUPPORT
   - WIIMOTE_SUPPORT (CWIID)
   - TRACKIR_SUPPORT
   - FACE_TRACKER_SUPPORT
   - XPLANE_PLUGIN
   - MICKEY
   - WINE_PLUGIN
   - WINE64
   - OSC_SUPPORT
   - DARWIN (macOS)
   - X64 (64-bit host)
   - LTR32ON64 (32-bit lib on 64-bit host)

7. **Generated Files**
   - config.h (from config.h.in)
   - src/pathconfig.h (from pathconfig.h.in)
   - src/local_config.h (from local_config.h.in)
   - src/qt_gui/ltr_gui.pro (from ltr_gui.pro.in)
   - src/wii_server/wii_server.pro (from wii_server.pro.in)
   - src/mickey/mickey.pro (from mickey.pro.in)
   - src/linuxtrack.pc (pkg-config file)
   - Multiple Makefile.in files

## Makefile.am Files

### Root Makefile.am
**Location**: `/Makefile.am`  
**Purpose**: Top-level build coordination

- Defines SUBDIRS based on conditional features
- Installs documentation files
- Contains verify-32bit target

**Subdirectories**:
- src (always)
- src/mac (always)
- doc (always)
- src/osc_server_mac (always)
- src/wine_bridge/* (if WINE_PLUGIN)
- src/helper (if LTR32ON64 and X64)

### src/Makefile.am
**Location**: `/src/Makefile.am`  
**Purpose**: Core libraries and binaries

**Libraries Built**:
- liblinuxtrack.la (public API shim)
- liblinuxtrack32.la (32-bit version, conditional)
- libltr.la (main engine)
- libtir.la (TrackIR driver)
- libjoy.la (joystick helper)
- libltusb1.la (USB interface)
- libwc.la (webcam driver, conditional)
- xlinuxtrack9.la (X-Plane plugin, conditional)
- xlinuxtrack9_32.la (32-bit X-Plane plugin, conditional)

**Binaries Built**:
- ltr_server1 (daemon)
- ltr_pipe (CLI)
- ltr_extractor (game data extractor)
- ltr_recenter (helper)

**Qt5 Integration**:
- Calls qmake for ltr_gui.pro, mickey.pro, wii_server.pro
- Builds via make in subdirectories
- Installs via make install in subdirectories

**Special Features**:
- Flex/Bison integration for preferences parser
- 32-bit library installation to multiarch directories
- ldconfig integration hooks
- Desktop file installation

### src/wine_bridge/Makefile.am
**Location**: `/src/wine_bridge/Makefile.am`  
**Purpose**: Wine bridge coordination

- Builds NSIS installer (linuxtrack-wine.exe)
- Coordinates subdirectories:
  - client (NPClient DLLs)
  - controller
  - tester
  - ft_client (FreeTrack)
  - ft_tester
  - views

### src/wine_bridge/client/Makefile.am
**Location**: `/src/wine_bridge/client/Makefile.am`  
**Purpose**: TrackIR API compatibility DLLs

**Targets**:
- NPClient.dll.so (32-bit)
- NPClient64.dll.so (64-bit, conditional)
- check_data.exe.so

**Special Features**:
- Uses winegcc/wineg++ compilers
- Custom suffix rules for .exe.so and .dll.so
- Resource compilation with wrc
- 32-bit and 64-bit builds

### src/wine_bridge/controller/Makefile.am
**Purpose**: Controller component (Wine bridge)

### src/wine_bridge/tester/Makefile.am
**Purpose**: Tester component (Wine bridge)

### src/wine_bridge/ft_client/Makefile.am
**Purpose**: FreeTrack client DLL

### src/wine_bridge/ft_tester/Makefile.am
**Purpose**: FreeTrack tester

### src/wine_bridge/views/Makefile.am
**Purpose**: Views component (Wine bridge)

### src/mac/Makefile.am
**Purpose**: macOS-specific components

### src/osc_server_mac/Makefile.am
**Purpose**: macOS OSC server

### src/helper/Makefile.am
**Purpose**: Helper utilities (currently commented out)

### src/tests/Makefile.am
**Purpose**: Test suite

### doc/Makefile.am
**Purpose**: Documentation installation

## Qt5 Project Files (.pro.in)

### src/qt_gui/ltr_gui.pro.in
**Purpose**: Main GUI application

**Qt Modules**: opengl, network, help, concurrent, widgets, sql  
**Features**: Precompiled headers, resources, forms, many source files

### src/mickey/mickey.pro.in
**Purpose**: Mickey calibration tool

**Qt Modules**: widgets, x11extras, help  
**Features**: Calibration UI, uinput integration

### src/wii_server/wii_server.pro.in
**Purpose**: Wii server application

## Key Build Patterns

### Library Build Pattern
```makefile
libname_la_SOURCES = file1.c file2.c ...
libname_la_LIBADD = -lm -lpthread
libname_la_LDFLAGS = -export-symbols "symbols.sym"
```

### Binary Build Pattern
```makefile
binary_SOURCES = main.c utils.c
binary_LDADD = liblinuxtrack.la libltr.la -ldl
```

### Wine Build Pattern
```makefile
CC = winegcc
CXX = wineg++
target.dll.so : source.o
	winegcc $(WINE_LIBS) $(LDFLAGS) -m32 -o $@ -shared $^
```

### Qt5 Build Pattern
```makefile
$(GUI_MAKEFILE): $(GUI_PRO)
	cd $(GUI_DIR) && $(QMAKE) -spec linux-g++ "LIBDIR=$(libdir)/linuxtrack" ltr_gui.pro
```

## Installation Patterns

### Standard Installation
- Binaries: $(bindir)
- Libraries: $(libdir)/linuxtrack
- Data: $(datadir)/linuxtrack
- Desktop files: /usr/share/applications

### 32-bit Multiarch Installation
- Libraries: $(prefix)/lib/$(LIB32DIR)/linuxtrack
- Only when LIB32DIR is set and != "lib32"

### ldconfig Integration
- Creates /etc/ld.so.conf.d/linuxtrack.conf
- Runs ldconfig after installation
- Can be disabled with --disable-ldconfig

## Dependencies Mapping

| Autotools Check | CMake Equivalent |
|----------------|------------------|
| AC_CHECK_LIB | find_library or pkg_check_modules |
| AC_CHECK_HEADER | check_include_file |
| AC_CHECK_PROG | find_program |
| PKG_CHECK_MODULES | pkg_check_modules |
| AC_PATH_PROGS | find_program with PATHS |

## Conditional Build Logic

Autotools uses AM_CONDITIONAL for conditional builds:
- XPLANE_PLUGIN
- CWIID
- OSC_SUPPORT
- WEBCAM_SUPPORT
- DARWIN
- TRACKIR_SUPPORT
- X64
- LTR32ON64
- FACE_TRACKER
- MICKEY
- WINE_PLUGIN
- WINE64
- LDCONFIG_SUPPORT

CMake equivalent: `if()` blocks with options or variables.

## Generated Configuration

### config.h
Defines feature flags:
- WEBCAM_SUPPORT
- TRACKIR_SUPPORT
- WIIMOTE_SUPPORT
- FACE_TRACKER_SUPPORT
- XPLANE_PLUGIN_SUPPORT
- MICKEY_SUPPORT
- WINE_PLUGIN_SUPPORT
- OSC_SUPPORT
- PIE_SUPPORT
- V4L2, LIBV4L2
- DARWIN
- HAVE_NEW_LIBLO

### src/pathconfig.h
Defines library path: `#define LIB_PATH "/opt/lib/linuxtrack/"`

### src/local_config.h
Local configuration settings

## Build Scripts Using Autotools

1. `scripts/build_arch_linux.sh` - Uses autoreconf, ./configure, make
2. `scripts/install/install_arch_prebuilt.sh` - Uses autoreconf, ./configure
3. `scripts/appimage/v2/prepare.sh` - Uses autoreconf, ./configure
4. `scripts/install/modify_build_for_prebuilt.sh` - Modifies configure.ac

## Migration Notes

- Wine bridge requires custom compiler functions (winegcc)
- Qt5 projects need migration from qmake to CMake Qt5
- 32-bit library installation is complex (multiarch paths)
- ldconfig integration needs custom install hooks
- Flex/Bison integration for preferences parser
- Multiple conditional builds based on dependencies
- Desktop file installation
- Resource file compilation (.qrc)

