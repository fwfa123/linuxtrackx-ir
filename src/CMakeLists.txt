# src/CMakeLists.txt
# Core libraries and binaries for LinuxTrack

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
)

# Define HAVE_CONFIG_H for all targets
add_definitions(-DHAVE_CONFIG_H)

# Add config.h to include path
include_directories(${CMAKE_BINARY_DIR})

# Library path define
add_definitions(-DLIB_PATH="${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/")

# Flex/Bison integration for preferences parser
set(BISON_OUTPUT_DIR ${CMAKE_BINARY_DIR}/src)
set(FLEX_OUTPUT_DIR ${CMAKE_BINARY_DIR}/src)

# Ensure output directory exists
file(MAKE_DIRECTORY ${BISON_OUTPUT_DIR})
file(MAKE_DIRECTORY ${FLEX_OUTPUT_DIR})

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Generate Bison output
add_custom_command(
    OUTPUT ${BISON_OUTPUT_DIR}/pref_bison.cpp ${BISON_OUTPUT_DIR}/pref_bison.hpp
    COMMAND ${BISON_EXECUTABLE} -d --name-prefix=ltr_int_parser_ -o ${BISON_OUTPUT_DIR}/pref_bison.cpp ${CMAKE_CURRENT_SOURCE_DIR}/pref_bison.ypp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pref_bison.ypp
    COMMENT "Generating pref_bison.cpp and pref_bison.hpp with bison"
)

# Generate Flex output
add_custom_command(
    OUTPUT ${FLEX_OUTPUT_DIR}/pref_flex.cpp
    COMMAND ${FLEX_EXECUTABLE} -i ${CMAKE_CURRENT_SOURCE_DIR}/pref_flex.lpp
    COMMAND mv lex.yy.c ${FLEX_OUTPUT_DIR}/pref_flex.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pref_flex.lpp
    COMMENT "Generating pref_flex.cpp with flex"
)

# Add generated files to include path
include_directories(${BISON_OUTPUT_DIR})
include_directories(${FLEX_OUTPUT_DIR})

# Create interface for generated files
add_custom_target(pref_parser_generated 
    DEPENDS ${BISON_OUTPUT_DIR}/pref_bison.cpp ${BISON_OUTPUT_DIR}/pref_bison.hpp ${FLEX_OUTPUT_DIR}/pref_flex.cpp
)

# ============================================================================
# Core Libraries
# ============================================================================

# liblinuxtrack: small public shim used by client utilities
add_library(linuxtrack SHARED
    ltlib.c ltlib.h
    utils.c utils.h
    ipc_utils.c ipc_utils.h
    linuxtrack.h
)

set_target_properties(linuxtrack PROPERTIES
    VERSION 0.0.0
    SOVERSION 0
    OUTPUT_NAME "linuxtrack"
)

# Export symbols from liblt.sym if it exists
# TODO: Implement proper symbol export compatible with libtool's -export-symbols
# For now, disable to get build working - shared libraries export symbols by default
# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/liblt.sym)
#     target_link_options(linuxtrack PRIVATE
#         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/liblt.sym
#     )
# endif()

# 32-bit version of liblinuxtrack for Wine bridge compatibility
if(LTR32ON64)
    add_library(linuxtrack32 SHARED
        ltlib.c ltlib.h
        utils.c utils.h
        ipc_utils.c ipc_utils.h
        linuxtrack.h
    )
    
    set_target_properties(linuxtrack32 PROPERTIES
        VERSION 0.0.0
        SOVERSION 0
        OUTPUT_NAME "linuxtrack32"
    )
    
    target_compile_options(linuxtrack32 PRIVATE
        -m32
    )
    
    target_link_options(linuxtrack32 PRIVATE
        -m32
        -Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib/linuxtrack
    )
    
    # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/liblt.sym)
    #     target_link_options(linuxtrack32 PRIVATE
    #         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/liblt.sym
    #     )
    # endif()
endif()

# libltr: main engine
add_library(ltr SHARED
    cal.c cal.h
    list.c list.h
    dyn_load.c dyn_load.h
    math_utils.c math_utils.h
    pose.c pose.h
    pref.cpp pref.hpp pref.h
    ${BISON_OUTPUT_DIR}/pref_bison.cpp
    ${FLEX_OUTPUT_DIR}/pref_flex.cpp
    pref_global.c pref_global.h
    utils.c utils.h
    image_process.c image_process.h
    tracking.c tracking.h
    ltlib_int.c ltlib_int.h
    spline.c spline.h
    axis.c axis.h
    wii_driver_prefs.c wii_driver_prefs.h
    tir_driver_prefs.c tir_driver_prefs.h
    wc_driver_prefs.c wc_driver_prefs.h
    joy_driver_prefs.c joy_driver_prefs.h
    ipc_utils.c ipc_utils.h
    com_proc.c com_proc.h
    wii_com.c wii_com.h
    ps3_prefs.c ps3_prefs.h
)

add_dependencies(ltr pref_parser_generated)

set_target_properties(ltr PROPERTIES
    VERSION 0.0.0
    SOVERSION 0
    OUTPUT_NAME "ltr"
)

