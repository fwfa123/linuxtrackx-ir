include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

if(WINE_PLUGIN)
    add_mingw_executable(Controller
        ARCH x86
        SOURCES
            main.cpp
            dinput.cpp
            resources.rc
            ../../linuxtrack.c
            ../client/linuxtrack_path_shim.c
        LIBS
            -ldxguid -ldinput -ldinput8 -lshell32 -lcomctl32
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Controller.exe
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

