# src/wine_bridge/client/CMakeLists.txt
# TrackIR API compatibility DLLs

include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

if(WINE_PLUGIN)
    add_mingw_library(NPClient64
        ARCH x64
        DEF NPClient.def
        SOURCES
            NPClient_main.c
            ../../linuxtrack.c
            ../../axis.c
            ../../spline.c
            ../../math_utils.c
            rest.c
            linuxtrack_path_shim.c
            mingw_axes_prefs.c
        LIBS -lws2_32 -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
    )

    add_mingw_library(NPClient
        ARCH x86
        DEF NPClient.def
        SOURCES
            NPClient_main.c
            ../../linuxtrack.c
            ../../axis.c
            ../../spline.c
            ../../math_utils.c
            rest.c
            linuxtrack_path_shim.c
            mingw_axes_prefs.c
        LIBS -lws2_32 -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
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

