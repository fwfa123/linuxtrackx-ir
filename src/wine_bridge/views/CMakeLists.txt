# src/wine_bridge/views/CMakeLists.txt
# Views component (TrackIR) - matches Autotools behavior
# Only 32-bit version exists (no 64-bit version)

include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)

# 32-bit TrackIR executable (built when WINE_PLUGIN is enabled, matching Autotools behavior)
# Matches: TrackIR.exe.so : main.o rest.o
# NOTE: This is a CRITICAL 32-bit only component. 32-bit development headers are REQUIRED
# when WINE_PLUGIN is enabled (checked in CMakeLists.txt)
if(WINE_PLUGIN)
    add_wine_executable(TrackIR
        SOURCES
            main.cpp
            rest.c
    )
endif()

# Installation
if(WINE_PLUGIN)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/TrackIR.exe.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

