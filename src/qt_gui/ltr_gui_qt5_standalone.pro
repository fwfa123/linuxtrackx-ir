######################################################################
# LinuxTrack GUI - Qt5 Migration
# Modernized version of ltr_gui.pro for Qt5 compatibility
######################################################################

QT += core widgets opengl help network

CONFIG += qt debug warn_on c++11
TEMPLATE = app
TARGET = ltr_gui_qt5
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ..

# Qt5 Resources
RESOURCES = ltr_rc.qrc

# Remove deprecated options and add Qt5-specific ones
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_DISABLE_DEPRECATED_BEFORE=0x050000

# Precompiled headers (optional in Qt5)
PRECOMPILED_HEADER = precomp_headers.h

# Headers - Core GUI components
HEADERS += ltr_gui.h ltr_show.h ltr_dev_help.h help_view.h \
           ltr_gui_prefs.h prefs_link.h tir_prefs.h device_setup.h \
           ltr_model.h window.h glwidget.h objreader.h ltr_profiles.h \
           log_view.h ltr_state.h scp_form.h buffering.h progress.h \
           scurve.h scview.h wiimote_prefs.h tracker.h plugin_install.h \
           profile_setup.h profile_selector.h guardian.h xplugin.h wine_warn.h \
           extractor.h ../game_data.h hashing.h downloading.h wine_launcher.h \
           help_viewer.h ../extract.h ../digest.h

# Platform-specific headers
unix:!macx {
    HEADERS += webcam_prefs.h webcam_info.h webcam_ft_prefs.h joy_prefs.h
}

macx {
    HEADERS += macps3eye_prefs.h macwebcam_info.h ../ps3_prefs.h macps3eyeft_prefs.h \
               macwebcam_prefs.h macwebcamft_prefs.h
}

# UI Forms
FORMS += ltr_gui.ui dev_help.ui ltr.ui model_creation.ui scurve.ui scp_form.ui \
         logview.ui wii_setup.ui tir_setup.ui \
         cap_edit.ui clip_edit.ui single_edit.ui model_edit.ui \
         device_setup.ui profile_setup.ui profile_selector.ui cap_tweaking.ui clip_tweaking.ui \
         extractor.ui progress.ui xplugin.ui wine_warn.ui

# Platform-specific forms
unix:!macx {
    FORMS += l_wc_setup.ui l_wcft_setup.ui joy_setup.ui
}

macx {
    FORMS += m_ps3eye_setup.ui m_ps3eye_ft_setup.ui m_wc_setup.ui m_wcft_setup.ui
}

# Source files - Core components
SOURCES += main.cpp ltr_gui.cpp ltr_show.cpp ltr_dev_help.cpp \
           ltr_gui_prefs.cpp tir_prefs.cpp help_view.cpp buffering.cpp \
           ltr_model.cpp window.cpp glwidget.cpp objreader.cpp ltr_profiles.cpp \
           log_view.cpp ltr_state.cpp scp_form.cpp guardian.cpp \
           scurve.cpp scview.cpp wiimote_prefs.cpp \
           tracker.cpp ../ltr_srv_master.cpp device_setup.cpp \
           ../ltr_srv_slave.c ../ltr_srv_comm.c plugin_install.cpp profile_setup.cpp \
           profile_selector.cpp xplugin.cpp wine_warn.cpp progress.cpp \
           extractor.cpp ../game_data.c hashing.cpp downloading.cpp wine_launcher.cpp \
           help_viewer.cpp ../extract.c ../digest.c

# Platform-specific sources
unix:!macx {
    SOURCES += webcam_info.cpp webcam_prefs.cpp webcam_ft_prefs.cpp joy_prefs.cpp
}

macx {
    SOURCES += macps3eye_prefs.cpp macwebcam_info.cpp macps3eyeft_prefs.cpp \
               macwebcam_info.cpp macwebcamft_prefs.cpp macwebcam_prefs.cpp
}

# Compiler flags - Modern C++ and security features
QMAKE_CXXFLAGS += -Wall -Wextra -std=c++11 -DHAVE_CONFIG_H -DHELP_BASE="'\""ltr_gui/"\"'"
QMAKE_CFLAGS += -Wall -Wextra -DLTR_GUI

# Platform-specific configuration
unix:!macx {
    QMAKE_CXXFLAGS += -fvisibility=hidden -O2 -Wformat -Wformat-security \
                      -fstack-protector-strong -D_FORTIFY_SOURCE=2
    QMAKE_CFLAGS += -fvisibility=hidden -O2 -Wformat -Wformat-security \
                    -fstack-protector-strong -D_FORTIFY_SOURCE=2
    
    # Libraries - Updated for modern systems
    LIBS += -lm -lGLU -lmxml
    
    # Try to find linuxtrack library
    LIBS += -L../.libs -lltr
    
    # Installation paths
    target.path = /usr/local/bin
    data.path = /usr/local/share/linuxtrack
    data.files += sparow_opaq.obj sparow_glass.obj xm8_detail.png sources.txt spec.txt \
                  sphere.obj sphere.png sources_mfc.txt win7.reg
    help.path = /usr/local/share/linuxtrack/help/ltr_gui
    help.files += help.qhc help.qch 
    INSTALLS += target data help
}

macx {
    CONFIG += x86_64
    LIBS += -L../.libs -lm -lltr -lmxml
    
    data.path = ltr_gui.app/Contents/Resources/linuxtrack
    data.files += sparow_opaq.obj sparow_glass.obj xm8_detail.png ../linuxtrack1.conf \
                  sources_mac.txt spec.txt sphere.obj sphere.png sources_mfc.txt win7.reg
    help.path = ltr_gui.app/Contents/Resources/linuxtrack/help/ltr_gui
    help.files += help.qhc help.qch 
    INSTALLS += data help
    ICON = linuxtrack.icns
}

# Debug configuration
CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    TARGET = $$join(TARGET,,,_debug)
}

# Version information
VERSION = 0.99.20
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

message("Building LinuxTrack GUI with Qt5")
message("Qt version: $$[QT_VERSION]")
message("Target: $$TARGET") 