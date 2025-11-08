# src/qt_gui/CMakeLists.txt
# Qt5 GUI application - using qmake like Autotools does

# Find qmake
find_program(QMAKE_EXECUTABLE
    NAMES qmake-qt5 qt5-qmake qmake5 qmake
    REQUIRED
)

# Build Qt GUI using qmake
set(QMAKE_MAKEFILE ${CMAKE_CURRENT_BINARY_DIR}/qmake_Makefile)

add_custom_target(ltr_gui_qmake ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/config.h ${CMAKE_CURRENT_BINARY_DIR}/config.h
    COMMAND ${QMAKE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/ltr_gui.pro LIBDIR=${CMAKE_BINARY_DIR}/src -o ${QMAKE_MAKEFILE}
    COMMAND ${CMAKE_MAKE_PROGRAM} -f ${QMAKE_MAKEFILE}
    COMMENT "Building Qt GUI with qmake"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Install Qt GUI executable
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ltr_gui
    DESTINATION bin
)

# Install Qt GUI data files
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

# Install help files if they exist
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/help.qhc
    ${CMAKE_CURRENT_BINARY_DIR}/help.qch
    DESTINATION share/linuxtrack/help/ltr_gui
    OPTIONAL
)

