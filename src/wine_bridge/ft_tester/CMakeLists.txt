include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

if(WINE_PLUGIN)
    add_mingw_executable(ftc
        ARCH x86
        SOURCES
            main.cpp
            fttester.rc
            ../client/rest.c
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ftc.exe
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

