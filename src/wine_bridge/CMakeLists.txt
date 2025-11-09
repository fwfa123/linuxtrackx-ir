# src/wine_bridge/CMakeLists.txt
# Wine bridge components

if(WINE_PLUGIN)
    include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)
    
    # Add subdirectories
    add_subdirectory(client)
    add_subdirectory(controller)
    add_subdirectory(tester)
    add_subdirectory(ft_client)
    add_subdirectory(ft_tester)
    add_subdirectory(views)
    
    # NSIS installer generation (if makensis is available)
    if(MAKENSIS_EXECUTABLE)
        message(STATUS "NSIS installer support: enabled (makensis found)")
        
        # Determine which NSIS script to use based on WINE64 support
        if(WINE64)
            set(NSIS_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ltr_wine64.nsi)
        else()
            set(NSIS_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ltr_wine.nsi)
        endif()
        
        # Collect dependencies - depend on component targets and their output files
        # This ensures all components are built before the installer
        set(INSTALLER_TARGET_DEPS)
        set(INSTALLER_FILE_DEPS ${NSIS_SCRIPT})
        
        # 64-bit components (always built when WINE_PLUGIN is enabled, if 64-bit libs found)
        # 64-bit Wine is easier to install, so we prioritize it
        if(WINE64)
            list(APPEND INSTALLER_TARGET_DEPS NPClient64)
            list(APPEND INSTALLER_FILE_DEPS ${CMAKE_CURRENT_BINARY_DIR}/client/NPClient64.dll.so)
            list(APPEND INSTALLER_TARGET_DEPS Tester64)
            list(APPEND INSTALLER_FILE_DEPS ${CMAKE_CURRENT_BINARY_DIR}/tester/Tester64.exe.so)
        endif()
        
        # 32-bit components (built when WINE_PLUGIN is enabled, matching Autotools behavior)
        if(WINE_PLUGIN)
            list(APPEND INSTALLER_TARGET_DEPS NPClient check_data)
            list(APPEND INSTALLER_FILE_DEPS 
                ${CMAKE_CURRENT_BINARY_DIR}/client/NPClient.dll.so
                ${CMAKE_CURRENT_BINARY_DIR}/client/check_data.exe.so
            )
            list(APPEND INSTALLER_TARGET_DEPS Tester)
            list(APPEND INSTALLER_FILE_DEPS ${CMAKE_CURRENT_BINARY_DIR}/tester/Tester.exe.so)
            list(APPEND INSTALLER_TARGET_DEPS Controller)
            list(APPEND INSTALLER_FILE_DEPS ${CMAKE_CURRENT_BINARY_DIR}/controller/Controller.exe.so)
            list(APPEND INSTALLER_TARGET_DEPS FreeTrackClient)
            list(APPEND INSTALLER_FILE_DEPS ${CMAKE_CURRENT_BINARY_DIR}/ft_client/FreeTrackClient.dll.so)
            list(APPEND INSTALLER_TARGET_DEPS ftc)
            list(APPEND INSTALLER_FILE_DEPS ${CMAKE_CURRENT_BINARY_DIR}/ft_tester/ftc.exe.so)
            list(APPEND INSTALLER_TARGET_DEPS TrackIR)
            list(APPEND INSTALLER_FILE_DEPS ${CMAKE_CURRENT_BINARY_DIR}/views/TrackIR.exe.so)
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

