# src/wine_bridge/CMakeLists.txt
# Wine bridge components

if(WINE_PLUGIN)
    include(${CMAKE_SOURCE_DIR}/cmake/MinGWCompiler.cmake)
    
    # Add subdirectories
    add_subdirectory(client)
    add_subdirectory(controller)
    add_subdirectory(tester)
    add_subdirectory(ft_client)
    add_subdirectory(ft_tester)
    add_subdirectory(views)
    
    # NSIS installer generation (requires makensis and Stubs — see install_nsis_arch.sh)
    if(MAKENSIS_EXECUTABLE AND NSIS_HAVE_STUBS)
        message(STATUS "NSIS installer support: enabled (makensis + stubs)")
        
        # Set variables for NSIS template configuration
        set(srcdir ${CMAKE_CURRENT_SOURCE_DIR})

        # Configure NSIS templates
        if(WINE64)
            configure_file(
                ${CMAKE_CURRENT_SOURCE_DIR}/ltr_wine64.nsi.in
                ${CMAKE_CURRENT_BINARY_DIR}/ltr_wine64.nsi
                @ONLY
            )
            set(NSIS_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/ltr_wine64.nsi)
        else()
            configure_file(
                ${CMAKE_CURRENT_SOURCE_DIR}/ltr_wine.nsi.in
                ${CMAKE_CURRENT_BINARY_DIR}/ltr_wine.nsi
                @ONLY
            )
            set(NSIS_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/ltr_wine.nsi)
        endif()
        
        # Collect dependencies - depend on component targets and their output files
        # This ensures all components are built before the installer
        set(INSTALLER_TARGET_DEPS)
        set(INSTALLER_FILE_DEPS ${NSIS_SCRIPT})
        
        if(WINE_PLUGIN)
            list(APPEND INSTALLER_TARGET_DEPS NPClient NPClient64 check_data Controller Tester Tester64 FreeTrackClient ftc TrackIR)
            list(APPEND INSTALLER_FILE_DEPS 
                ${CMAKE_CURRENT_BINARY_DIR}/client/NPClient.dll
                ${CMAKE_CURRENT_BINARY_DIR}/client/NPClient64.dll
                ${CMAKE_CURRENT_BINARY_DIR}/client/check_data.exe
                ${CMAKE_CURRENT_BINARY_DIR}/controller/Controller.exe
                ${CMAKE_CURRENT_BINARY_DIR}/tester/Tester.exe
                ${CMAKE_CURRENT_BINARY_DIR}/tester/Tester64.exe
                ${CMAKE_CURRENT_BINARY_DIR}/ft_client/FreeTrackClient.dll
                ${CMAKE_CURRENT_BINARY_DIR}/ft_tester/ftc.exe
                ${CMAKE_CURRENT_BINARY_DIR}/views/TrackIR.exe
            )
        endif()
        
        # Build the installer using makensis - always rebuild from all built components
        set(INSTALLER_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/linuxtrack-wine.exe)
        
        # Create a staging directory and copy only existing component files
        # This allows the installer to build with available components
        set(INSTALLER_STAGING_DIR ${CMAKE_CURRENT_BINARY_DIR}/installer_staging)
        set(INSTALLER_STAGING_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/prepare_installer_files.sh)
        set(MODIFIED_NSIS_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/ltr_wine_modified.nsi)
        
        # Create script to prepare installer files and modify NSIS script
        file(WRITE ${INSTALLER_STAGING_SCRIPT} "#!/bin/bash
set -e
STAGING_DIR=\"${INSTALLER_STAGING_DIR}\"
NSIS_SCRIPT=\"${NSIS_SCRIPT}\"
MODIFIED_SCRIPT=\"${MODIFIED_NSIS_SCRIPT}\"
BUILD_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\"

# Create staging directory structure
mkdir -p \"\${STAGING_DIR}/client\"
mkdir -p \"\${STAGING_DIR}/tester\"
mkdir -p \"\${STAGING_DIR}/controller\"
mkdir -p \"\${STAGING_DIR}/ft_client\"
mkdir -p \"\${STAGING_DIR}/ft_tester\"
mkdir -p \"\${STAGING_DIR}/views\"

# Copy built components
")

        if(WINE_PLUGIN)
            file(APPEND ${INSTALLER_STAGING_SCRIPT} "
if [ -f \"\${BUILD_DIR}/client/NPClient.dll\" ]; then
    cp \"\${BUILD_DIR}/client/NPClient.dll\" \"\${STAGING_DIR}/client/NPClient.dll\"
fi
if [ -f \"\${BUILD_DIR}/client/NPClient64.dll\" ]; then
    cp \"\${BUILD_DIR}/client/NPClient64.dll\" \"\${STAGING_DIR}/client/NPClient64.dll\"
fi
if [ -f \"\${BUILD_DIR}/client/check_data.exe\" ]; then
    cp \"\${BUILD_DIR}/client/check_data.exe\" \"\${STAGING_DIR}/client/check_data.exe\"
fi
if [ -f \"\${BUILD_DIR}/tester/Tester.exe\" ]; then
    cp \"\${BUILD_DIR}/tester/Tester.exe\" \"\${STAGING_DIR}/tester/Tester.exe\"
fi
if [ -f \"\${BUILD_DIR}/tester/Tester64.exe\" ]; then
    cp \"\${BUILD_DIR}/tester/Tester64.exe\" \"\${STAGING_DIR}/tester/Tester64.exe\"
fi
if [ -f \"\${BUILD_DIR}/controller/Controller.exe\" ]; then
    cp \"\${BUILD_DIR}/controller/Controller.exe\" \"\${STAGING_DIR}/controller/Controller.exe\"
fi
if [ -f \"\${BUILD_DIR}/ft_client/FreeTrackClient.dll\" ]; then
    cp \"\${BUILD_DIR}/ft_client/FreeTrackClient.dll\" \"\${STAGING_DIR}/ft_client/FreeTrackClient.dll\"
fi
if [ -f \"\${BUILD_DIR}/ft_tester/ftc.exe\" ]; then
    cp \"\${BUILD_DIR}/ft_tester/ftc.exe\" \"\${STAGING_DIR}/ft_tester/ftc.exe\"
fi
if [ -f \"\${BUILD_DIR}/views/TrackIR.exe\" ]; then
    cp \"\${BUILD_DIR}/views/TrackIR.exe\" \"\${STAGING_DIR}/views/TrackIR.exe\"
fi

# Copy icon file if it exists (to both controller subdirectory and staging root for Icon directive)
if [ -f \"${CMAKE_CURRENT_SOURCE_DIR}/controller/linuxtrack.ico\" ]; then
    cp \"${CMAKE_CURRENT_SOURCE_DIR}/controller/linuxtrack.ico\" \"\${STAGING_DIR}/controller/linuxtrack.ico\"
    # Also copy to staging root for Icon directive (makensis runs from staging directory)
    cp \"${CMAKE_CURRENT_SOURCE_DIR}/controller/linuxtrack.ico\" \"\${STAGING_DIR}/linuxtrack.ico\"
fi

# Create modified NSIS script that only includes files that exist
cp \"\${NSIS_SCRIPT}\" \"\${MODIFIED_SCRIPT}\"

# Comment out Icon directive if icon file doesn't exist
# Note: We'll use relative path \"linuxtrack.ico\" since makensis runs from staging directory
if [ ! -f \"\${STAGING_DIR}/linuxtrack.ico\" ] ; then
    sed -i 's|^Icon \".*\"|# Icon \"linuxtrack.ico\"|' \"\${MODIFIED_SCRIPT}\"
fi

# Comment out File commands for files that don't exist (before changing directory)
if [ ! -f \"\${STAGING_DIR}/controller/Controller.exe\" ] ; then
    sed -i 's|^  File controller\\\\Controller.exe|  # File controller\\\\Controller.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/tester/Tester.exe\" ] ; then
    sed -i 's|^  File tester\\\\Tester.exe|  # File tester\\\\Tester.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/tester/Tester64.exe\" ] ; then
    sed -i 's|^  File tester\\\\Tester64.exe|  # File tester\\\\Tester64.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/client/NPClient64.dll\" ] ; then
    sed -i 's|^  File client\\\\NPClient64.dll|  # File client\\\\NPClient64.dll|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/views/TrackIR.exe\" ] ; then
    sed -i 's|^  File views\\\\TrackIR.exe|  # File views\\\\TrackIR.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/ft_client/FreeTrackClient.dll\" ] ; then
    sed -i 's|^  File ft_client\\\\FreeTrackClient.dll|  # File ft_client\\\\FreeTrackClient.dll|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/ft_tester/ftc.exe\" ] ; then
    sed -i 's|^  File /oname=FreeTrackTester.exe|  # File /oname=FreeTrackTester.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/controller/linuxtrack.ico\" ] ; then
    sed -i 's|^  File /oname=linuxtrack.ico|  # File /oname=linuxtrack.ico|' \"\${MODIFIED_SCRIPT}\"
fi

# Change to staging directory for makensis (it needs to run from there)
cd \"\${STAGING_DIR}\" || exit 1

# Copy the modified NSIS script to staging directory
cp \"\${MODIFIED_SCRIPT}\" \"\${STAGING_DIR}/ltr_wine_modified.nsi\"

# Verify icon file is present before running makensis
if [ ! -f \"linuxtrack.ico\" ] ; then
    echo \"Warning: linuxtrack.ico not found in staging directory - Icon directive will be commented out\" >&2
fi

# Run makensis from staging directory
# Icon directive uses relative path linuxtrack.ico which will be found in current directory
${MAKENSIS_EXECUTABLE} ltr_wine_modified.nsi || exit 1

# Move the built installer to the expected output location
if [ -f \"\${STAGING_DIR}/linuxtrack-wine.exe\" ]; then
    mv \"\${STAGING_DIR}/linuxtrack-wine.exe\" \"${INSTALLER_OUTPUT}\" || exit 1
else
    echo \"Error: linuxtrack-wine.exe not found in staging directory\" >&2
    exit 1
fi
")
        endif()
        
        # Build installer with makensis - use staging directory and modified script
        # The script creates the staging directory, changes into it, runs makensis, and moves the output
        add_custom_command(
            OUTPUT ${INSTALLER_OUTPUT}
            COMMAND chmod +x ${INSTALLER_STAGING_SCRIPT}
            COMMAND ${INSTALLER_STAGING_SCRIPT}
            DEPENDS ${INSTALLER_TARGET_DEPS} ${INSTALLER_FILE_DEPS} ${NSIS_SCRIPT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Building Wine installer with NSIS from all built components"
        )
        
        add_custom_target(wine_installer ALL
            DEPENDS ${INSTALLER_OUTPUT} ${INSTALLER_TARGET_DEPS}
        )
        
        # Install the built installer
        install(FILES
            ${INSTALLER_OUTPUT}
            DESTINATION share/linuxtrack/wine
            OPTIONAL
        )
        message(STATUS "Wine installer will be built from components and installed to: share/linuxtrack/wine/linuxtrack-wine.exe")
    else()
        if(MAKENSIS_EXECUTABLE)
            message(STATUS "NSIS installer support: disabled (NSIS stubs not found)")
            message(WARNING "Wine installer cannot be built without NSIS stubs. Arch/CachyOS: yay -S nsis, or ./scripts/install/install_nsis_arch.sh --force")
        else()
            message(STATUS "NSIS installer support: disabled (makensis not found)")
            message(WARNING "Wine installer cannot be built without makensis. Install full NSIS (with Stubs) to build linuxtrack-wine.exe.")
        endif()
    endif()
    
    # Install Wine bridge README
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/README.wine)
        install(FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/README.wine
            DESTINATION share/linuxtrack/wine
            OPTIONAL
        )
    endif()
endif()

