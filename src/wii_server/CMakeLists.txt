# src/wii_server/CMakeLists.txt
# Wii server application

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(wii_server
    wii_server.h
    wiimote.h
    ../wii_com.h
    ../com_proc.h
    ../image_process.h
    wii_server.ui
    main.cpp
    wii_server.cpp
    wiimote.cpp
    wii_server.qrc
)

target_link_libraries(wii_server
    Qt5::Core
    Qt5::Widgets
    ltr
    ${MATH_LIBRARY}
    ${CWIID_LIBRARIES}
)

target_compile_definitions(wii_server PRIVATE
    HAVE_CONFIG_H
)

target_include_directories(wii_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
)

target_compile_options(wii_server PRIVATE
    -Wall
    -Wextra
    -fvisibility=hidden
)

if(NOT APPLE)
    target_compile_options(wii_server PRIVATE
        -Wformat
        -Wformat-security
        --param ssp-buffer-size=4
        -fstack-protector
        -D_FORTIFY_SOURCE=1
    )
endif()

set_target_properties(wii_server PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack"
    BUILD_WITH_INSTALL_RPATH TRUE
)

install(TARGETS wii_server
    RUNTIME DESTINATION bin
)

