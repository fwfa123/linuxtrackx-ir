# src/wine_bridge/controller/CMakeLists.txt
# Controller component - matches Autotools behavior
# Only 32-bit version exists (no 64-bit version)

include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)

# 32-bit Controller executable (built when WINE_PLUGIN is enabled, matching Autotools behavior)
# Matches: Controller.exe.so : main.o resources.o dinput.o linuxtrack.o utils.o
# Note: utils.c is from ../../utils.c, linuxtrack.c is from current directory
# NOTE: This is a CRITICAL 32-bit only component. 32-bit development headers are REQUIRED
# when WINE_PLUGIN is enabled (checked in CMakeLists.txt)

# Build when WINE_PLUGIN is enabled (matching Autotools behavior)
if(WINE_PLUGIN)
# Compile utils.c from parent directory (special handling per Makefile.am)
set(utils_obj ${CMAKE_CURRENT_BINARY_DIR}/Controller_utils.o)
add_custom_command(
    OUTPUT ${utils_obj}
    COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_SOURCE_DIR}/src/utils.c
        -I${CMAKE_SOURCE_DIR}/src -I${CMAKE_SOURCE_DIR} -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -DHAVE_CONFIG_H -m32 -g
        -o ${utils_obj}
    DEPENDS ${CMAKE_SOURCE_DIR}/src/utils.c ${CMAKE_SOURCE_DIR}/src/utils.h ${CMAKE_BINARY_DIR}/config.h
    COMMENT "Compiling utils.c for Controller"
)

# Compile linuxtrack.c from src/ directory (VPATH in Makefile.am looks in ../..)
set(linuxtrack_obj ${CMAKE_CURRENT_BINARY_DIR}/Controller_linuxtrack.o)
add_custom_command(
    OUTPUT ${linuxtrack_obj}
    COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_SOURCE_DIR}/src/linuxtrack.c
        -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
        -DHAVE_CONFIG_H -m32 -g
        -o ${linuxtrack_obj}
    DEPENDS ${CMAKE_SOURCE_DIR}/src/linuxtrack.c ${CMAKE_SOURCE_DIR}/src/linuxtrack.h ${CMAKE_BINARY_DIR}/config.h
    COMMENT "Compiling linuxtrack.c for Controller"
)

# Compile other sources using standard wine functions
set(CONTROLLER_OBJECTS ${utils_obj} ${linuxtrack_obj})

# Compile main.cpp
set(main_obj ${CMAKE_CURRENT_BINARY_DIR}/Controller_main.o)
add_custom_command(
    OUTPUT ${main_obj}
    COMMAND ${WINEGPP_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        -DHAVE_CONFIG_H
        -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
        -m32 -g
        -o ${main_obj}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_BINARY_DIR}/config.h
    COMMENT "Compiling main.cpp for Controller"
)
list(APPEND CONTROLLER_OBJECTS ${main_obj})

# Compile dinput.cpp
set(dinput_obj ${CMAKE_CURRENT_BINARY_DIR}/Controller_dinput.o)
add_custom_command(
    OUTPUT ${dinput_obj}
    COMMAND ${WINEGPP_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/dinput.cpp
        -DHAVE_CONFIG_H
        -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
        -m32 -g
        -o ${dinput_obj}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/dinput.cpp ${CMAKE_CURRENT_SOURCE_DIR}/kbi_interface.h
    COMMENT "Compiling dinput.cpp for Controller"
)
list(APPEND CONTROLLER_OBJECTS ${dinput_obj})

# Compile resources.rc
set(resources_obj ${CMAKE_CURRENT_BINARY_DIR}/Controller_resources.o)
add_custom_command(
    OUTPUT ${resources_obj}
    COMMAND ${WRC_EXECUTABLE} -o ${resources_obj} ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc
    COMMENT "Compiling resources.rc for Controller"
)
list(APPEND CONTROLLER_OBJECTS ${resources_obj})

# Link Controller executable - matches Makefile.am: winegcc -o Controller.exe.so $(LDFLAGS) -m32 $^ -ldxguid -ldinput -ldinput8 -lstdc++ -lshell32
add_custom_target(Controller ALL
    COMMAND ${WINEGCC_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/Controller.exe.so -m32
        ${CONTROLLER_OBJECTS}
        -ldxguid -ldinput -ldinput8 -lstdc++ -lshell32
    DEPENDS ${CONTROLLER_OBJECTS}
    COMMENT "Linking Controller with winegcc"
)

set_target_properties(Controller PROPERTIES
        OUTPUT_NAME "Controller.exe.so"
    )

    # Installation
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Controller.exe.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

