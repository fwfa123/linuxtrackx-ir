# src/wine_bridge/CMakeLists.txt
# Wine bridge components (MinGW PE payloads installed to lib/linuxtrack/wine_bridge)

if(WINE_PLUGIN)
    include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

    add_subdirectory(client)
    add_subdirectory(controller)
    add_subdirectory(tester)
    add_subdirectory(ft_client)
    add_subdirectory(ft_tester)
    add_subdirectory(views)

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/README.wine)
        install(FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/README.wine
            DESTINATION share/linuxtrack/wine
            OPTIONAL
        )
    endif()
endif()
