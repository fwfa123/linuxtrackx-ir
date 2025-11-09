# src/wine_bridge/client/CMakeLists.txt
# TrackIR API compatibility DLLs

include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)

# 64-bit NPClient DLL (always built when WINE_PLUGIN is enabled, if 64-bit libs found)
# 64-bit Wine is easier to install, so we prioritize it
if(WINE64)
    add_wine64_library(NPClient
        SOURCES
            NPClient_main.c
            ../../linuxtrack.c
            ../../utils.c
            rest.c
        SPEC NPClient.spec
    )
endif()

# 32-bit NPClient DLL (built when WINE_PLUGIN is enabled, matching Autotools behavior)
# NOTE: 32-bit development headers are REQUIRED when WINE_PLUGIN is enabled (checked in CMakeLists.txt)
# This is a critical component for Wine plugin functionality
if(WINE_PLUGIN)
    add_wine_library(NPClient
        SOURCES
            NPClient_main.c
            ../../linuxtrack.c
            ../../utils.c
            rest.c
        SPEC NPClient.spec
    )
endif()

# check_data executable (32-bit only - no 64-bit version exists)
# NOTE: This is a CRITICAL 32-bit only component. 32-bit development headers are REQUIRED
# when WINE_PLUGIN is enabled (checked in CMakeLists.txt)
if(WINE_PLUGIN)
    add_wine_executable(check_data
        SOURCES
            check_data.c
            rest.c
    )
endif()

# Installation
if(WINE64)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/NPClient64.dll.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

if(WINE_PLUGIN)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/NPClient.dll.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

if(WINE_PLUGIN)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/check_data.exe.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

