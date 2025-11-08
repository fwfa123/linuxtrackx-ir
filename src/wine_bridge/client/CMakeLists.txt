# src/wine_bridge/client/CMakeLists.txt
# TrackIR API compatibility DLLs

include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)

# 32-bit NPClient DLL
# Note: linuxtrack.c and utils.c are from src/ directory
add_wine_library(NPClient
    SOURCES
        NPClient_main.c
        ../../linuxtrack.c
        ../../utils.c
        rest.c
    SPEC NPClient.spec
)

# 64-bit NPClient DLL (if WINE64)
if(WINE64)
    add_wine64_library(NPClient
        SOURCES
            NPClient_main.c
            ../../linuxtrack.c
            ../../utils.c
            rest.c
        SPEC NPClient.spec
    )
endif()

# check_data executable
add_wine_executable(check_data
    SOURCES
        check_data.c
        rest.c
)

# Installation
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NPClient.dll.so
    DESTINATION lib/linuxtrack/wine_bridge
    OPTIONAL
)

if(WINE64)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/NPClient64.dll.so
        DESTINATION lib/linuxtrack/wine_bridge
        OPTIONAL
    )
endif()

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/check_data.exe.so
    DESTINATION lib/linuxtrack/wine_bridge
    OPTIONAL
)

