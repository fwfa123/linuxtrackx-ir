# CMake build system for LinuxTrack X-IR
# Migrated from Autotools

cmake_minimum_required(VERSION 3.16)
project(LinuxTrack VERSION 0.99.27 LANGUAGES C CXX)

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

# Optional feature options
option(ENABLE_OSC "Enable OSC support" OFF)
option(ENABLE_WIIMOTE "Enable Wiimote support" OFF)
option(ENABLE_XPLANE "Enable X-Plane plugin" OFF)

# Path options
set(XPLANE_SDK_PATH "/opt/xplane-sdk/CHeaders" CACHE PATH "Path to X-Plane SDK")
set(WINE_LIBS_PATH "" CACHE PATH "Wine libraries path (auto-detect if empty)")
set(WINE64_LIBS_PATH "" CACHE PATH "Wine64 libraries path (auto-detect if empty)")
set(LIB32DIR "lib32" CACHE STRING "32-bit library directory (usually needed only on amd64 arch)")

# Set C/C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Compiler flags
if(ENABLE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -Wextra -Wformat -Wformat-security")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wextra -Wformat -Wformat-security")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
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
if(CWIID_FOUND)
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

# OpenCV (for face tracker)
pkg_check_modules(OPENCV opencv4)
if(OPENCV_FOUND AND WEBCAM_SUPPORT)
    set(FACE_TRACKER_SUPPORT TRUE)
    message(STATUS "Face tracker support: enabled")
else()
    set(FACE_TRACKER_SUPPORT FALSE)
    message(STATUS "Face tracker support: disabled")
endif()

# TrackIR support (always enabled if dependencies found)
if(USB_FOUND AND ZLIB_FOUND)
    set(TRACKIR_SUPPORT TRUE)
    message(STATUS "TrackIR support: enabled")
else()
    set(TRACKIR_SUPPORT FALSE)
    message(STATUS "TrackIR support: disabled (missing dependencies)")
endif()

# Qt5 detection
find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)
find_package(Qt5 QUIET COMPONENTS X11Extras Help Sql Concurrent Network)

# Find qmake
find_program(QMAKE_EXECUTABLE 
    NAMES qmake-qt5 qt5-qmake qmake5 qmake
    REQUIRED
)

# Verify Qt5 version
execute_process(
    COMMAND ${QMAKE_EXECUTABLE} -query QT_VERSION
    OUTPUT_VARIABLE QT5_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(QT5_VERSION)
    message(STATUS "Qt5 version: ${QT5_VERSION}")
else()
    message(FATAL_ERROR "Qt5 qmake not found or invalid")
endif()

# Find qhelpgenerator
find_program(QT5_QHELPGENERATOR
    NAMES qhelpgenerator-qt5 qhelpgenerator
)

# Wine detection
find_program(WINEGCC_EXECUTABLE winegcc)
find_program(WINEGPP_EXECUTABLE wineg++)
find_program(WRC_EXECUTABLE wrc)
find_program(MAKENSIS_EXECUTABLE makensis)

if(WINEGCC_EXECUTABLE AND WINEGPP_EXECUTABLE AND MAKENSIS_EXECUTABLE)
    set(WINE_PLUGIN TRUE)
    message(STATUS "Wine plugin: enabled")
    
    # Check for wine64
    find_program(WINE64_EXECUTABLE wine64)
    if(WINE64_EXECUTABLE OR X64)
        set(WINE64 TRUE)
        message(STATUS "Wine64 plugin: enabled")
    else()
        set(WINE64 FALSE)
        message(STATUS "Wine64 plugin: disabled")
    endif()
else()
    set(WINE_PLUGIN FALSE)
    set(WINE64 FALSE)
    message(STATUS "Wine plugin: disabled (winegcc/wineg++/makensis not found)")
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
        message(WARNING "And extract to: /opt/xplane-sdk/")
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

# Wine library paths detection (multi-distribution support)
if(WINE_PLUGIN)
    include(cmake/FindWineLibs.cmake)
    detect_wine_libs()
endif()

# Set config.h defines (matching config.h.in template)
if(WEBCAM_SUPPORT)
    set(WEBCAM_SUPPORT "#define WEBCAM_SUPPORT 1")
else()
    set(WEBCAM_SUPPORT "/* #undef WEBCAM_SUPPORT */")
endif()

if(TRACKIR_SUPPORT)
    set(TRACKIR_SUPPORT "#define TRACKIR_SUPPORT 1")
else()
    set(TRACKIR_SUPPORT "/* #undef TRACKIR_SUPPORT */")
endif()

if(WIIMOTE_SUPPORT)
    set(WIIMOTE_SUPPORT "#define WIIMOTE_SUPPORT 1")
else()
    set(WIIMOTE_SUPPORT "/* #undef WIIMOTE_SUPPORT */")
endif()

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
configure_file(
    ${CMAKE_SOURCE_DIR}/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
    @ONLY
)

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
message(STATUS "Wine plugin: ${WINE_PLUGIN}")
message(STATUS "OSC support: ${OSC_SUPPORT}")
message(STATUS "PIE support for native builds: ${ENABLE_PIE}")
message(STATUS "===============================================================")

