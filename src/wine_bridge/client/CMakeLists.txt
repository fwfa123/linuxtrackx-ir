# src/wine_bridge/client/CMakeLists.txt
# TrackIR API compatibility DLLs

include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

if(WINE_PLUGIN)
    add_mingw_library(NPClient64
        ARCH x64
        SOURCES
            NPClient_main.c
            ../../linuxtrack.c
            rest.c
            linuxtrack_path_shim.c
        LIBS -lws2_32
    )

    add_mingw_library(NPClient
        ARCH x86
        SOURCES
            NPClient_main.c
            ../../linuxtrack.c
            rest.c
            linuxtrack_path_shim.c
        LIBS -lws2_32
    )

    add_mingw_executable(check_data
        ARCH x86
        SOURCES
            check_data.c
            rest.c
    )
endif()

# Installation
if(WINE_PLUGIN)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/NPClient64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/NPClient.dll
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/check_data.exe
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

