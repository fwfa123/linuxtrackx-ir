include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)

if(WINE_PLUGIN)
    add_mingw_executable(Tester
        ARCH x86
        SOURCES
            main.cpp
            npview.rc
            rest.c
            npifc.c
    )

    set(TESTER64_OBJECTS)
    set(MW64_INC
        -DHAVE_CONFIG_H -DFOR_WIN64=1
        -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Tester64_main.o
        COMMAND ${MINGW64_GPP_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${MW64_INC} -o ${CMAKE_CURRENT_BINARY_DIR}/Tester64_main.o
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
    list(APPEND TESTER64_OBJECTS ${CMAKE_CURRENT_BINARY_DIR}/Tester64_main.o)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Tester64_rest.o
        COMMAND ${MINGW64_GCC_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/rest.c ${MW64_INC} -o ${CMAKE_CURRENT_BINARY_DIR}/Tester64_rest.o
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rest.c
    )
    list(APPEND TESTER64_OBJECTS ${CMAKE_CURRENT_BINARY_DIR}/Tester64_rest.o)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npifc.o
        COMMAND ${MINGW64_GCC_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/npifc.c ${MW64_INC} -o ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npifc.o
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/npifc.c
    )
    list(APPEND TESTER64_OBJECTS ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npifc.o)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npview.o
        COMMAND ${MINGW64_WINDRES_EXECUTABLE} -DFOR_WIN64=1 -I${CMAKE_CURRENT_SOURCE_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npview.o ${CMAKE_CURRENT_SOURCE_DIR}/npview.rc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/npview.rc
    )
    list(APPEND TESTER64_OBJECTS ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npview.o)

    add_custom_target(Tester64 ALL
        COMMAND ${MINGW64_GPP_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/Tester64.exe ${TESTER64_OBJECTS}
        DEPENDS ${TESTER64_OBJECTS}
        COMMENT "Linking Tester64.exe (x64)"
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Tester.exe
        ${CMAKE_CURRENT_BINARY_DIR}/Tester64.exe
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

