include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

if(WINE_PLUGIN)
    add_mingw_library(FreeTrackClient
        ARCH x86
        SOURCES
            FreeTrackClient_main.c
            ../../linuxtrack.c
            ../client/linuxtrack_path_shim.c
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/FreeTrackClient.dll
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

