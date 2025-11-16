# src/mickey/CMakeLists.txt
# Mickey calibration tool

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(mickey
    mickey.h
    mickey.ui
    calibration.ui
    chsettings.ui
    ../qt_gui/logview.ui
    hotkey.ui
    hotkey_setup.ui
    main.cpp
    mickey.cpp
    ../ltlib.c
    ../math_utils.c
    uinput_ifc.c
    piper.c
    keyb.cpp
    transform.cpp
    ../qt_gui/help_view.cpp
    ../qt_gui/help_viewer.cpp
    ../qt_gui/ltr_gui_prefs.cpp
    keyb_x11.cpp
    ../linuxtrack.c
    hotkey.cpp
    my_line_edit.cpp
    hotkey_setup_dlg.cpp
    mickey.qrc
)

# Exclude keyb_x11.h from MOC processing (X11 includes cause parsing issues, and Q_OBJECT is commented out)
set_source_files_properties(keyb_x11.h PROPERTIES SKIP_AUTOMOC ON)

if(NOT APPLE)
    target_sources(mickey PRIVATE mouse_linux.cpp)
else()
    target_sources(mickey PRIVATE mouse_mac.cpp)
endif()

target_link_libraries(mickey
    Qt6::Core
    Qt6::Widgets
    Qt6::Help
    ltr
    ${MATH_LIBRARY}
    dl
)

if(NOT APPLE)
    # Note: Qt6::X11Extras is not available in Qt6, X11 functionality is handled differently
    target_link_libraries(mickey
        X11
    )
endif()

target_compile_definitions(mickey PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    HAVE_CONFIG_H
    HELP_BASE="mickey/"
)

if(NOT APPLE)
    target_compile_definitions(mickey PRIVATE
        QT6_OVERRIDES
    )
endif()

target_include_directories(mickey PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../qt_gui
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
)

target_compile_options(mickey PRIVATE
    -Wall
    -Wextra
    -fvisibility=hidden
)

if(NOT APPLE)
    target_compile_options(mickey PRIVATE
        -Wformat
        -Wformat-security
        --param ssp-buffer-size=4
        -fstack-protector
        -D_FORTIFY_SOURCE=1
    )
endif()

set_target_properties(mickey PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack"
    BUILD_WITH_INSTALL_RPATH TRUE
)

install(TARGETS mickey
    RUNTIME DESTINATION bin
)

