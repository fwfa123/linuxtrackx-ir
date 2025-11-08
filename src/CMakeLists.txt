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
    
    set_target_properties(wc PROPERTIES
        VERSION 0.0.0
        SOVERSION 0
        OUTPUT_NAME "wc"
    )
    
    target_link_libraries(wc
        ltr
        ${LIBV4L2_LIBRARY}
    )
    
    # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/webcam_driver.sym)
    #     target_link_options(wc PRIVATE
    #         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/webcam_driver.sym
    #     )
    # endif()
endif()

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
    
    # 32-bit version
    if(LTR32ON64)
        add_library(xlinuxtrack9_32 MODULE
            xlinuxtrack9.c
            linuxtrack.c linuxtrack.h
            utils.c utils.h
        )
        
        set_target_properties(xlinuxtrack9_32 PROPERTIES
            PREFIX ""
            OUTPUT_NAME "xlinuxtrack9_32"
        )
        
        target_compile_options(xlinuxtrack9_32 PRIVATE
            -fPIC
            -m32
            -I${XPLANE_SDK_PATH}/XPLM
            -I${XPLANE_SDK_PATH}/Widgets
            -DIBM=0
            -DAPL=0
            -DLIN=1
        )
        
        target_link_options(xlinuxtrack9_32 PRIVATE
            -m32
        )
        
        target_link_libraries(xlinuxtrack9_32
            ${MATH_LIBRARY}
            dl
        )
        
        # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/xlinuxtrack.sym)
        #     target_link_options(xlinuxtrack9_32 PRIVATE
        #         -Wl,--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/xlinuxtrack.sym
        #     )
        # endif()
    endif()
endif()

# ============================================================================
# Qt5 Applications (subdirectories)
# ============================================================================

# Add Qt5 GUI subdirectory
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
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/linuxtrack
    ARCHIVE DESTINATION lib/linuxtrack
)

if(WEBCAM_SUPPORT)
    install(TARGETS wc
        LIBRARY DESTINATION lib/linuxtrack
        ARCHIVE DESTINATION lib/linuxtrack
    )
endif()

if(ENABLE_XPLANE AND XPLANE_SDK_FOUND)
    install(TARGETS xlinuxtrack9
        LIBRARY DESTINATION lib/linuxtrack
        ARCHIVE DESTINATION lib/linuxtrack
    )
    if(LTR32ON64)
        install(TARGETS xlinuxtrack9_32
            LIBRARY DESTINATION lib/linuxtrack
            ARCHIVE DESTINATION lib/linuxtrack
        )
    endif()
endif()

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

# Build Qt GUI after core libraries
if(TARGET ltr_gui_qmake)
    add_dependencies(ltr_gui_qmake ltr tir)
endif()

# ldconfig integration
if(ENABLE_LDCONFIG)
    install(CODE "
        file(WRITE \"\$ENV{DESTDIR}/etc/ld.so.conf.d/linuxtrack.conf\" \"${CMAKE_INSTALL_PREFIX}/lib/linuxtrack\\n\")
        if(\"\$ENV{DESTDIR}\" STREQUAL \"\")
            execute_process(
                COMMAND ldconfig
                OUTPUT_QUIET
                ERROR_QUIET
            )
        endif()
    ")
endif()

