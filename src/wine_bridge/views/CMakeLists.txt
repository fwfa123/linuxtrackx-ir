include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

if(WINE_PLUGIN)
    add_mingw_executable(TrackIR
        ARCH x86
        SOURCES
            main.cpp
            rest.c
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/TrackIR.exe
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