# Copy 64-bit components (always built when WINE_PLUGIN is enabled, if 64-bit libs found)
")
        
        # Add 64-bit components if available
        if(WINE64)
            file(APPEND ${INSTALLER_STAGING_SCRIPT} "
if [ -f \"\${BUILD_DIR}/client/NPClient64.dll.so\" ]; then
    cp \"\${BUILD_DIR}/client/NPClient64.dll.so\" \"\${STAGING_DIR}/client/NPClient64.dll.so\"
fi
if [ -f \"\${BUILD_DIR}/tester/Tester64.exe.so\" ]; then
    cp \"\${BUILD_DIR}/tester/Tester64.exe.so\" \"\${STAGING_DIR}/tester/Tester64.exe.so\"
fi
")
        endif()
        
        # Add 32-bit components if available (built when WINE_PLUGIN is enabled)
        if(WINE_PLUGIN)
            file(APPEND ${INSTALLER_STAGING_SCRIPT} "
# Copy 32-bit components (built when WINE_PLUGIN is enabled)
if [ -f \"\${BUILD_DIR}/client/NPClient.dll.so\" ]; then
    cp \"\${BUILD_DIR}/client/NPClient.dll.so\" \"\${STAGING_DIR}/client/NPClient.dll.so\"
fi
if [ -f \"\${BUILD_DIR}/client/check_data.exe.so\" ]; then
    cp \"\${BUILD_DIR}/client/check_data.exe.so\" \"\${STAGING_DIR}/client/check_data.exe.so\"
fi
if [ -f \"\${BUILD_DIR}/tester/Tester.exe.so\" ]; then
    cp \"\${BUILD_DIR}/tester/Tester.exe.so\" \"\${STAGING_DIR}/tester/Tester.exe.so\"
fi
")
        endif()
        
        # Add other 32-bit components if they exist
        if(WINE_PLUGIN)
            file(APPEND ${INSTALLER_STAGING_SCRIPT} "
if [ -f \"\${BUILD_DIR}/controller/Controller.exe.so\" ]; then
    cp \"\${BUILD_DIR}/controller/Controller.exe.so\" \"\${STAGING_DIR}/controller/Controller.exe.so\"
fi
if [ -f \"\${BUILD_DIR}/ft_client/FreeTrackClient.dll.so\" ]; then
    cp \"\${BUILD_DIR}/ft_client/FreeTrackClient.dll.so\" \"\${STAGING_DIR}/ft_client/FreeTrackClient.dll.so\"
fi
if [ -f \"\${BUILD_DIR}/ft_tester/ftc.exe.so\" ]; then
    cp \"\${BUILD_DIR}/ft_tester/ftc.exe.so\" \"\${STAGING_DIR}/ft_tester/ftc.exe.so\"
fi
if [ -f \"\${BUILD_DIR}/views/TrackIR.exe.so\" ]; then
    cp \"\${BUILD_DIR}/views/TrackIR.exe.so\" \"\${STAGING_DIR}/views/TrackIR.exe.so\"
fi

# Copy icon file if it exists
if [ -f \"${CMAKE_CURRENT_SOURCE_DIR}/controller/linuxtrack.ico\" ]; then
    cp \"${CMAKE_CURRENT_SOURCE_DIR}/controller/linuxtrack.ico\" \"\${STAGING_DIR}/controller/linuxtrack.ico\"
fi

# Create modified NSIS script that only includes files that exist
cp \"\${NSIS_SCRIPT}\" \"\${MODIFIED_SCRIPT}\"

# Comment out File commands for files that don't exist (before changing directory)
if [ ! -f \"\${STAGING_DIR}/controller/Controller.exe.so\" ]; then
    sed -i 's|^  File /oname=Controller.exe|  # File /oname=Controller.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/tester/Tester.exe.so\" ]; then
    sed -i 's|^  File /oname=Tester.exe|  # File /oname=Tester.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/tester/Tester64.exe.so\" ]; then
    sed -i 's|^  File /oname=Tester64.exe|  # File /oname=Tester64.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/views/TrackIR.exe.so\" ]; then
    sed -i 's|^  File /oname=TrackIR.exe|  # File /oname=TrackIR.exe|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/ft_client/FreeTrackClient.dll.so\" ]; then
    sed -i 's|^  File /oname=FreeTrackClient.dll|  # File /oname=FreeTrackClient.dll|' \"\${MODIFIED_SCRIPT}\"
fi
if [ ! -f \"\${STAGING_DIR}/ft_tester/ftc.exe.so\" ]; then
    sed -i 's|^  File /oname=FreeTrackTester.exe|  # File /oname=FreeTrackTester.exe|' \"\${MODIFIED_SCRIPT}\"
fi

# Change to staging directory for makensis (it needs to run from there)
cd \"\${STAGING_DIR}\" || exit 1

# Copy the modified NSIS script to staging directory
cp \"\${MODIFIED_SCRIPT}\" \"\${STAGING_DIR}/ltr_wine_modified.nsi\"

# Run makensis from staging directory
${MAKENSIS_EXECUTABLE} \"\${STAGING_DIR}/ltr_wine_modified.nsi\" || exit 1

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
        message(STATUS "NSIS installer support: disabled (makensis not found)")
        message(WARNING "Wine installer cannot be built without makensis. Install makensis to build the installer from components.")
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