target_link_libraries(ltr
    ${MATH_LIBRARY}
    ${PTHREAD_LIBRARY}
    dl
)

# Export symbols matching '^ltr_int'
# TODO: Implement proper symbol export
# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ltr.sym)
#     target_link_options(ltr PRIVATE
#         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/ltr.sym
#     )
# endif()

# libtir: TrackIR USB driver
add_library(tir SHARED
    tir_hw.c
    tir_img.c
    tir_driver.c
    runloop.c
)

set_target_properties(tir PROPERTIES
    VERSION 0.0.0
    SOVERSION 0
    OUTPUT_NAME "tir"
)

target_link_libraries(tir
    ltr
    ${ZLIB_LIBRARIES}
)

# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tir.sym)
#     target_link_options(tir PRIVATE
#         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/tir.sym
#     )
# endif()

# libjoy: Joystick helper
add_library(joy SHARED
    runloop.c
    joy.c
)

set_target_properties(joy PROPERTIES
    VERSION 0.0.0
    SOVERSION 0
    OUTPUT_NAME "joy"
)

# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/joy_driver.sym)
#     target_link_options(joy PRIVATE
#         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/joy_driver.sym
#     )
# endif()

# libltusb1: USB interface helper used by TrackIR driver
add_library(ltusb1 SHARED
    libusb_ifc.c
)

set_target_properties(ltusb1 PROPERTIES
    VERSION 0.0.0
    SOVERSION 0
    OUTPUT_NAME "ltusb1"
)

target_link_libraries(ltusb1
    ltr
    ${USB_LIBRARIES}
)

# libwc: V4L2 webcam/face-tracking driver (conditional)
if(WEBCAM_SUPPORT AND WITH_LIBV4L2)
    add_library(wc SHARED
        webcam_driver.c
        runloop.c
    )

    if(FACE_TRACKER_SUPPORT)
        target_sources(wc PRIVATE facetrack.cpp)
        target_include_directories(wc PRIVATE ${OPENCV_INCLUDE_DIRS})
        target_compile_options(wc PRIVATE ${OPENCV_CFLAGS_OTHER})
        target_compile_definitions(wc PRIVATE OPENCV)
    endif()
    
    set_target_properties(wc PROPERTIES
        VERSION 0.0.0
        SOVERSION 0
        OUTPUT_NAME "wc"
    )
    
    target_link_libraries(wc
        ltr
        ${LIBV4L2_LIBRARY}
    )
    if(FACE_TRACKER_SUPPORT)
        # facetrack.cpp: CascadeClassifier only (core/imgproc/objdetect).
        # ${OPENCV_LIBRARIES} from opencv4.pc lists every module (HDF5/VTK, etc.)
        # and breaks AppImage dlopen when those optional libs are not bundled.
        target_link_directories(wc PRIVATE ${OPENCV_LIBRARY_DIRS})
        target_link_libraries(wc opencv_core opencv_imgproc opencv_objdetect)
    endif()
    
    # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/webcam_driver.sym)
    #     target_link_options(wc PRIVATE
    #         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/webcam_driver.sym
    #     )
    # endif()
endif()

# libp3e / libp3eft: PS3 Eye USB plugins (loaded by cal.c; not used on macOS build)
if(NOT APPLE)
    add_library(p3e SHARED
        ps3eye_driver.c
        runloop.c
    )

    set_target_properties(p3e PROPERTIES
        VERSION 0.0.0
        SOVERSION 0
        OUTPUT_NAME "p3e"
    )

    # libltusb1 is loaded at runtime via ltr_int_load_library; ps3eye_driver fills
    # ltr_int_find_p3e (and other usb_ifc pointers) from dlsym — do not link ltusb1
    # into libp3e or the loader may bind the wrong symbol type vs. pointer slots.
    target_link_libraries(p3e
        ltr
        ${MATH_LIBRARY}
    )

    if(FACE_TRACKER_SUPPORT)
        add_library(p3eft SHARED
            ps3eye_driver.c
            runloop.c
            facetrack.cpp
        )

        target_include_directories(p3eft PRIVATE ${OPENCV_INCLUDE_DIRS})
        target_compile_options(p3eft PRIVATE ${OPENCV_CFLAGS_OTHER})
        target_compile_definitions(p3eft PRIVATE OPENCV)

        set_target_properties(p3eft PROPERTIES
            VERSION 0.0.0
            SOVERSION 0
            OUTPUT_NAME "p3eft"
        )

        target_link_libraries(p3eft
            ltr
            ${MATH_LIBRARY}
        )
        target_link_directories(p3eft PRIVATE ${OPENCV_LIBRARY_DIRS})
        target_link_libraries(p3eft opencv_core opencv_imgproc opencv_objdetect)
    endif()
endif()

# ============================================================================
# Helper Functions
# ============================================================================

