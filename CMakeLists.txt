cmake_minimum_required(VERSION 3.16)
project(linuxtrack VERSION 0.99.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options focused on TrackIR hardware
option(BUILD_TRACKIR_SUPPORT "Build TrackIR hardware support" ON)
option(BUILD_QT5_GUI "Build Qt5 GUI (modernized)" ON)
option(STUB_CAMERA_TRACKING "Stub out camera/webcam tracking" ON)
option(STUB_WIIMOTE_TRACKING "Stub out Wiimote tracking" ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# Qt5 for modernized GUI
if(BUILD_QT5_GUI)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL)
    message(STATUS "Qt5 found: ${Qt5_VERSION}")
endif()

# TrackIR hardware support
if(BUILD_TRACKIR_SUPPORT)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
    if(LIBUSB_FOUND)
        message(STATUS "libusb-1.0 found for TrackIR support")
        set(HAVE_TRACKIR ON)
    else()
        message(WARNING "libusb-1.0 not found - TrackIR support disabled")
        set(HAVE_TRACKIR OFF)
    endif()
endif()

# System libraries
find_library(MATH_LIBRARY m)
find_package(Threads REQUIRED)

# Find mxml for configuration
pkg_check_modules(MXML mxml)
if(NOT MXML_FOUND)
    message(WARNING "mxml not found - using fallback")
endif()

# Configure header
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/local_config.h.in"
    "${CMAKE_BINARY_DIR}/local_config.h"
    @ONLY
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

# Add subdirectories
add_subdirectory(src)

# Installation
install(FILES 99-TIR.rules 
    DESTINATION /lib/udev/rules.d/
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

# Summary
message(STATUS "")
message(STATUS "=== LinuxTrack Configuration Summary ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "TrackIR support: ${HAVE_TRACKIR}")
message(STATUS "Qt5 GUI: ${BUILD_QT5_GUI}")
message(STATUS "Camera tracking: STUBBED")
message(STATUS "Wiimote tracking: STUBBED")
message(STATUS "========================================")
message(STATUS "") 