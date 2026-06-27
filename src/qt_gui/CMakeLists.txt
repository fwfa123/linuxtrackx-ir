# src/qt_gui/CMakeLists.txt
# Qt6 GUI application - native CMake Qt integration

# Enable Qt6 automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Copy config.h to build directory (needed for HAVE_CONFIG_H)
set(LOCAL_CONFIG_H ${CMAKE_CURRENT_BINARY_DIR}/config.h)
add_custom_command(
    OUTPUT ${LOCAL_CONFIG_H}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/config.h ${LOCAL_CONFIG_H}
    DEPENDS ${CMAKE_BINARY_DIR}/config.h
    COMMENT "Copying config.h to qt_gui build directory"
)

# Common source files (all platforms)
set(LTR_GUI_HEADERS
    ltr_gui.h
    ltr_show.h
    ltr_dev_help.h
    help_view.h
    ltr_gui_prefs.h
    prefs_link.h
    tir_prefs.h
    device_setup.h
    ltr_model.h
    window.h
    glwidget.h
    objreader.h
    ltr_profiles.h
    log_view.h
    ltr_state.h
    scp_form.h
    buffering.h
    progress.h
    scurve.h
    scview.h
    pose_monitor_strip.h
    pose_crosshair_widget.h
    pose_crosshair_panel.h
    wiimote_prefs.h
    tracker.h
    plugin_install.h
    profile_setup.h
    profile_selector.h
    guardian.h
    xplugin.h
    wine_warn.h
    extractor.h
    ../game_data.h
    hashing.h
    downloading.h
    wine_launcher.h
    macps3eye_prefs.h
    macwebcam_info.h
    ../ps3_prefs.h
    macps3eyeft_prefs.h
    help_viewer.h
    ../extract.h
    ../digest.h
    trackir_permission_dialog.h
    lutris_integration.h
    lutris_path_config_dialog.h
    steam_integration.h
    testing_section.h
    tester_launcher.h
    flatpak_detector.h
    installer_paths.h
    wine_bridge_install.h
    wine_executable.h
    wine_prefix_resolver.h
    about_dialog.h
    ../mickey/hotkey.h
    ../mickey/hotkey_setup_dlg.h
    ../mickey/my_line_edit.h
    ../mickey/keyb.h
)

set(LTR_GUI_FORMS
    ltr_gui.ui
    dev_help.ui
    ltr.ui
    model_creation.ui
    scurve.ui
    scp_form.ui
    logview.ui
    wii_setup.ui
    tir_setup.ui
    cap_edit.ui
    clip_edit.ui
    single_edit.ui
    model_edit.ui
    device_setup.ui
    profile_setup.ui
    profile_selector.ui
    cap_tweaking.ui
    clip_tweaking.ui
    extractor.ui
    progress.ui
    xplugin.ui
    wine_warn.ui
    m_ps3eye_setup.ui
    m_ps3eye_ft_setup.ui
    ../mickey/hotkey.ui
    ../mickey/hotkey_setup.ui
)

set(LTR_GUI_SOURCES
    main.cpp
    ltr_gui.cpp
    ltr_show.cpp
    ltr_dev_help.cpp
    ltr_gui_prefs.cpp
    tir_prefs.cpp
    help_view.cpp
    buffering.cpp
    ltr_model.cpp
    window.cpp
    glwidget.cpp
    objreader.cpp
    ltr_profiles.cpp
    log_view.cpp
    ltr_state.cpp
    scp_form.cpp
    guardian.cpp
    scurve.cpp
    scview.cpp
    pose_monitor_strip.cpp
    pose_crosshair_widget.cpp
    pose_crosshair_panel.cpp
    wiimote_prefs.cpp
    ../ltr_srv_master.cpp
    device_setup.cpp
    ../ltr_srv_slave.c
    ../ltr_srv_comm.c
    plugin_install.cpp
    profile_setup.cpp
    profile_selector.cpp
    xplugin.cpp
    wine_warn.cpp
    progress.cpp
    extractor.cpp
    ../game_data.c
    hashing.cpp
    downloading.cpp
    wine_launcher.cpp
    macps3eye_prefs.cpp
    macwebcam_info.cpp
    macps3eyeft_prefs.cpp
    help_viewer.cpp
    ../extract.c
    ../digest.c
    trackir_permission_dialog.cpp
    lutris_integration.cpp
    lutris_path_config_dialog.cpp
    steam_integration.cpp
    testing_section.cpp
    tester_launcher.cpp
    flatpak_detector.cpp
    installer_paths.cpp
    wine_bridge_install.cpp
    wine_executable.cpp
    wine_prefix_resolver.cpp
    about_dialog.cpp
    ../mickey/hotkey.cpp
    ../mickey/hotkey_setup_dlg.cpp
    ../mickey/my_line_edit.cpp
    ../mickey/keyb.cpp
)

