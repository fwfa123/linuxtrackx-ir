# src/qt_gui/CMakeLists.txt
# Qt5 GUI application - using qmake like Autotools does

# Find qmake
find_program(QMAKE_EXECUTABLE
    NAMES qmake-qt5 qt5-qmake qmake5 qmake
    REQUIRED
)

# Find qhelpgenerator and get its directory for PATH
# This ensures qhelpgenerator is found when qmake-generated Makefile runs
find_program(QT5_QHELPGENERATOR
    NAMES qhelpgenerator-qt5 qhelpgenerator
)

# Enhanced cross-distro Qt5 bin directory detection
if(NOT QT5_BIN_DIR)
    # Method 1: Get Qt5 bin directory from qmake (most reliable cross-distro method)
    execute_process(
        COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_BINS
        OUTPUT_VARIABLE QT5_BINS_QUERY
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(QT5_BINS_QUERY AND EXISTS "${QT5_BINS_QUERY}")
        set(QT5_BIN_DIR "${QT5_BINS_QUERY}")
        message(STATUS "Qt5 bin directory found via qmake query: ${QT5_BIN_DIR}")
    endif()
endif()

if(NOT QT5_BIN_DIR AND QT5_QHELPGENERATOR)
    # Method 2: Get directory from found qhelpgenerator (works across all distros)
    get_filename_component(QT5_BIN_DIR ${QT5_QHELPGENERATOR} DIRECTORY)
    message(STATUS "Qt5 bin directory found via qhelpgenerator location: ${QT5_BIN_DIR}")
endif()

if(NOT QT5_BIN_DIR)
    # Method 3: Get directory from qmake executable itself
    get_filename_component(QMAKE_DIR ${QMAKE_EXECUTABLE} DIRECTORY)
    if(EXISTS "${QMAKE_DIR}/qhelpgenerator" OR EXISTS "${QMAKE_DIR}/qhelpgenerator-qt5")
        set(QT5_BIN_DIR "${QMAKE_DIR}")
        message(STATUS "Qt5 bin directory found via qmake location: ${QT5_BIN_DIR}")
    endif()
endif()

if(NOT QT5_BIN_DIR)
    # Method 4: Try common distribution-specific paths
    # These are used as fallback when automatic detection fails
    foreach(COMMON_PATH
        "/usr/lib64/qt5/bin"      # Fedora/RHEL 64-bit
        "/usr/lib/qt5/bin"        # Fedora/RHEL 32-bit, Arch Linux, openSUSE
        "/usr/lib/x86_64-linux-gnu/qt5/bin"  # Debian/Ubuntu 64-bit
        "/usr/lib/i386-linux-gnu/qt5/bin"    # Debian/Ubuntu 32-bit
        "/opt/qt5/bin"             # Some custom installations
        "/usr/local/lib/qt5/bin"   # Local Qt5 installations
    )
        if(EXISTS "${COMMON_PATH}/qhelpgenerator" OR EXISTS "${COMMON_PATH}/qhelpgenerator-qt5")
            set(QT5_BIN_DIR "${COMMON_PATH}")
            message(STATUS "Qt5 bin directory found via common path fallback: ${QT5_BIN_DIR}")
            break()
        endif()
    endforeach()
endif()

# Verify qhelpgenerator exists in the detected directory
if(QT5_BIN_DIR)
    if(EXISTS "${QT5_BIN_DIR}/qhelpgenerator-qt5")
        set(QT5_QHELPGENERATOR "${QT5_BIN_DIR}/qhelpgenerator-qt5")
    elseif(EXISTS "${QT5_BIN_DIR}/qhelpgenerator")
        set(QT5_QHELPGENERATOR "${QT5_BIN_DIR}/qhelpgenerator")
    endif()
endif()

# Report Qt5 tool detection results
if(QT5_QHELPGENERATOR)
    message(STATUS "Qt5 qhelpgenerator found: ${QT5_QHELPGENERATOR}")
else()
    message(WARNING "Qt5 qhelpgenerator not found. Help file generation will fail. Install qt5-qttools or qt5-tools package.")
endif()

# Build Qt GUI using qmake
set(QMAKE_MAKEFILE ${CMAKE_CURRENT_BINARY_DIR}/qmake_Makefile)

# Prepare environment for qmake/make execution
# Pass QT5_QHELPGENERATOR as environment variable so .pro file can use it via $$(QT5_QHELPGENERATOR)
# Also ensure PATH includes Qt5 bin directory for all subprocesses
set(QMAKE_ENV_VARS "")
if(QT5_QHELPGENERATOR)
    set(QMAKE_ENV_VARS "QT5_QHELPGENERATOR=${QT5_QHELPGENERATOR}")
endif()

# Always include Qt5 bin directory in PATH if found, even as fallback
if(QT5_BIN_DIR)
    if(QMAKE_ENV_VARS)
        set(QMAKE_ENV_VARS "${QMAKE_ENV_VARS};PATH=${QT5_BIN_DIR}:$ENV{PATH}")
    else()
        set(QMAKE_ENV_VARS "PATH=${QT5_BIN_DIR}:$ENV{PATH}")
    endif()
    # Create a PATH prefix that can be used with bash -c to ensure subprocesses inherit the PATH
    set(QT5_PATH_PREFIX "PATH=${QT5_BIN_DIR}:$ENV{PATH} ")
    message(STATUS "Qt5 tools PATH configured: ${QT5_BIN_DIR}")
else()
    set(QT5_PATH_PREFIX "")
    if(NOT QT5_QHELPGENERATOR)
        message(WARNING "Qt5 bin directory not found. Qt5 tools may not be accessible during build.")
    endif()
endif()

# Build Qt GUI using qmake
# Commands run in sequence: copy config.h -> run qmake -> run make -> fix help files
add_custom_target(ltr_gui_qmake ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/config.h ${CMAKE_CURRENT_BINARY_DIR}/config.h
    COMMAND ${CMAKE_COMMAND} -E echo "Running qmake to generate Makefile..."
)

# Run qmake (after config.h is copied)
# Always set environment variables to ensure Qt5 tools are accessible
add_custom_command(TARGET ltr_gui_qmake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env ${QMAKE_ENV_VARS} ${QMAKE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/ltr_gui.pro LIBDIR=${CMAKE_BINARY_DIR}/src -o ${QMAKE_MAKEFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Makefile with qmake"
)

# Run make (after qmake generates Makefile)
# Use env command to ensure environment variables are properly inherited by all subprocesses
if(QMAKE_ENV_VARS)
    add_custom_command(TARGET ltr_gui_qmake POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env ${QMAKE_ENV_VARS} ${CMAKE_MAKE_PROGRAM} -f ${QMAKE_MAKEFILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Building Qt GUI with make"
    )
else()
    add_custom_command(TARGET ltr_gui_qmake POST_BUILD
        COMMAND ${CMAKE_MAKE_PROGRAM} -f ${QMAKE_MAKEFILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Building Qt GUI with make"
    )
endif()

# Fix help.qhc path after generation (after make completes)
add_custom_command(TARGET ltr_gui_qmake POST_BUILD
    COMMAND chmod u+w ${CMAKE_CURRENT_BINARY_DIR}/help.qhc
    COMMAND ${CMAKE_COMMAND} -DHELP_QHC_FILE="${CMAKE_CURRENT_BINARY_DIR}/help.qhc" -DHELP_QCH_FILE="${CMAKE_CURRENT_BINARY_DIR}/help.qch" -P "${CMAKE_SOURCE_DIR}/cmake/fix_help_qhc.cmake"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Fixing help file paths"
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
# Note: We install both files, then fix the path in help.qhc after installation
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/help.qhc
    ${CMAKE_CURRENT_BINARY_DIR}/help.qch
    DESTINATION share/linuxtrack/help/ltr_gui
    OPTIONAL
)

# Note: help.qhc path is fixed during build (in ltr_gui_qmake target)
# No need to fix during installation since it's already corrected

