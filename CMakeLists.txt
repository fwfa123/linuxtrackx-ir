# CMake build system for LinuxTrack X-IR
# Migrated from Autotools

cmake_minimum_required(VERSION 3.16)
project(LinuxTrack VERSION 2.0.0 LANGUAGES C CXX)

# Set default installation prefix to /opt (matching Autotools)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/opt" CACHE PATH "Installation prefix" FORCE)
endif()

# Policy settings
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW) # Let AUTOMOC, AUTOUIC, AUTORCC process .qrc files
endif()
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW) # find_package uses <Package>_ROOT variables
endif()

# Build options (matching configure.ac options)
option(ENABLE_WEBCAM "Enable webcam driver support" OFF)
option(ENABLE_LTR_32LIB_ON_X64 "Build 32-bit linuxtrack library on 64-bit host" ON)
option(ENABLE_PIE "Enable Position Independent Executable (PIE)" ON)
option(ENABLE_LDCONFIG "Enable automatic ldconfig configuration" ON)
option(ENABLE_DEBUG "Enable debug build" OFF)
# Debugging / instrumentation options
option(ENABLE_ASAN "Enable AddressSanitizer for debugging Qt6 crashes" OFF)

# Optional feature options
option(ENABLE_OSC "Enable OSC support" OFF)
option(ENABLE_WIIMOTE "Enable Wiimote support" OFF)
# When ON, never build wii_server even if pkg-config finds libcwiid (standard AppImage / slim bundles).
option(DISABLE_WIIMOTE "Force-disable Wiimote even if libcwiid is found" OFF)
option(ENABLE_XPLANE "Enable X-Plane plugin" OFF)
option(ENABLE_FACE_TRACKER "Enable face tracker (requires OpenCV and webcam support)" OFF)

# Path options
set(XPLANE_SDK_PATH "/opt/xplane-sdk/CHeaders" CACHE PATH "Path to X-Plane SDK")
set(LIB32DIR "lib32" CACHE STRING "32-bit library directory (usually needed only on amd64 arch)")

# Set C/C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)  # Qt6.2+ requires C++17 or higher

# Compiler flags
if(ENABLE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -Wextra -Wformat -Wformat-security")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wextra -Wformat -Wformat-security")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# AddressSanitizer (for tracking down Qt6 layout crashes)
if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer: ENABLED")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
else()
    message(STATUS "AddressSanitizer: disabled (ENABLE_ASAN=OFF)")
endif()

# Security flags (matching configure.ac)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=1")