# Platform-specific files
if(NOT APPLE)
    # Linux-specific files
    list(APPEND LTR_GUI_HEADERS
        webcam_prefs.h
        webcam_info.h
        webcam_ft_prefs.h
        joy_prefs.h
    )
    list(APPEND LTR_GUI_FORMS
        l_wc_setup.ui
        l_wcft_setup.ui
        joy_setup.ui
    )
    list(APPEND LTR_GUI_SOURCES
        webcam_info.cpp
        webcam_prefs.cpp
        webcam_ft_prefs.cpp
        joy_prefs.cpp
        ../mickey/keyb_x11.cpp
    )
else()
    # macOS-specific files
    list(APPEND LTR_GUI_HEADERS
        macwebcam_prefs.h
        macwebcamft_prefs.h
    )
    list(APPEND LTR_GUI_FORMS
        m_wc_setup.ui
        m_wcft_setup.ui
    )
    list(APPEND LTR_GUI_SOURCES
        macwebcamft_prefs.cpp
        macwebcam_prefs.cpp
        ../mickey/keyb.cpp
    )
endif()

# Create executable
add_executable(ltr_gui
    ${LOCAL_CONFIG_H}
    ${LTR_GUI_HEADERS}
    ${LTR_GUI_FORMS}
    ${LTR_GUI_SOURCES}
    ltr_rc.qrc
    precomp_headers.h
)

# Link Qt6 libraries
target_link_libraries(ltr_gui
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Network
    Qt6::Concurrent
    Qt6::Sql
    ltr
    ${MXML_LIBRARIES}
    ${MATH_LIBRARY}
)

# Link OpenGL and GLU libraries (for OpenGL utilities)
find_library(OPENGL_LIBRARY OpenGL)
find_library(GLU_LIBRARY GLU)
if(OPENGL_LIBRARY)
    target_link_libraries(ltr_gui ${OPENGL_LIBRARY})
else()
    message(WARNING "OpenGL library not found - some OpenGL features may not work")
endif()
if(GLU_LIBRARY)
    target_link_libraries(ltr_gui ${GLU_LIBRARY})
else()
    message(WARNING "GLU library not found - some OpenGL features may not work")
endif()

# Platform-specific linking
# Note: Qt6::X11Extras is not available in Qt6, X11 functionality is handled differently
if(NOT APPLE)
    target_link_libraries(ltr_gui
        X11
    )
endif()

# Compile definitions
target_compile_definitions(ltr_gui PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT6_OVERRIDES
    HAVE_CONFIG_H
    HELP_BASE="ltr_gui/"
)
# Match root WEBCAM_SUPPORT so the GUI always sees WEBCAM_SUPPORT even if the wrong config.h is picked up.
if(WEBCAM_SUPPORT)
    target_compile_definitions(ltr_gui PRIVATE WEBCAM_SUPPORT=1)
endif()

# Platform-specific compile definitions
if(NOT APPLE)
    target_compile_definitions(ltr_gui PRIVATE
        LTR_GUI
    )
endif()

# Include directories — put ${CMAKE_BINARY_DIR} first so generated config.h wins over any
# stale config.h elsewhere in the tree (e.g. legacy autotools file at repo root).
target_include_directories(ltr_gui PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Compile options
target_compile_options(ltr_gui PRIVATE
    -Wall
    -Wextra
)

# Platform-specific compile options
if(NOT APPLE)
    target_compile_options(ltr_gui PRIVATE
        -fvisibility=hidden
        -Wformat
        -Wformat-security
        --param ssp-buffer-size=4
        -fstack-protector
        -D_FORTIFY_SOURCE=1
    )
endif()

# Set RPATH so ltr_gui can find linuxtrack libraries at runtime
set_linuxtrack_rpath(ltr_gui)

# Copy help files to build dir so running from build finds them
add_custom_command(TARGET ltr_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/help ${CMAKE_CURRENT_BINARY_DIR}/help
    COMMENT "Copying help files to build directory"
)

# Precompiled header support
# Note: CMake 3.16+ supports target_precompile_headers, but we'll handle it manually
# for compatibility. The precomp_headers.h file will be included in sources.

# ============================================================================
# Installation
# ============================================================================

# Install executable
install(TARGETS ltr_gui
    RUNTIME DESTINATION bin
)

# Install data files
install(FILES
    sparow_opaq.obj
    sparow_glass.obj
    xm8_detail.png
    sources.txt
    spec.txt
    sphere.obj
    sphere.png
    sources_mfc.txt
    sources_mfc42.txt
    win7.reg
    win10.reg
    DESTINATION share/linuxtrack
)

# Install help HTML files (direct file help, no Qt Help compilation)
install(DIRECTORY help/
    DESTINATION share/linuxtrack/help/ltr_gui
    FILES_MATCHING PATTERN "*.htm" PATTERN "*.html" PATTERN "*.png"
)

