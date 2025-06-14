######################################################################
# LinuxTrack GUI - Qt5 Standalone Version
# This version builds without the main LinuxTrack library for GUI testing
######################################################################

QT += core widgets opengl help network sql

CONFIG += qt debug warn_on c++11
TEMPLATE = app
TARGET = ltr_gui_qt5_standalone
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ..

# Qt5 Resources
RESOURCES = ltr_rc.qrc

# Remove deprecated options and add Qt5-specific ones
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_DISABLE_DEPRECATED_BEFORE=0x050000

# Precompiled headers (optional in Qt5)
PRECOMPILED_HEADER = precomp_headers.h

# Compiler flags - Modern C++ and security features
QMAKE_CXXFLAGS += -Wall -Wextra -std=c++11 -DHAVE_CONFIG_H -DHELP_BASE="'\"ltr_gui/\"'" -DSTANDALONE_GUI
QMAKE_CFLAGS += -Wall -Wextra -DLTR_GUI -DSTANDALONE_GUI

# Security hardening
QMAKE_CXXFLAGS += -fvisibility=hidden -O2 -Wformat -Wformat-security -fstack-protector-strong -D_FORTIFY_SOURCE=2
QMAKE_CFLAGS += -fvisibility=hidden -O2 -Wformat -Wformat-security -fstack-protector-strong -D_FORTIFY_SOURCE=2

# Headers - Core GUI components
HEADERS += ltr_gui.h ltr_show.h ltr_dev_help.h help_view.h \
           ltr_gui_prefs.h prefs_link.h help_viewer.h \
           buffering.h window.h glwidget.h objreader.h \
           ltr_profiles.h log_view.h ltr_state.h scp_form.h \
           guardian.h scurve.h scview.h wiimote_prefs.h \
           device_setup.h plugin_install.h profile_setup.h \
           profile_selector.h xplugin.h wine_warn.h progress.h \
           extractor.h downloading.h wine_launcher.h \
           hashing.h tir_prefs.h ltr_model.h tracker.h

# Platform-specific headers
unix:!macx {
    HEADERS += webcam_prefs.h webcam_info.h webcam_ft_prefs.h joy_prefs.h
}

macx {
    HEADERS += macps3eye_prefs.h macwebcam_info.h ../ps3_prefs.h macps3eyeft_prefs.h \
               macwebcam_prefs.h macwebcamft_prefs.h
}

# Sources - Core GUI components  
SOURCES += main.cpp ltr_gui.cpp ltr_show.cpp ltr_dev_help.cpp \
           ltr_gui_prefs.cpp tir_prefs.cpp help_view.cpp \
           buffering.cpp ltr_model.cpp window.cpp glwidget.cpp \
           objreader.cpp ltr_profiles.cpp log_view.cpp ltr_state.cpp \
           scp_form.cpp guardian.cpp scurve.cpp scview.cpp \
           wiimote_prefs.cpp device_setup.cpp plugin_install.cpp \
           profile_setup.cpp profile_selector.cpp xplugin.cpp \
           wine_warn.cpp progress.cpp extractor.cpp downloading.cpp \
           wine_launcher.cpp help_viewer.cpp hashing.cpp

# Platform-specific sources
unix:!macx {
    SOURCES += webcam_info.cpp webcam_prefs.cpp webcam_ft_prefs.cpp joy_prefs.cpp
}

macx {
    SOURCES += macwebcam_info.cpp macwebcam_prefs.cpp macwebcamft_prefs.cpp \
               macps3eye_prefs.cpp macps3eyeft_prefs.cpp
}

# Standalone stubs - replace LinuxTrack library functions with stubs
SOURCES += ../standalone_stubs.c

# UI Forms
FORMS += ltr_gui.ui dev_help.ui ltr.ui model_creation.ui \
         scurve.ui scp_form.ui logview.ui wii_setup.ui \
         tir_setup.ui cap_edit.ui clip_edit.ui single_edit.ui \
         model_edit.ui device_setup.ui profile_setup.ui \
         profile_selector.ui cap_tweaking.ui clip_tweaking.ui \
         extractor.ui progress.ui xplugin.ui wine_warn.ui \
         l_wc_setup.ui l_wcft_setup.ui joy_setup.ui

# Libraries - Remove LinuxTrack dependency, keep system libs
LIBS += -lm -lGLU -lmxml -lGL -lpthread

# Build configuration
CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    TARGET = ltr_gui_qt5_standalone_debug
    message("Building LinuxTrack GUI Standalone with Qt5 (Debug)")
} else {
    TARGET = ltr_gui_qt5_standalone_release
    message("Building LinuxTrack GUI Standalone with Qt5 (Release)")
}

message("Qt version: $$[QT_VERSION]")
message("Target: $$TARGET") 