# PIE support
if(ENABLE_PIE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")
    message(STATUS "PIE enabled for native Linux builds (security feature)")
else()
    message(STATUS "PIE disabled by user request")
endif()

# Platform detection
if(APPLE)
    set(DARWIN TRUE)
    add_definitions(-DDARWIN)
    set(SYSFLAGS "-DIBM=0 -DAPL=1 -DLIN=0")
else()
    set(DARWIN FALSE)
    set(SYSFLAGS "-DIBM=0 -DAPL=0 -DLIN=1")
endif()

# Architecture detection
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(X64 TRUE)
    set(WITH_64_BIT TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i[3456]86")
    set(X64 FALSE)
    set(WITH_64_BIT FALSE)
else()
    message(FATAL_ERROR "Unsupported CPU type: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Required tools
find_program(BISON_EXECUTABLE bison REQUIRED)
find_program(FLEX_EXECUTABLE flex REQUIRED)

# Required libraries
find_library(MATH_LIBRARY m REQUIRED)
find_library(PTHREAD_LIBRARY pthread REQUIRED)

# pkg-config for library detection
find_package(PkgConfig REQUIRED)

# CMake check modules for library and header detection
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)

# Required libraries via pkg-config
pkg_check_modules(MXML REQUIRED mxml)
pkg_check_modules(ZLIB REQUIRED zlib)
pkg_check_modules(USB REQUIRED libusb-1.0)

# Optional libraries
pkg_check_modules(CWIID cwiid)
if(DISABLE_WIIMOTE)
    set(WIIMOTE_SUPPORT FALSE)
    message(STATUS "Wiimote support: disabled (DISABLE_WIIMOTE=ON)")
    if(ENABLE_WIIMOTE)
        message(WARNING "ENABLE_WIIMOTE is ON but DISABLE_WIIMOTE=ON — Wiimote remains disabled")
    endif()
elseif(CWIID_FOUND)
    # Auto-enable if dependency found (matching Autotools behavior)
    set(WIIMOTE_SUPPORT TRUE)
    message(STATUS "Wiimote support: enabled (libcwiid found)")
else()
    set(WIIMOTE_SUPPORT FALSE)
    if(ENABLE_WIIMOTE)
        message(WARNING "ENABLE_WIIMOTE requested but libcwiid not found")
    endif()
    message(STATUS "Wiimote support: disabled (libcwiid not found)")
endif()

pkg_check_modules(LIBLO liblo)
if(LIBLO_FOUND)
    # Auto-enable if dependency found (matching Autotools behavior)
    set(OSC_SUPPORT TRUE)
    message(STATUS "OSC support: enabled (liblo found)")
    # Check for new liblo version
    check_library_exists(lo lo_bundle_free_recursive "" HAVE_NEW_LIBLO)
    if(HAVE_NEW_LIBLO)
        set(HAVE_NEW_LIBLO_DEFINE "#define HAVE_NEW_LIBLO 1")
    else()
        set(HAVE_NEW_LIBLO_DEFINE "/* #undef HAVE_NEW_LIBLO */")
    endif()
else()
    set(OSC_SUPPORT FALSE)
    set(HAVE_NEW_LIBLO_DEFINE "/* #undef HAVE_NEW_LIBLO */")
    if(ENABLE_OSC)
        message(WARNING "ENABLE_OSC requested but liblo not found")
    endif()
    message(STATUS "OSC support: disabled (liblo not found)")
endif()

# V4L2 detection
check_include_file(linux/videodev2.h HAVE_V4L2)
if(HAVE_V4L2)
    set(V4L2_DEFINE "#define V4L2")
    set(WITH_V4L2 TRUE)
    message(STATUS "V4L2 support: found")
else()
    set(V4L2_DEFINE "/* #undef V4L2 */")
    set(WITH_V4L2 FALSE)
    message(STATUS "V4L2 support: not found")
endif()

check_include_file(libv4l2.h HAVE_LIBV4L2)
if(HAVE_LIBV4L2)
    # Find the libv4l2 library
    find_library(LIBV4L2_LIBRARY v4l2)
    if(LIBV4L2_LIBRARY)
        set(LIBV4L2_DEFINE "#define LIBV4L2")
        set(WITH_LIBV4L2 TRUE)
        message(STATUS "libv4l2 support: found (header and library)")
    else()
        set(LIBV4L2_DEFINE "/* #undef LIBV4L2 */")
        set(WITH_LIBV4L2 FALSE)
        message(STATUS "libv4l2 support: header found but library not found")
    endif()
else()
    set(LIBV4L2_DEFINE "/* #undef LIBV4L2 */")
    set(WITH_LIBV4L2 FALSE)
    set(LIBV4L2_LIBRARY "")
    message(STATUS "libv4l2 support: not found")
endif()

# Webcam support
if(ENABLE_WEBCAM AND (WITH_LIBV4L2 OR DARWIN))
    set(WEBCAM_SUPPORT TRUE)
    message(STATUS "Webcam support: enabled")
else()
    set(WEBCAM_SUPPORT FALSE)
    message(STATUS "Webcam support: disabled")
endif()

# OpenCV (for face tracker) — try opencv4 first, then opencv (distro naming differs)
if(ENABLE_FACE_TRACKER)
    pkg_check_modules(OPENCV opencv4)
    if(NOT OPENCV_FOUND)
        pkg_check_modules(OPENCV opencv)
    endif()
else()
    set(OPENCV_FOUND FALSE)
endif()
if(ENABLE_FACE_TRACKER AND OPENCV_FOUND AND WEBCAM_SUPPORT)
    set(FACE_TRACKER_SUPPORT TRUE)
    message(STATUS "Face tracker support: enabled")
else()
    set(FACE_TRACKER_SUPPORT FALSE)
    if(NOT ENABLE_FACE_TRACKER)
        message(STATUS "Face tracker support: disabled (ENABLE_FACE_TRACKER=OFF)")
    elseif(NOT WEBCAM_SUPPORT)
        message(STATUS "Face tracker support: disabled (webcam support off)")
    elseif(NOT OPENCV_FOUND)
        message(STATUS "Face tracker support: disabled (OpenCV not found)")
        message(WARNING "ENABLE_FACE_TRACKER requested but OpenCV (opencv4/opencv) not found via pkg-config")
    endif()
endif()

# TrackIR support (always enabled if dependencies found)
if(USB_FOUND AND ZLIB_FOUND)
    set(TRACKIR_SUPPORT TRUE)
    message(STATUS "TrackIR support: enabled")
else()
    set(TRACKIR_SUPPORT FALSE)
    message(STATUS "TrackIR support: disabled (missing dependencies)")
endif()

# Qt6 detection
# Require Qt6.2 or higher (Qt6.5 LTS recommended for stability and best compatibility)
# Note: OpenGLWidgets requires OpenGL as a dependency
# Qt6.2+ requires C++17 (already set above)
# The fixes in this codebase are compatible with Qt6.2+, but Qt6.5 is recommended
find_package(Qt6 6.2 COMPONENTS Core Widgets OpenGL OpenGLWidgets REQUIRED)
find_package(Qt6 QUIET COMPONENTS Concurrent Network Sql)

# Get Qt6 version and provide compatibility information
if(Qt6Core_VERSION)
    if(Qt6Core_VERSION VERSION_LESS "6.5.0")
        message(STATUS "Qt6 version: ${Qt6Core_VERSION} (Qt6.5+ recommended for best compatibility)")
    else()
        message(STATUS "Qt6 version: ${Qt6Core_VERSION} (Qt6.5 or higher compatibility confirmed)")
    endif()
else()
    message(WARNING "Qt6 version could not be determined")
endif()

# Wine bridge detection (MinGW + NSIS with installer stubs)
find_program(MAKENSIS_EXECUTABLE makensis HINTS /usr/bin /usr/sbin)
set(NSIS_HAVE_STUBS FALSE)
if(MAKENSIS_EXECUTABLE)
    foreach(_nsis_stub_dir /usr/share/nsis/Stubs /usr/local/share/nsis/Stubs)
        if(EXISTS "${_nsis_stub_dir}/zlib-x86-unicode")
            set(NSIS_HAVE_STUBS TRUE)
            set(NSIS_STUBS_DIR "${_nsis_stub_dir}")
            break()
        endif()
    endforeach()
    if(NOT NSIS_HAVE_STUBS)
        message(WARNING "makensis found at ${MAKENSIS_EXECUTABLE} but NSIS stubs are missing.")
        message(WARNING "Wine DLLs may build, but linuxtrack-wine.exe will not. Install full nsis (Arch: yay -S nsis).")
    endif()
endif()
include(cmake/FindMinGW.cmake)
detect_mingw()

if(MINGW_FOUND AND MAKENSIS_EXECUTABLE)
    set(WINE_PLUGIN TRUE)
    set(WINE64 TRUE)
    set(WINE32 TRUE)
    if(NSIS_HAVE_STUBS)
        message(STATUS "Wine bridge: enabled (MinGW toolchain + makensis + NSIS stubs)")
    else()
        message(STATUS "Wine bridge: enabled (MinGW + makensis; NSIS stubs missing — installer skipped)")
    endif()
else()
    set(WINE_PLUGIN FALSE)
    set(WINE64 FALSE)
    set(WINE32 FALSE)
    message(STATUS "Wine bridge: disabled (mingw-w64 toolchains and/or makensis not found)")
endif()

# X-Plane SDK detection
if(ENABLE_XPLANE)
    set(XPLANE_SDK_FOUND FALSE)
    foreach(sdk_path 
        "${XPLANE_SDK_PATH}"
        "/opt/xplane-sdk/CHeaders"
        "/usr/include/xplane_sdk"
        "/usr/local/include/xplane_sdk"
        "$ENV{HOME}/xplane-sdk/CHeaders"
    )
        if(EXISTS "${sdk_path}/XPLM/XPLMPlugin.h")
            set(XPLANE_SDK_FOUND TRUE)
            set(XPLANE_SDK_PATH "${sdk_path}")
            message(STATUS "X-Plane SDK found: ${sdk_path}")
            break()
        endif()
    endforeach()
    
    if(NOT XPLANE_SDK_FOUND)
        message(WARNING "X-Plane SDK not found. X-Plane plugin will not be built.")
        message(WARNING "Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/")
        message(WARNING "Unzip the SDK archive (e.g. XPSDK*.zip) so /opt/xplane-sdk/CHeaders exists")
        message(WARNING "See docs/readme/arch-linux.md (X-Plane Support) for unzip/cp steps")
        set(ENABLE_XPLANE FALSE)
    endif()
endif()

# 32-bit library support
if(X64 AND ENABLE_LTR_32LIB_ON_X64)
    # Check for 32-bit development headers
    set(HAS_32BIT_HEADERS FALSE)
    foreach(header_path
        "/usr/include/i386-linux-gnu/bits/libc-header-start.h"
        "/usr/include/bits/libc-header-start.h"
        "/usr/include/i386-linux-gnu/stdlib.h"
        "/usr/include/stdlib.h"
    )
        if(EXISTS "${header_path}")
            set(HAS_32BIT_HEADERS TRUE)
            break()
        endif()
    endforeach()
    
    if(HAS_32BIT_HEADERS)
        set(LTR32ON64 TRUE)
        message(STATUS "32-bit library on 64-bit host: enabled")
    else()
        set(LTR32ON64 FALSE)
        message(WARNING "32-bit development headers not found. 32-bit library will not be built.")
        message(WARNING "Install gcc-multilib and libc6-dev-i386 (Debian/Ubuntu/MX) or glibc-devel.i686 (Fedora/RHEL)")
    endif()
else()
    set(LTR32ON64 FALSE)
endif()

# uinput detection
check_include_file(linux/uinput.h HAVE_UINPUT)
if(HAVE_UINPUT)
    set(MICKEY_SUPPORT TRUE)
    message(STATUS "Mickey support: enabled (uinput found)")
else()
    set(MICKEY_SUPPORT FALSE)
    message(STATUS "Mickey support: disabled (uinput not found)")
endif()

# Wine library paths detection is now done earlier when WINE_PLUGIN is enabled

# Set config.h defines (matching config.h.in template)
# Use separate variable to avoid overwriting boolean WEBCAM_SUPPORT used in conditions
if(WEBCAM_SUPPORT)
    set(WEBCAM_SUPPORT_DEFINE "#define WEBCAM_SUPPORT 1")
else()
    set(WEBCAM_SUPPORT_DEFINE "/* #undef WEBCAM_SUPPORT */")
endif()

if(TRACKIR_SUPPORT)
    set(TRACKIR_SUPPORT "#define TRACKIR_SUPPORT 1")
else()
    set(TRACKIR_SUPPORT "/* #undef TRACKIR_SUPPORT */")
endif()

# Use separate variable to avoid overwriting boolean WIIMOTE_SUPPORT used in conditions
if(WIIMOTE_SUPPORT)
    set(WIIMOTE_SUPPORT_DEFINE "#define WIIMOTE_SUPPORT 1")
else()
    set(WIIMOTE_SUPPORT_DEFINE "/* #undef WIIMOTE_SUPPORT */")
endif()

# Preserve boolean for src/CMakeLists.txt (config.h overwrites the string below)
set(FACE_TRACKER_SUPPORT_BOOL ${FACE_TRACKER_SUPPORT})
if(FACE_TRACKER_SUPPORT)
    set(FACE_TRACKER_SUPPORT "#define FACE_TRACKER_SUPPORT 1")
else()
    set(FACE_TRACKER_SUPPORT "/* #undef FACE_TRACKER_SUPPORT */")
endif()

if(ENABLE_XPLANE)
    set(XPLANE_PLUGIN_SUPPORT "#define XPLANE_PLUGIN_SUPPORT 1")
else()
    set(XPLANE_PLUGIN_SUPPORT "/* #undef XPLANE_PLUGIN_SUPPORT */")
endif()

if(MICKEY_SUPPORT)
    set(MICKEY_SUPPORT "#define MICKEY_SUPPORT 1")
else()
    set(MICKEY_SUPPORT "/* #undef MICKEY_SUPPORT */")
endif()

if(WINE_PLUGIN)
    set(WINE_PLUGIN_SUPPORT "#define WINE_PLUGIN_SUPPORT 1")
else()
    set(WINE_PLUGIN_SUPPORT "/* #undef WINE_PLUGIN_SUPPORT */")
endif()

if(OSC_SUPPORT)
    set(OSC_SUPPORT "#define OSC_SUPPORT 1")
else()
    set(OSC_SUPPORT "/* #undef OSC_SUPPORT */")
endif()

if(ENABLE_PIE)
    set(PIE_SUPPORT "#define PIE_SUPPORT 1")
else()
    set(PIE_SUPPORT "/* #undef PIE_SUPPORT */")
endif()

# Set additional config.h variables
set(PACKAGE_VERSION "${PROJECT_VERSION}")

if(HAVE_NEW_LIBLO)
    set(HAVE_NEW_LIBLO "#define HAVE_NEW_LIBLO 1")
else()
    set(HAVE_NEW_LIBLO "/* #undef HAVE_NEW_LIBLO */")
endif()

if(WITH_V4L2)
    set(V4L2 "#define V4L2 1")
else()
    set(V4L2 "/* #undef V4L2 */")
endif()

if(WITH_LIBV4L2)
    set(LIBV4L2 "#define LIBV4L2 1")
else()
    set(LIBV4L2 "/* #undef LIBV4L2 */")
endif()

# Configure header file using template
# Store booleans used after configure (do not alias WEBCAM_SUPPORT/WIIMOTE_SUPPORT to *_DEFINE
# strings — that breaks @ONLY substitution clarity and can confuse include order with a
# stale tree config.h).
set(WEBCAM_SUPPORT_BOOL ${WEBCAM_SUPPORT})
set(WIIMOTE_SUPPORT_BOOL ${WIIMOTE_SUPPORT})
configure_file(
    ${CMAKE_SOURCE_DIR}/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
    @ONLY
)
# Restore boolean values for use in src/CMakeLists.txt (called after this)
set(WEBCAM_SUPPORT ${WEBCAM_SUPPORT_BOOL})
set(WIIMOTE_SUPPORT ${WIIMOTE_SUPPORT_BOOL})
set(FACE_TRACKER_SUPPORT ${FACE_TRACKER_SUPPORT_BOOL})

# Set library path define
set(LIB_PATH "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack")
configure_file(
    ${CMAKE_SOURCE_DIR}/src/pathconfig.h.in
    ${CMAKE_BINARY_DIR}/src/pathconfig.h
    @ONLY
)

# Add subdirectories
add_subdirectory(src)

if(WINE_PLUGIN)
    add_subdirectory(src/wine_bridge)
endif()

add_subdirectory(doc)

# Installation
set(CMAKE_INSTALL_LIBDIR "lib" CACHE STRING "Library directory")
set(CMAKE_INSTALL_BINDIR "bin" CACHE STRING "Binary directory")
set(CMAKE_INSTALL_DATADIR "share" CACHE STRING "Data directory")

# Uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
        COMMENT "Uninstalling LinuxTrack"
    )
endif()

# Summary
message(STATUS "")
message(STATUS "===============================================================")
message(STATUS "===                         Summary                         ===")
message(STATUS "===============================================================")
message(STATUS "Webcam support: ${WEBCAM_SUPPORT}")
message(STATUS "Wiimote support: ${WIIMOTE_SUPPORT}")
message(STATUS "TrackIR support: ${TRACKIR_SUPPORT}")
message(STATUS "Facetracker support: ${FACE_TRACKER_SUPPORT}")
message(STATUS "XPlane plugin: ${ENABLE_XPLANE}")
message(STATUS "Mickey: ${MICKEY_SUPPORT}")
message(STATUS "Wine bridge: ${WINE_PLUGIN}")
message(STATUS "OSC support: ${OSC_SUPPORT}")
message(STATUS "PIE support for native builds: ${ENABLE_PIE}")
message(STATUS "===============================================================")

