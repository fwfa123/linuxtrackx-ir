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

# Set RPATH so mickey can find linuxtrack libraries at runtime
set_linuxtrack_rpath(mickey)

# ============================================================================
# Help File Generation
# ============================================================================

# Find qhelpgenerator for help file generation
# Qt6.2+: Use Qt6 qhelpgenerator for best compatibility with Qt6 QHelpEngine
# Help files generated with Qt5 qhelpgenerator may work but Qt6-generated files are recommended
# Arch: qhelpgenerator is in /usr/lib/qt6/ (not in PATH). Debian/Fedora: often in qt6/bin.
find_program(QT6_QHELPGENERATOR
    NAMES qhelpgenerator-qt6 qhelpgenerator
    PATHS /usr/lib/qt6/bin /usr/lib/qt6 /usr/lib64/qt6/bin
)

set(HELP_QCH_FILE ${CMAKE_CURRENT_BINARY_DIR}/help.qch)
set(HELP_QHC_FILE ${CMAKE_CURRENT_BINARY_DIR}/help.qhc)

if(QT6_QHELPGENERATOR)
    # Generate help.qch from mickey.qhp
    add_custom_command(
        OUTPUT ${HELP_QCH_FILE}
        COMMAND ${QT6_QHELPGENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/mickey.qhp -o ${HELP_QCH_FILE}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mickey.qhp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating Mickey Qt Help file (help.qch)"
    )
    
    # Generate help.qhc from mickey.qhcp
    add_custom_command(
        OUTPUT ${HELP_QHC_FILE}
        COMMAND ${QT6_QHELPGENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/mickey.qhcp -o ${HELP_QHC_FILE}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mickey.qhcp ${HELP_QCH_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating Mickey Qt Help collection (help.qhc)"
    )
    
    # Fix help.qhc path after generation (modifies file in place)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/help.qhc.fixed
        COMMAND chmod u+w ${HELP_QHC_FILE}
        COMMAND ${CMAKE_COMMAND} -DHELP_QHC_FILE="${HELP_QHC_FILE}" -DHELP_QCH_FILE="${HELP_QCH_FILE}" -P "${CMAKE_SOURCE_DIR}/cmake/fix_help_qhc.cmake"
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/help.qhc.fixed
        DEPENDS ${HELP_QHC_FILE}
        COMMENT "Fixing Mickey help.qhc path references"
    )
    
    # Custom target for help generation
    add_custom_target(mickey_help ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/help.qhc.fixed
    )
else()
    message(WARNING "Qt6 qhelpgenerator not found - Mickey help files will not be generated")
    # Create empty files to avoid installation errors
    add_custom_command(
        OUTPUT ${HELP_QCH_FILE} ${HELP_QHC_FILE}
        COMMAND ${CMAKE_COMMAND} -E touch ${HELP_QCH_FILE} ${HELP_QHC_FILE}
        COMMENT "Creating placeholder Mickey help files (qhelpgenerator not found)"
    )
    add_custom_target(mickey_help ALL
        DEPENDS ${HELP_QCH_FILE} ${HELP_QHC_FILE}
    )
endif()

# ============================================================================
# Installation
# ============================================================================

install(TARGETS mickey
    RUNTIME DESTINATION bin
)

# Install help files if they exist
install(FILES
    ${HELP_QHC_FILE}
    ${HELP_QCH_FILE}
    DESTINATION share/linuxtrack/help/mickey
    OPTIONAL
)

