cmake_minimum_required(VERSION 3.16)
project(LinuxTrack VERSION 0.99.20 LANGUAGES C CXX)

# Modern C++ standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build options
option(BUILD_GUI "Build Qt5 GUI" ON)
option(BUILD_WINE_BRIDGE "Build Wine bridge" ON)
option(BUILD_OPENCV_SUPPORT "Build OpenCV webcam support" ON)
option(BUILD_TRACKIR_SUPPORT "Build TrackIR support" ON)
option(BUILD_WIIMOTE_SUPPORT "Build Wiimote support" ON)
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_STANDALONE_TEST "Build standalone GUI test" ON)

# Find packages
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

# Core dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(MXML REQUIRED mxml)

# Math library
find_library(MATH_LIBRARY m)
if(NOT MATH_LIBRARY)
    message(FATAL_ERROR "Math library not found")
endif()

# Qt5 for GUI (already working!)
if(BUILD_GUI)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Help)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
endif()

# Optional dependencies
if(BUILD_OPENCV_SUPPORT)
    find_package(OpenCV)
    if(OpenCV_FOUND)
        set(HAVE_OPENCV ON)
    endif()
endif()

if(BUILD_TRACKIR_SUPPORT)
    pkg_check_modules(LIBUSB libusb-1.0)
    find_package(ZLIB)
    if(LIBUSB_FOUND AND ZLIB_FOUND)
        set(HAVE_TRACKIR ON)
    endif()
endif()

if(BUILD_WIIMOTE_SUPPORT)
    pkg_check_modules(CWIID cwiid)
    if(CWIID_FOUND)
        set(HAVE_WIIMOTE ON)
    endif()
endif()

# Configure header
configure_file(
    ${CMAKE_SOURCE_DIR}/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
    ${MXML_INCLUDE_DIRS}
)

# Compiler flags (improved from autotools)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wformat -Wformat-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat -Wformat-security")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -fstack-protector -D_FORTIFY_SOURCE=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -fstack-protector -D_FORTIFY_SOURCE=2")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# Add subdirectories
add_subdirectory(src)

# Testing (preserve your standalone test!)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install rules
install(DIRECTORY share/linuxtrack DESTINATION share)
install(FILES share/pixmaps/linuxtrack.png DESTINATION share/pixmaps)

# Package configuration
set(CPACK_PACKAGE_NAME "LinuxTrack")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Head tracking system for Linux")
set(CPACK_PACKAGE_VENDOR "LinuxTrack Community")
set(CPACK_GENERATOR "DEB;RPM;TGZ")

# Modern packaging
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5, libqt5help5, libmxml1, libusb-1.0-0")
set(CPACK_RPM_PACKAGE_REQUIRES "qt5-qtbase, qt5-qttools, mxml, libusb1")

include(CPack)

# Status report
message(STATUS "LinuxTrack Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Qt5 GUI: ${BUILD_GUI}")
message(STATUS "  OpenCV Support: ${HAVE_OPENCV}")
message(STATUS "  TrackIR Support: ${HAVE_TRACKIR}")
message(STATUS "  Wiimote Support: ${HAVE_WIIMOTE}")
message(STATUS "  Standalone Test: ${BUILD_STANDALONE_TEST}") 