# Helper function to set RPATH for executables linking against linuxtrack libraries
# This ensures all executables can find liblinuxtrack.so.0 at runtime
# Usage: set_linuxtrack_rpath(target_name)
function(set_linuxtrack_rpath target)
    set_target_properties(${target} PROPERTIES
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endfunction()

# ============================================================================
# Binaries
# ============================================================================

# ltr_server1 daemon
add_executable(ltr_server1
    ltr_srv_comm.c ltr_srv_comm.h
    ltr_srv_slave.c ltr_srv_slave.h
    ltr_srv_master.cpp ltr_srv_master.h
    ltr_server1.c
)

target_link_libraries(ltr_server1
    ltr
    ${PTHREAD_LIBRARY}
)

# Set RPATH so ltr_server1 can find libltr.so at runtime
# This is critical when the slave process is spawned from Wine context
set_linuxtrack_rpath(ltr_server1)

# ltr_pipe CLI
add_executable(ltr_pipe
    ltr_pipe.c
    linuxtrack.c linuxtrack.h
)

target_link_libraries(ltr_pipe
    linuxtrack
    ltr
    dl
)

# Set RPATH so ltr_pipe can find liblinuxtrack.so.0 at runtime
set_linuxtrack_rpath(ltr_pipe)

# ltr_extractor CLI
add_executable(ltr_extractor
    hashing.c
    game_data.c game_data.h
    extract.c extract.h
    digest.c digest.h
)

target_link_libraries(ltr_extractor
    linuxtrack
    ltr
    ${MXML_LIBRARIES}
)

# Set RPATH so ltr_extractor can find liblinuxtrack.so.0 at runtime
set_linuxtrack_rpath(ltr_extractor)

# ltr_recenter helper
add_executable(ltr_recenter
    ltr_recenter.c
    ltr_srv_comm.c ltr_srv_comm.h
)

target_link_libraries(ltr_recenter
    linuxtrack
    ltr
    dl
)

# Set RPATH so ltr_recenter can find liblinuxtrack.so.0 at runtime
set_linuxtrack_rpath(ltr_recenter)

# ltr_udp (Bridge for X4: Foundations / OpenTrack games)
add_executable(ltr_udp ltr_udp.c linuxtrack.c)
target_include_directories(ltr_udp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ..)
target_link_libraries(ltr_udp linuxtrack pthread dl)
set_linuxtrack_rpath(ltr_udp)

# Man page for ltr_udp (other tools ship .1 sources; wire this one into install)
install(FILES ltr_udp.1 DESTINATION share/man/man1)

# OSC sender (liblo) — upstream discussion: https://github.com/uglyDwarf/linuxtrack/issues/80
if(LIBLO_FOUND)
    add_executable(osc_server
        osc_server.c
        linuxtrack.c
        linuxtrack.h
    )
    target_include_directories(osc_server PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/src
        ${LIBLO_INCLUDE_DIRS}
    )
    target_link_libraries(osc_server PRIVATE
        linuxtrack
        ltr
        dl
        ${LIBLO_LIBRARIES}
        ${PTHREAD_LIBRARY}
    )
    set_linuxtrack_rpath(osc_server)
endif()

option(LTR_BUILD_TESTS "Build unit tests" ON)
if(LTR_BUILD_TESTS)
    add_executable(test_align_translation
        tests/align_translation_unit.c
        math_utils.c
    )
    target_include_directories(test_align_translation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(test_align_translation PRIVATE m)
endif()

# ============================================================================
# X-Plane Plugin (conditional)
# ============================================================================

if(ENABLE_XPLANE AND XPLANE_SDK_FOUND)
    add_library(xlinuxtrack9 MODULE
        xlinuxtrack9.c
        linuxtrack.c linuxtrack.h
        utils.c utils.h
    )

    set_target_properties(xlinuxtrack9 PROPERTIES
        PREFIX ""
        OUTPUT_NAME "xlinuxtrack9"
    )

    target_compile_options(xlinuxtrack9 PRIVATE
        -fPIC
        -I${XPLANE_SDK_PATH}/XPLM
        -I${XPLANE_SDK_PATH}/Widgets
        -DIBM=0
        -DAPL=0
        -DLIN=1
    )

    target_link_libraries(xlinuxtrack9
        ${MATH_LIBRARY}
        dl
    )

    # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/xlinuxtrack.sym)
    #     target_link_options(xlinuxtrack9 PRIVATE
    #         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/xlinuxtrack.sym
    #     )
    # endif()
endif()

# ============================================================================
# Qt6 Applications (subdirectories)
# ============================================================================

# Add Qt6 GUI subdirectory
add_subdirectory(qt_gui)

# Add Mickey subdirectory
if(MICKEY_SUPPORT)
    add_subdirectory(mickey)
endif()

# Add Wii server subdirectory
if(WIIMOTE_SUPPORT)
    add_subdirectory(wii_server)
endif()

# ============================================================================
# Installation
# ============================================================================

# Install libraries to lib/linuxtrack
install(TARGETS
    linuxtrack
    ltr
    tir
    joy
    ltusb1
    ltr_server1
    ltr_pipe
    ltr_extractor
    ltr_recenter
    ltr_udp
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/linuxtrack
    ARCHIVE DESTINATION lib/linuxtrack
)

if(LIBLO_FOUND)
    install(TARGETS osc_server
        RUNTIME DESTINATION bin
    )
endif()

if(WEBCAM_SUPPORT)
    install(TARGETS wc
        LIBRARY DESTINATION lib/linuxtrack
        ARCHIVE DESTINATION lib/linuxtrack
    )
    if(FACE_TRACKER_SUPPORT)
        # Autotools built libft.so separately; CMake unifies into libwc. Symlink for
        # backward compatibility with docs/scripts that reference libft.so.0.
        install(CODE "
            set(LIB_DIR \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\")
            set(LIB_WC \"\${LIB_DIR}/libwc.so.0\")
            set(SYMLINK \"\${LIB_DIR}/libft.so.0\")
            if(EXISTS \"\${LIB_WC}\")
                if(NOT EXISTS \"\${SYMLINK}\")
                    execute_process(
                        COMMAND \${CMAKE_COMMAND} -E create_symlink libwc.so.0 libft.so.0
                        WORKING_DIRECTORY \"\${LIB_DIR}\"
                        RESULT_VARIABLE SYMLINK_RESULT
                    )
                    if(SYMLINK_RESULT EQUAL 0)
                        message(STATUS \"Created libft.so.0 -> libwc.so.0 symlink\")
                    else()
                        message(WARNING \"Failed to create libft.so.0 symlink\")
                    endif()
                endif()
            endif()
        ")
    endif()
endif()

if(NOT APPLE)
    install(TARGETS p3e
        LIBRARY DESTINATION lib/linuxtrack
        ARCHIVE DESTINATION lib/linuxtrack
    )
    if(FACE_TRACKER_SUPPORT)
        install(TARGETS p3eft
            LIBRARY DESTINATION lib/linuxtrack
            ARCHIVE DESTINATION lib/linuxtrack
        )
    endif()
endif()

if(ENABLE_XPLANE AND XPLANE_SDK_FOUND)
    install(TARGETS xlinuxtrack9
        LIBRARY DESTINATION lib/linuxtrack
        ARCHIVE DESTINATION lib/linuxtrack
    )
    install(CODE "
        set(LIB_DIR \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\")
        set(LIB_FILE \"\${LIB_DIR}/xlinuxtrack9.so\")
        set(SYMLINK_FILE \"\${LIB_DIR}/xlinuxtrack9.so.0\")
        if(EXISTS \"\${LIB_FILE}\")
            if(NOT EXISTS \"\${SYMLINK_FILE}\")
                execute_process(
                    COMMAND \${CMAKE_COMMAND} -E create_symlink xlinuxtrack9.so xlinuxtrack9.so.0
                    WORKING_DIRECTORY \"\${LIB_DIR}\"
                    RESULT_VARIABLE SYMLINK_RESULT
                )
                if(SYMLINK_RESULT EQUAL 0)
                    message(STATUS \"✅ Created xlinuxtrack9.so.0 symlink\")
                else()
                    message(WARNING \"⚠️  Failed to create xlinuxtrack9.so.0 symlink\")
                endif()
            else()
                message(STATUS \"ℹ️  xlinuxtrack9.so.0 symlink already exists\")
            endif()
        else()
            message(WARNING \"⚠️  xlinuxtrack9.so not found at \${LIB_FILE}\")
        endif()
    ")
endif()

# Note: /opt is now the default install prefix, providing Steam Proton compatibility
# Steam Proton's pressure-vessel container can access /opt but not /usr
# No need for dual-installation logic since /opt is the default

# Install 32-bit library to multiarch directory if LIB32DIR is set
if(LTR32ON64)
    if(LIB32DIR AND NOT LIB32DIR STREQUAL "lib32")
        # Install to multiarch directory with standard name (matching Autotools behavior)
        # The library is installed as liblinuxtrack.so.0.0.0 (not liblinuxtrack32.so.0.0.0)
        # to match the naming convention expected by Wine
        install(TARGETS linuxtrack32
            LIBRARY DESTINATION lib/${LIB32DIR}/linuxtrack
            ARCHIVE DESTINATION lib/${LIB32DIR}/linuxtrack
            RENAME liblinuxtrack.so.0.0.0
        )
        
        # Create symlinks for versioned library (matching Autotools behavior)
        # liblinuxtrack.so.0 -> liblinuxtrack.so.0.0.0
        # liblinuxtrack.so -> liblinuxtrack.so.0
        install(CODE "
            set(LIB_DIR \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/${LIB32DIR}/linuxtrack\")
            set(LIB_FILE \"\${LIB_DIR}/liblinuxtrack.so.0.0.0\")
            if(EXISTS \"\${LIB_FILE}\")
                execute_process(
                    COMMAND \${CMAKE_COMMAND} -E create_symlink liblinuxtrack.so.0.0.0 liblinuxtrack.so.0
                    WORKING_DIRECTORY \"\${LIB_DIR}\"
                    RESULT_VARIABLE SYMLINK_RESULT
                )
                if(SYMLINK_RESULT EQUAL 0)
                    execute_process(
                        COMMAND \${CMAKE_COMMAND} -E create_symlink liblinuxtrack.so.0 liblinuxtrack.so
                        WORKING_DIRECTORY \"\${LIB_DIR}\"
                    )
                    message(STATUS \"✅ 32-bit library installed to ${CMAKE_INSTALL_PREFIX}/lib/${LIB32DIR}/linuxtrack/liblinuxtrack.so.0\")
                endif()
            else()
                message(WARNING \"⚠️  32-bit library not found at \${LIB_FILE}\")
            endif()
        ")
        
        # Remove 32-bit library from standard location (matching Autotools behavior)
        install(CODE "
            set(STD_LIB_DIR \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\")
            file(GLOB STD_LIB_FILES \"\${STD_LIB_DIR}/liblinuxtrack32.so*\")
            foreach(FILE \${STD_LIB_FILES})
                file(REMOVE \"\${FILE}\")
            endforeach()
        ")
        
        # Note: /opt is now the default, so no dual-installation needed
    else()
        # Install to standard location if LIB32DIR is not set or is "lib32"
        install(TARGETS linuxtrack32
            LIBRARY DESTINATION lib/linuxtrack
            ARCHIVE DESTINATION lib/linuxtrack
        )
        message(STATUS "ℹ️  LIB32DIR not set - 32-bit library installed to standard location: lib/linuxtrack")
        message(STATUS "ℹ️  For Debian/Ubuntu multiarch support, use: -DLIB32DIR=i386-linux-gnu")
    endif()
endif()

# Install data files
install(FILES
    linuxtrack1.conf
    linuxtrack.c
    linuxtrack_hello_world.c
    linuxtrack_hello_world_adv.c
    99-TIR.rules
    99-PS3Eye.rules
    99-Mickey.rules
    haarcascade_frontalface_alt2.xml
    DESTINATION share/linuxtrack
)

# Install desktop files
install(FILES
    linuxtrack.desktop
    mickey.desktop
    DESTINATION share/applications
)

if(WIIMOTE_SUPPORT)
    install(FILES
        linuxtrack-wii.desktop
        DESTINATION share/applications
    )
endif()

# Install application icons
# Install PNG icon (48x48 is standard for desktop entries)
install(FILES
    linuxtrack.png
    DESTINATION share/icons/hicolor/48x48/apps
)

# Install SVG icon (scalable)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/linuxtrack.svg)
    install(FILES
        linuxtrack.svg
        DESTINATION share/icons/hicolor/scalable/apps
    )
endif()

# Install Wiimote icon if Wiimote support is enabled
if(WIIMOTE_SUPPORT AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/linuxtrack-wii.png)
    install(FILES
        linuxtrack-wii.png
        DESTINATION share/icons/hicolor/48x48/apps
    )
endif()

if(WIIMOTE_SUPPORT AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/linuxtrack-wii.svg)
    install(FILES
        linuxtrack-wii.svg
        DESTINATION share/icons/hicolor/scalable/apps
    )
endif()

# Update desktop database and icon cache after installation
# This ensures desktop files and icons are visible in application menus
install(CODE "
    if(NOT \"\$ENV{DESTDIR}\" STREQUAL \"\")
        # Staging installation - skip updates (package managers handle this)
        message(STATUS \"Skipping desktop/icon cache updates for staging installation\")
    else()
        # System installation - update desktop database and icon cache
        set(ICON_DIR \"${CMAKE_INSTALL_PREFIX}/share/icons/hicolor\")
        set(DESKTOP_DIR \"${CMAKE_INSTALL_PREFIX}/share/applications\")
        
        # Update desktop database
        if(EXISTS \"\${DESKTOP_DIR}\")
            find_program(UPDATE_DESKTOP_DB update-desktop-database)
            if(UPDATE_DESKTOP_DB)
                message(STATUS \"Updating desktop database: \${DESKTOP_DIR}\")
                execute_process(
                    COMMAND \${UPDATE_DESKTOP_DB} \"\${DESKTOP_DIR}\"
                    OUTPUT_QUIET
                    ERROR_QUIET
                    RESULT_VARIABLE DESKTOP_DB_RESULT
                )
                if(NOT DESKTOP_DB_RESULT EQUAL 0)
                    message(WARNING \"⚠️  update-desktop-database failed (exit code: \${DESKTOP_DB_RESULT})\")
                    message(WARNING \"⚠️  You may need to run 'sudo update-desktop-database \${DESKTOP_DIR}' manually\")
                else()
                    message(STATUS \"✅ Desktop database updated successfully\")
                endif()
            else()
                message(STATUS \"ℹ️  update-desktop-database not found - desktop files may not appear in menus until cache is updated\")
            endif()
        endif()
        
        # Update icon cache
        if(EXISTS \"\${ICON_DIR}\")
            find_program(UPDATE_ICON_CACHE gtk-update-icon-cache)
            if(UPDATE_ICON_CACHE)
                message(STATUS \"Updating icon cache: \${ICON_DIR}\")
                execute_process(
                    COMMAND \${UPDATE_ICON_CACHE} -f -t \"\${ICON_DIR}\"
                    OUTPUT_QUIET
                    ERROR_QUIET
                    RESULT_VARIABLE ICON_CACHE_RESULT
                )
                if(NOT ICON_CACHE_RESULT EQUAL 0)
                    message(WARNING \"⚠️  gtk-update-icon-cache failed (exit code: \${ICON_CACHE_RESULT})\")
                    message(WARNING \"⚠️  You may need to run 'sudo gtk-update-icon-cache -f -t \${ICON_DIR}' manually\")
                else()
                    message(STATUS \"✅ Icon cache updated successfully\")
                endif()
            else()
                message(STATUS \"ℹ️  gtk-update-icon-cache not found - icons may not appear until cache is updated\")
            endif()
        endif()
    endif()
")

# Build Qt GUI after core libraries
if(TARGET ltr_gui)
    add_dependencies(ltr_gui ltr tir)
endif()

# Create symlinks from /opt/bin to /usr/local/bin for PATH compatibility
# This allows binaries to be found even if /opt/bin is not in PATH
if(CMAKE_INSTALL_PREFIX STREQUAL "/opt")
    install(CODE "
        message(STATUS \"Creating symlinks from /opt to /usr/local for compatibility...\")
        set(BIN_DIR \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin\")
        set(SYMLINK_BIN_DIR \"\$ENV{DESTDIR}/usr/local/bin\")
        
        # Create /usr/local/bin directory if it doesn't exist (handles both staging and system installs)
        if(NOT EXISTS \"\${SYMLINK_BIN_DIR}\")
            execute_process(
                COMMAND \${CMAKE_COMMAND} -E make_directory \"\${SYMLINK_BIN_DIR}\"
                RESULT_VARIABLE MKDIR_RESULT
            )
            if(NOT MKDIR_RESULT EQUAL 0)
                message(WARNING \"⚠️  Failed to create directory: \${SYMLINK_BIN_DIR}\")
            endif()
        endif()
        
        # List of binaries to symlink (/usr/local/bin is on typical PATH; /opt/bin often is not)
        set(BINARIES ltr_gui ltr_server1 ltr_pipe ltr_extractor ltr_recenter ltr_udp)
        if(EXISTS \"\${BIN_DIR}/mickey\")
            list(APPEND BINARIES mickey)
        endif()
        if(EXISTS \"\${BIN_DIR}/osc_server\")
            list(APPEND BINARIES osc_server)
        endif()
        
        foreach(BINARY \${BINARIES})
            set(BINARY_PATH \"\${BIN_DIR}/\${BINARY}\")
            set(SYMLINK_BIN_PATH \"\${SYMLINK_BIN_DIR}/\${BINARY}\")
            # Symlink target should be the final installed path (without DESTDIR)
            # This ensures symlinks work correctly after package deployment
            set(SYMLINK_TARGET \"${CMAKE_INSTALL_PREFIX}/bin/\${BINARY}\")
            
            if(EXISTS \"\${BINARY_PATH}\")
                # Remove existing symlink or file if it exists
                if(EXISTS \"\${SYMLINK_BIN_PATH}\")
                    execute_process(
                        COMMAND \${CMAKE_COMMAND} -E remove \"\${SYMLINK_BIN_PATH}\"
                    )
                endif()
                
                # Create symlink pointing to final installed path (not staging path)
                execute_process(
                    COMMAND \${CMAKE_COMMAND} -E create_symlink \"\${SYMLINK_TARGET}\" \"\${SYMLINK_BIN_PATH}\"
                    RESULT_VARIABLE SYMLINK_RESULT
                )
                
                if(SYMLINK_RESULT EQUAL 0)
                    message(STATUS \"✅ Created symlink: \${SYMLINK_BIN_PATH} -> \${SYMLINK_TARGET}\")
                else()
                    message(WARNING \"⚠️  Failed to create symlink: \${SYMLINK_BIN_PATH}\")
                endif()
            endif()
        endforeach()
        
        # Create symlinks for desktop files (desktop menu compatibility)
        # Many desktop environments don't search /opt/share/applications by default
        set(DESKTOP_DIR \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/applications\")
        set(SYMLINK_DESKTOP_DIR \"\$ENV{DESTDIR}/usr/local/share/applications\")
        
        # Create /usr/local/share/applications directory if it doesn't exist
        if(NOT EXISTS \"\${SYMLINK_DESKTOP_DIR}\")
            execute_process(
                COMMAND \${CMAKE_COMMAND} -E make_directory \"\${SYMLINK_DESKTOP_DIR}\"
                RESULT_VARIABLE MKDIR_RESULT
            )
            if(NOT MKDIR_RESULT EQUAL 0)
                message(WARNING \"⚠️  Failed to create directory: \${SYMLINK_DESKTOP_DIR}\")
            endif()
        endif()
        
        # List of desktop files to symlink
        set(DESKTOP_FILES linuxtrack.desktop mickey.desktop)
        if(EXISTS \"\${DESKTOP_DIR}/linuxtrack-wii.desktop\")
            list(APPEND DESKTOP_FILES linuxtrack-wii.desktop)
        endif()
        
        foreach(DESKTOP_FILE \${DESKTOP_FILES})
            set(DESKTOP_PATH \"\${DESKTOP_DIR}/\${DESKTOP_FILE}\")
            set(SYMLINK_PATH \"\${SYMLINK_DESKTOP_DIR}/\${DESKTOP_FILE}\")
            # Symlink target should be the final installed path (without DESTDIR)
            set(SYMLINK_TARGET \"${CMAKE_INSTALL_PREFIX}/share/applications/\${DESKTOP_FILE}\")
            
            if(EXISTS \"\${DESKTOP_PATH}\")
                # Remove existing symlink or file if it exists
                if(EXISTS \"\${SYMLINK_PATH}\")
                    execute_process(
                        COMMAND \${CMAKE_COMMAND} -E remove \"\${SYMLINK_PATH}\"
                    )
                endif()
                
                # Create symlink pointing to final installed path
                execute_process(
                    COMMAND \${CMAKE_COMMAND} -E create_symlink \"\${SYMLINK_TARGET}\" \"\${SYMLINK_PATH}\"
                    RESULT_VARIABLE SYMLINK_RESULT
                )
                
                if(SYMLINK_RESULT EQUAL 0)
                    message(STATUS \"✅ Created desktop symlink: \${SYMLINK_PATH} -> \${SYMLINK_TARGET}\")
                else()
                    message(WARNING \"⚠️  Failed to create desktop symlink: \${SYMLINK_PATH}\")
                endif()
            endif()
        endforeach()
        
        # Update desktop database for /usr/local/share/applications (standard location)
        if(\"\$ENV{DESTDIR}\" STREQUAL \"\" AND EXISTS \"\${SYMLINK_DESKTOP_DIR}\")
            find_program(UPDATE_DESKTOP_DB update-desktop-database)
            if(UPDATE_DESKTOP_DB)
                message(STATUS \"Updating desktop database: \${SYMLINK_DESKTOP_DIR}\")
                execute_process(
                    COMMAND \${UPDATE_DESKTOP_DB} \"\${SYMLINK_DESKTOP_DIR}\"
                    OUTPUT_QUIET
                    ERROR_QUIET
                    RESULT_VARIABLE DESKTOP_DB_RESULT
                )
                if(NOT DESKTOP_DB_RESULT EQUAL 0)
                    message(WARNING \"⚠️  update-desktop-database failed (exit code: \${DESKTOP_DB_RESULT})\")
                    message(WARNING \"⚠️  You may need to run 'sudo update-desktop-database \${SYMLINK_DESKTOP_DIR}' manually\")
                else()
                    message(STATUS \"✅ Desktop database updated successfully\")
                endif()
            endif()
        endif()
        
        # Create symlinks for icons (icon theme compatibility)
        # /opt/share/icons/hicolor may not have index.theme, so symlink to /usr/local/share/icons/hicolor
        # which desktop environments definitely search
        set(ICON_DIR \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/icons/hicolor\")
        set(SYMLINK_ICON_DIR \"\$ENV{DESTDIR}/usr/local/share/icons/hicolor\")
        
        # Create icon theme directory structure if it doesn't exist
        set(ICON_SUBDIRS 48x48/apps scalable/apps)
        foreach(SUBDIR \${ICON_SUBDIRS})
            set(SUBDIR_PATH \"\${SYMLINK_ICON_DIR}/\${SUBDIR}\")
            if(NOT EXISTS \"\${SUBDIR_PATH}\")
                execute_process(
                    COMMAND \${CMAKE_COMMAND} -E make_directory \"\${SUBDIR_PATH}\"
                    RESULT_VARIABLE MKDIR_RESULT
                )
                if(NOT MKDIR_RESULT EQUAL 0)
                    message(WARNING \"⚠️  Failed to create icon directory: \${SUBDIR_PATH}\")
                endif()
            endif()
        endforeach()
        
        # List of icons to symlink
        set(ICONS linuxtrack.png linuxtrack.svg)
        if(EXISTS \"\${ICON_DIR}/48x48/apps/linuxtrack-wii.png\")
            list(APPEND ICONS linuxtrack-wii.png)
        endif()
        if(EXISTS \"\${ICON_DIR}/scalable/apps/linuxtrack-wii.svg\")
            list(APPEND ICONS linuxtrack-wii.svg)
        endif()
        
        foreach(ICON \${ICONS})
            # Determine source directory based on icon type
            # Check if icon ends with .svg using string operations (avoids regex escape issues)
            string(LENGTH \${ICON} ICON_LEN)
            if(ICON_LEN GREATER 4)
                # Calculate start position for last 4 characters
                math(EXPR START_POS \"\${ICON_LEN} - 4\")
                string(SUBSTRING \${ICON} \${START_POS} 4 ICON_SUFFIX)
                if(ICON_SUFFIX STREQUAL \".svg\")
                    set(ICON_SUBDIR \"scalable/apps\")
                else()
                    set(ICON_SUBDIR \"48x48/apps\")
                endif()
            else()
                set(ICON_SUBDIR \"48x48/apps\")
            endif()
            
            set(ICON_SOURCE_DIR \"\${ICON_DIR}/\${ICON_SUBDIR}\")
            set(ICON_SYMLINK_DIR \"\${SYMLINK_ICON_DIR}/\${ICON_SUBDIR}\")
            set(ICON_SOURCE_PATH \"\${ICON_SOURCE_DIR}/\${ICON}\")
            set(ICON_SYMLINK_PATH \"\${ICON_SYMLINK_DIR}/\${ICON}\")
            # Symlink target should be the final installed path (without DESTDIR)
            set(ICON_SYMLINK_TARGET \"${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/\${ICON_SUBDIR}/\${ICON}\")
            
            if(EXISTS \"\${ICON_SOURCE_PATH}\")
                # Remove existing symlink or file if it exists
                if(EXISTS \"\${ICON_SYMLINK_PATH}\")
                    execute_process(
                        COMMAND \${CMAKE_COMMAND} -E remove \"\${ICON_SYMLINK_PATH}\"
                    )
                endif()
                
                # Create symlink pointing to final installed path
                execute_process(
                    COMMAND \${CMAKE_COMMAND} -E create_symlink \"\${ICON_SYMLINK_TARGET}\" \"\${ICON_SYMLINK_PATH}\"
                    RESULT_VARIABLE SYMLINK_RESULT
                )
                
                if(SYMLINK_RESULT EQUAL 0)
                    message(STATUS \"✅ Created icon symlink: \${ICON_SYMLINK_PATH} -> \${ICON_SYMLINK_TARGET}\")
                else()
                    message(WARNING \"⚠️  Failed to create icon symlink: \${ICON_SYMLINK_PATH}\")
                endif()
            endif()
        endforeach()
        
        # Update icon cache for /usr/local/share/icons/hicolor (standard location with index.theme)
        if(\"\$ENV{DESTDIR}\" STREQUAL \"\" AND EXISTS \"\${SYMLINK_ICON_DIR}\")
            find_program(UPDATE_ICON_CACHE gtk-update-icon-cache)
            if(UPDATE_ICON_CACHE)
                message(STATUS \"Updating icon cache: \${SYMLINK_ICON_DIR}\")
                execute_process(
                    COMMAND \${UPDATE_ICON_CACHE} -f -t \"\${SYMLINK_ICON_DIR}\"
                    OUTPUT_QUIET
                    ERROR_QUIET
                    RESULT_VARIABLE ICON_CACHE_RESULT
                )
                if(NOT ICON_CACHE_RESULT EQUAL 0)
                    message(WARNING \"⚠️  gtk-update-icon-cache failed (exit code: \${ICON_CACHE_RESULT})\")
                    message(WARNING \"⚠️  You may need to run 'sudo gtk-update-icon-cache -f -t \${SYMLINK_ICON_DIR}' manually\")
                else()
                    message(STATUS \"✅ Icon cache updated successfully\")
                endif()
            else()
                message(STATUS \"ℹ️  gtk-update-icon-cache not found - icons may not appear until cache is updated\")
            endif()
        endif()
    ")
endif()

# ldconfig integration
if(ENABLE_LDCONFIG)
    install(CODE "
        # Create ld.so.conf.d file for library path registration
        # This is required because /opt/lib/linuxtrack is not in default library search paths
        if(NOT \"\$ENV{DESTDIR}\" STREQUAL \"\")
            # Staging installation - write to DESTDIR
            set(CONF_DIR \"\$ENV{DESTDIR}/etc/ld.so.conf.d\")
            set(CONF_FILE \"\${CONF_DIR}/linuxtrack.conf\")
            # Create directory if it doesn't exist
            if(NOT EXISTS \"\${CONF_DIR}\")
                file(MAKE_DIRECTORY \"\${CONF_DIR}\")
            endif()
            file(WRITE \"\${CONF_FILE}\" \"${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\\n\")
            message(STATUS \"Created ${CONF_FILE} with path: ${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\")
        else()
            # System installation - create conf file and run ldconfig
            # Note: This requires root/sudo permissions to write to /etc/ld.so.conf.d/
            set(CONF_DIR \"/etc/ld.so.conf.d\")
            set(CONF_FILE \"\${CONF_DIR}/linuxtrack.conf\")
            
            # Check if directory exists (it should on all Linux systems)
            if(NOT EXISTS \"\${CONF_DIR}\")
                message(WARNING \"⚠️  Directory \${CONF_DIR} does not exist - cannot create ldconfig configuration\")
                message(WARNING \"⚠️  You will need to manually create ${CONF_FILE} with content: ${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\")
            else()
                # Write the configuration file
                file(WRITE \"\${CONF_FILE}\" \"${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\\n\")
                message(STATUS \"Created ${CONF_FILE} with path: ${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\")
                
                # Run ldconfig to update library cache
                # Note: This may fail if not run with sufficient permissions
                execute_process(
                    COMMAND ldconfig
                    OUTPUT_QUIET
                    ERROR_QUIET
                    RESULT_VARIABLE LDCONFIG_RESULT
                )
                if(NOT LDCONFIG_RESULT EQUAL 0)
                    message(WARNING \"⚠️  ldconfig failed (exit code: \${LDCONFIG_RESULT})\")
                    message(WARNING \"⚠️  You may need to run 'sudo ldconfig' manually to update the library cache\")
                    message(WARNING \"⚠️  Library path ${CMAKE_INSTALL_PREFIX}/lib/linuxtrack has been added to ${CONF_FILE}\")
                else()
                    message(STATUS \"✅ Library cache updated successfully\")
                endif()
            endif()
        endif()
    ")
endif()

