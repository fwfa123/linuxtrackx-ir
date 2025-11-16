# src/wine_bridge/tester/CMakeLists.txt
# Tester component - matches Autotools behavior

include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)

# 64-bit Tester executable (always built when WINE_PLUGIN is enabled, if 64-bit libs found)
# 64-bit Wine is easier to install, so we prioritize it
# Matches: Tester64.exe.so : main64.o rest64.o npifc64.o npview64.o
# Note: npifc.c needs -DFOR_WIN64=1 for 64-bit build
if(WINE64)
    set(TESTER64_OBJECTS)
    
    # Compile main.cpp (64-bit, no -m32, no -fPIC needed for executable)
    set(main64_obj ${CMAKE_CURRENT_BINARY_DIR}/Tester64_main64.o)
    add_custom_command(
        OUTPUT ${main64_obj}
        COMMAND ${WINEGPP_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
            -g -DHAVE_CONFIG_H
            -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
            -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
            -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
            -o ${main64_obj}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        COMMENT "Compiling main.cpp for Tester64 (64-bit)"
    )
    list(APPEND TESTER64_OBJECTS ${main64_obj})
    
    # Compile rest.c (64-bit)
    set(rest64_obj ${CMAKE_CURRENT_BINARY_DIR}/Tester64_rest64.o)
    add_custom_command(
        OUTPUT ${rest64_obj}
        COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/rest.c
            -g -DHAVE_CONFIG_H
            -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
            -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
            -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
            -o ${rest64_obj}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rest.c ${CMAKE_CURRENT_SOURCE_DIR}/rest.h
        COMMENT "Compiling rest.c for Tester64 (64-bit)"
    )
    list(APPEND TESTER64_OBJECTS ${rest64_obj})
    
    # Compile npifc.c with -DFOR_WIN64=1 (64-bit) - special case per Makefile.am
    set(npifc64_obj ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npifc64.o)
    add_custom_command(
        OUTPUT ${npifc64_obj}
        COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/npifc.c
            -DFOR_WIN64=1 -g -DHAVE_CONFIG_H
            -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
            -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
            -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
            -o ${npifc64_obj}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/npifc.c ${CMAKE_CURRENT_SOURCE_DIR}/npifc.h
        COMMENT "Compiling npifc.c for Tester64 (64-bit with FOR_WIN64)"
    )
    list(APPEND TESTER64_OBJECTS ${npifc64_obj})
    
    # Compile npview.rc with -DFOR_WIN64=1 (64-bit) - matches .rc64.o rule
    set(npview64_obj ${CMAKE_CURRENT_BINARY_DIR}/Tester64_npview64.o)
    add_custom_command(
        OUTPUT ${npview64_obj}
        COMMAND ${WRC_EXECUTABLE} --utf8 -DFOR_WIN64=1 -o ${npview64_obj} -I${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/npview.rc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/npview.rc
        COMMENT "Compiling npview.rc for Tester64 (64-bit)"
    )
    list(APPEND TESTER64_OBJECTS ${npview64_obj})
    
    # Link Tester64 executable - matches Makefile.am: winegcc -g -o Tester64.exe.so -L. $(WINE64_LIBS) $(LDFLAGS) -Wall -Wextra $^
    add_custom_target(Tester64 ALL
        COMMAND ${WINEGCC_EXECUTABLE} -g -o ${CMAKE_CURRENT_BINARY_DIR}/Tester64.exe.so -L. ${WINE64_LIBS} -Wall -Wextra ${TESTER64_OBJECTS}
        DEPENDS ${TESTER64_OBJECTS}
        COMMENT "Linking Tester64 with winegcc (64-bit)"
    )
    
    set_target_properties(Tester64 PROPERTIES
        OUTPUT_NAME "Tester64.exe.so"
    )
endif()

# 32-bit Tester executable (built when WINE_PLUGIN is enabled, matching Autotools behavior)
# Matches: Tester.exe.so : main.o npview.o rest.o npifc.o
# NOTE: 32-bit development headers are REQUIRED when WINE_PLUGIN is enabled (checked in CMakeLists.txt)
# This is a critical component for Wine plugin functionality
if(WINE_PLUGIN)
    add_wine_executable(Tester
        SOURCES
            main.cpp
            npview.rc
            rest.c
            npifc.c
    )
endif()

# Installation - matches Autotools noinst_SCRIPTS behavior
if(WINE64)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Tester64.exe.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

if(WINE_PLUGIN)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Tester.exe.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

