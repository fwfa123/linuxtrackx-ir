# src/wine_bridge/ft_client/CMakeLists.txt
# FreeTrack client DLL - matches Autotools behavior
# Only 32-bit version exists (no 64-bit version)

include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)

# 32-bit FreeTrackClient DLL (built when WINE_PLUGIN is enabled, matching Autotools behavior)
# Matches: FreeTrackClient.dll.so : FreeTrackClient.spec FreeTrackClient_main.o linuxtrack.o utils.o
# Note: utils.c is from ../../utils.c, linuxtrack.c is from current directory

# Build when WINE_PLUGIN is enabled (matching Autotools behavior)
if(WINE_PLUGIN)
# Compile utils.c from parent directory (special handling per Makefile.am)
set(utils_obj ${CMAKE_CURRENT_BINARY_DIR}/FreeTrackClient_utils.o)
add_custom_command(
    OUTPUT ${utils_obj}
    COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_SOURCE_DIR}/src/utils.c
        -g -I${CMAKE_SOURCE_DIR}/src -I${CMAKE_SOURCE_DIR} -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR}/../.. -DHAVE_CONFIG_H -m32
        -o ${utils_obj}
    DEPENDS ${CMAKE_SOURCE_DIR}/src/utils.c ${CMAKE_SOURCE_DIR}/src/utils.h
    COMMENT "Compiling utils.c for FreeTrackClient"
)

# Compile linuxtrack.c from src/ directory (VPATH in Makefile.am looks in ../..)
set(linuxtrack_obj ${CMAKE_CURRENT_BINARY_DIR}/FreeTrackClient_linuxtrack.o)
add_custom_command(
    OUTPUT ${linuxtrack_obj}
    COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_SOURCE_DIR}/src/linuxtrack.c
        -g -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
        -DHAVE_CONFIG_H -m32
        -o ${linuxtrack_obj}
    DEPENDS ${CMAKE_SOURCE_DIR}/src/linuxtrack.c ${CMAKE_SOURCE_DIR}/src/linuxtrack.h
    COMMENT "Compiling linuxtrack.c for FreeTrackClient"
)

# Compile FreeTrackClient_main.c
set(main_obj ${CMAKE_CURRENT_BINARY_DIR}/FreeTrackClient_FreeTrackClient_main.o)
add_custom_command(
    OUTPUT ${main_obj}
    COMMAND ${WINEGCC_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR}/FreeTrackClient_main.c
        -g -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR} -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/../..
        -DHAVE_CONFIG_H -m32
        -o ${main_obj}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/FreeTrackClient_main.c ${CMAKE_CURRENT_SOURCE_DIR}/FreeTrackClient_dll.h
    COMMENT "Compiling FreeTrackClient_main.c"
)

# Link FreeTrackClient DLL - matches Makefile.am: winegcc $(WINE_LIBS) $(LDFLAGS) -m32 -Wall -Wextra -g -o $@ -shared $^ -ldl
add_custom_target(FreeTrackClient ALL
    COMMAND ${WINEGCC_EXECUTABLE} ${WINE_LIBS} -m32 -Wall -Wextra -g
        -o ${CMAKE_CURRENT_BINARY_DIR}/FreeTrackClient.dll.so
        -shared ${main_obj} ${linuxtrack_obj} ${utils_obj} -ldl
    DEPENDS ${main_obj} ${linuxtrack_obj} ${utils_obj} ${CMAKE_CURRENT_SOURCE_DIR}/FreeTrackClient.spec
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Linking FreeTrackClient with winegcc"
)

set_target_properties(FreeTrackClient PROPERTIES
        OUTPUT_NAME "FreeTrackClient.dll.so"
    )

    # Installation
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/FreeTrackClient.dll.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

