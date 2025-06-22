# Qt5 GUI for LinuxTrack (Modernized from Qt4)
# =============================================

# Enable Qt5 automoc, autouic, and autorcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find required Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL)

# GUI source files (based on actual files in repository)
set(GUI_SOURCES
    # Main application
    main.cpp
    ltr_gui.cpp
    ltr_show.cpp
    
    # Device management (TrackIR focused)
    device_setup.cpp
    tir_prefs.cpp
    
    # Tracking and state management
    tracker.cpp
    ltr_state.cpp
    ltr_tracking.cpp
    
    # Helper windows
    help_view.cpp
    help_viewer.cpp
    log_view.cpp
    ltr_dev_help.cpp
    
    # Model and profile management
    ltr_model.cpp
    ltr_profiles.cpp
    profile_selector.cpp
    profile_setup.cpp
    
    # Utilities
    ltr_gui_prefs.cpp
    guardian.cpp
    
    # 3D visualization
    window.cpp
    glwidget.cpp
    objreader.cpp
    
    # Camera view and buffering
    buffering.cpp
    
    # Progress and other UI elements
    progress.cpp
    scp_form.cpp
    scurve.cpp
    scview.cpp
    
    # Hash utilities
    hashing.cpp
)

# TrackIR specific sources
if(HAVE_TRACKIR)
    list(APPEND GUI_SOURCES
        plugin_install.cpp
        extractor.cpp
        downloading.cpp
        dlfirmware.cpp
        wine_launcher.cpp
        wine_warn.cpp
        xplugin.cpp
    )
endif()

# GUI header files (Qt will auto-generate moc files)
set(GUI_HEADERS
    ltr_gui.h
    ltr_show.h
    device_setup.h
    tir_prefs.h
    tracker.h
    ltr_state.h
    ltr_tracking.h
    help_view.h
    help_viewer.h
    log_view.h
    ltr_dev_help.h
    ltr_model.h
    ltr_profiles.h
    profile_selector.h
    profile_setup.h
    ltr_gui_prefs.h
    guardian.h
    window.h
    glwidget.h
    buffering.h
    progress.h
    scp_form.h
    scurve.h
    scview.h
    hashing.h
    objreader.h
)

# TrackIR specific headers
if(HAVE_TRACKIR)
    list(APPEND GUI_HEADERS
        plugin_install.h
        extractor.h
        downloading.h
        dlfirmware.h
        wine_launcher.h
        wine_warn.h
        xplugin.h
    )
endif()

# UI files (Qt Designer)
set(GUI_UI_FILES
    ltr.ui
    ltr_gui.ui
    tir_setup.ui
    help_view.ui
    profile_selector.ui
    profile_setup.ui
    scurve.ui
    scview.ui
    scp_form.ui
    progress.ui
)

# TrackIR specific UI files
if(HAVE_TRACKIR)
    list(APPEND GUI_UI_FILES
        plugin_install.ui
        extractor.ui
        downloading.ui
        wine_warn.ui
        xplugin.ui
    )
endif()

# Resource files
set(GUI_RESOURCES
    ltr_rc.qrc
)

# Create Qt5 GUI executable
add_executable(ltr_gui_qt5_trackir
    ${GUI_SOURCES}
    ${GUI_RESOURCES}
)

# Link Qt5 libraries
target_link_libraries(ltr_gui_qt5_trackir
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    linuxtrack_core
    ${CMAKE_DL_LIBS}
)

# Include directories
target_include_directories(ltr_gui_qt5_trackir PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_BINARY_DIR}
)

# Set executable properties
set_target_properties(ltr_gui_qt5_trackir PROPERTIES
    OUTPUT_NAME "ltr_gui_qt5_trackir"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Create help directory and copy help files
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/help)

# Copy help files
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/help)
    file(GLOB HELP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/help/*")
    foreach(HELP_FILE ${HELP_FILES})
        configure_file(${HELP_FILE} ${CMAKE_CURRENT_BINARY_DIR}/help COPYONLY)
    endforeach()
endif()

# Install GUI
install(TARGETS ltr_gui_qt5_trackir
    RUNTIME DESTINATION bin
)

# Install help files
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/help)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/help
        DESTINATION share/linuxtrack
    )
endif()

# Install desktop file
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/linuxtrack.desktop)
    install(FILES linuxtrack.desktop
        DESTINATION share/applications
    )
endif()

# Install icon
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/linuxtrack.png)
    install(FILES linuxtrack.png
        DESTINATION share/pixmaps
    )
endif()

message(STATUS "Qt5 GUI configured for TrackIR support") 