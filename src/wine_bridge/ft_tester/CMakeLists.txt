# src/wine_bridge/ft_tester/CMakeLists.txt
# FreeTrack tester - matches Autotools behavior
# Only 32-bit version exists (no 64-bit version)

include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)

# 32-bit ftc executable (built when WINE_PLUGIN is enabled, matching Autotools behavior)
# Matches: ftc.exe.so : main.o fttester.o ../client/rest.o
# Note: rest.c is from ../client/rest.c, needs special include paths
# NOTE: This is a CRITICAL 32-bit only component. 32-bit development headers are REQUIRED
# when WINE_PLUGIN is enabled (checked in CMakeLists.txt)

# Build when WINE_PLUGIN is enabled (matching Autotools behavior)
if(WINE_PLUGIN)
# Compile rest.c from client directory (special handling per Makefile.am)
set(rest_obj ${CMAKE_CURRENT_BINARY_DIR}/ftc_rest.o)
add_custom_command(
    OUTPUT ${rest_obj}
    COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_SOURCE_DIR}/src/wine_bridge/client/rest.c
        -g -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR}/../client -I${CMAKE_CURRENT_SOURCE_DIR}/../..
        -DHAVE_CONFIG_H -m32
        -o ${rest_obj}
    DEPENDS ${CMAKE_SOURCE_DIR}/src/wine_bridge/client/rest.c ${CMAKE_SOURCE_DIR}/src/wine_bridge/client/rest.h
    COMMENT "Compiling rest.c from client for ftc"
)

# Compile main.cpp
set(main_obj ${CMAKE_CURRENT_BINARY_DIR}/ftc_main.o)
add_custom_command(
    OUTPUT ${main_obj}
    COMMAND ${WINEGPP_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        -g -DHAVE_CONFIG_H
        -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR}/../client -I${CMAKE_CURRENT_SOURCE_DIR}/../..
        -m32
        -o ${main_obj}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    COMMENT "Compiling main.cpp for ftc"
)

# Compile fttester.rc
set(fttester_obj ${CMAKE_CURRENT_BINARY_DIR}/ftc_fttester.o)
add_custom_command(
    OUTPUT ${fttester_obj}
    COMMAND ${WRC_EXECUTABLE} -o ${fttester_obj} -I${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/fttester.rc
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fttester.rc ${CMAKE_CURRENT_SOURCE_DIR}/resource.h ${CMAKE_BINARY_DIR}/config.h
    COMMENT "Compiling fttester.rc for ftc"
)

# Link ftc executable - matches Makefile.am: wineg++ -g -o $@ -L. $(WINE_LIBS) $(LDFLAGS) -m32 -Wall -Wextra $^
add_custom_target(ftc ALL
    COMMAND ${WINEGPP_EXECUTABLE} -g -o ${CMAKE_CURRENT_BINARY_DIR}/ftc.exe.so -L. ${WINE_LIBS} -m32 -Wall -Wextra
        ${main_obj} ${fttester_obj} ${rest_obj}
    DEPENDS ${main_obj} ${fttester_obj} ${rest_obj}
    COMMENT "Linking ftc with wineg++"
)

set_target_properties(ftc PROPERTIES
        OUTPUT_NAME "ftc.exe.so"
    )

    # Installation
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ftc.exe.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

