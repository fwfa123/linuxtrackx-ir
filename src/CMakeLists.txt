# LinuxTrack Core Library (TrackIR focused)
# ===========================================

# Core tracking sources (common to all builds)
set(CORE_SOURCES
    # Core tracking engine
    tracking.c
    pose.c
    runloop.c
    utils.c
    math_utils.c
    list.c
    
    # LinuxTrack library interface
    ltlib.c
    ltlib_int.c
    ltlib_client.c
    
    # Server/client communication
    lt_server.c
    lt_client.c
    ltr_srv_master.cpp
    ltr_srv_slave.c
    ltr_srv_comm.c
    
    # Axis control and calibration
    axis.c
    spline.c
    com_proc.c
    cal.c
    
    # Image processing (for TrackIR blob detection)
    image_process.c
    
    # Preference system
    pref_global.c
    
    # Hash and digest functions
    digest.c
    
    # IPC utilities
    ipc_utils.c
)

# TrackIR hardware sources
if(HAVE_TRACKIR)
    list(APPEND CORE_SOURCES
        # USB interface for TrackIR
        libusb_ifc.c
        
        # TrackIR hardware drivers
        tir_hw.c
        tir_img.c
        tir_model.cpp
        tir_driver_prefs.c
        
        # TrackIR device driver
        tir_driver.c
    )
endif()

# Camera tracking stubs
if(STUB_CAMERA_TRACKING)
    list(APPEND CORE_SOURCES
        stubs/webcam_stub.c
        stubs/facetrack_stub.c
    )
endif()

# Wiimote tracking stubs  
if(STUB_WIIMOTE_TRACKING)
    list(APPEND CORE_SOURCES
        stubs/wiimote_stub.c
    )
endif()

# Create main LinuxTrack library
add_library(linuxtrack_core STATIC ${CORE_SOURCES})

# Include directories
target_include_directories(linuxtrack_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

# Add configuration defines
target_compile_definitions(linuxtrack_core PRIVATE
    HAVE_CONFIG_H=1
    PACKAGE_VERSION="${PROJECT_VERSION}"
    LIB_PATH="${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/"
    DATA_PATH="${CMAKE_INSTALL_PREFIX}/share/linuxtrack/"
    LIB_SUFFIX=".so.0"
)

# Link dependencies
target_link_libraries(linuxtrack_core
    ${MATH_LIBRARY}
    Threads::Threads
)

# TrackIR hardware support
if(HAVE_TRACKIR)
    target_link_libraries(linuxtrack_core ${LIBUSB_LIBRARIES})
    target_include_directories(linuxtrack_core PRIVATE ${LIBUSB_INCLUDE_DIRS})
    target_compile_definitions(linuxtrack_core PRIVATE HAVE_LIBUSB=1)
    message(STATUS "  TrackIR support: ENABLED (libusb-1.0)")
else()
    message(STATUS "  TrackIR support: DISABLED (libusb-1.0 not found)")
endif()

# MXML support (for configuration)
if(MXML_FOUND)
    target_link_libraries(linuxtrack_core ${MXML_LIBRARIES})
    target_include_directories(linuxtrack_core PRIVATE ${MXML_INCLUDE_DIRS})
    target_compile_definitions(linuxtrack_core PRIVATE HAVE_MXML=1)
endif()

# Create Camera Abstraction Layer (CAL) libraries for runtime loading
if(HAVE_TRACKIR)
    # TrackIR library (libtir.so.0)
    add_library(libtir SHARED
        tir_driver_prefs.c
        tir_hw.c
        tir_img.c
        tir_model.cpp
        libusb_ifc.c
        utils.c
        digest.c
    )
    
    target_link_libraries(libtir
        ${LIBUSB_LIBRARIES}
        ${MATH_LIBRARY}
        Threads::Threads
    )
    
    target_include_directories(libtir PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
    )
    
    target_compile_definitions(libtir PRIVATE
        HAVE_CONFIG_H=1
        PACKAGE_VERSION="${PROJECT_VERSION}"
        LIB_PATH="${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/"
        DATA_PATH="${CMAKE_INSTALL_PREFIX}/share/linuxtrack/"
        LIB_SUFFIX=".so.0"
    )
    
    set_target_properties(libtir PROPERTIES
        OUTPUT_NAME "tir"
        VERSION "0.0.0"
        SOVERSION "0"
    )
    
    # USB interface libraries
    add_library(libltusb1 SHARED libusb_ifc.c utils.c)
    target_link_libraries(libltusb1 ${LIBUSB_LIBRARIES})
    target_include_directories(libltusb1 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
    )
    target_compile_definitions(libltusb1 PRIVATE
        HAVE_CONFIG_H=1
        PACKAGE_VERSION="${PROJECT_VERSION}"
        LIB_PATH="${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/"
        DATA_PATH="${CMAKE_INSTALL_PREFIX}/share/linuxtrack/"
        LIB_SUFFIX=".so.0"
    )
    set_target_properties(libltusb1 PROPERTIES
        OUTPUT_NAME "ltusb1"
        VERSION "0.0.0"
        SOVERSION "0"
    )
    
    add_library(libfakeusb SHARED fakeusb.c utils.c)
    target_compile_definitions(libfakeusb PRIVATE FAKE_USB=1)
    target_include_directories(libfakeusb PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
    )
    target_compile_definitions(libfakeusb PRIVATE
        HAVE_CONFIG_H=1
        PACKAGE_VERSION="${PROJECT_VERSION}"
        LIB_PATH="${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/"
        DATA_PATH="${CMAKE_INSTALL_PREFIX}/share/linuxtrack/"
        LIB_SUFFIX=".so.0"
        FAKE_USB=1
    )
    set_target_properties(libfakeusb PROPERTIES
        OUTPUT_NAME "fakeusb"
        VERSION "0.0.0"
        SOVERSION "0"
    )
endif()

# Stubbed libraries (empty implementations)
if(STUB_CAMERA_TRACKING)
    add_library(libwc SHARED stubs/webcam_stub.c)
    set_target_properties(libwc PROPERTIES
        OUTPUT_NAME "wc"
        VERSION "0.0.0"
        SOVERSION "0"
    )
endif()

if(STUB_WIIMOTE_TRACKING)
    add_library(libjoy SHARED stubs/wiimote_stub.c)
    set_target_properties(libjoy PROPERTIES
        OUTPUT_NAME "joy"
        VERSION "0.0.0"
        SOVERSION "0"
    )
endif()

# LinuxTrack server executable
add_executable(ltr_server1 ltr_server1.c)
target_link_libraries(ltr_server1 linuxtrack_core)

# LinuxTrack recenter utility
add_executable(ltr_recenter ltr_recenter.c)
target_link_libraries(ltr_recenter linuxtrack_core)

# Qt5 GUI (modernized from Qt4)
if(BUILD_QT5_GUI)
    add_subdirectory(qt_gui)
endif()

# Create stub directories
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/stubs)

# Install targets
install(TARGETS linuxtrack_core ltr_server1 ltr_recenter
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

if(HAVE_TRACKIR)
    install(TARGETS libtir libltusb1 libfakeusb
        LIBRARY DESTINATION lib/linuxtrack
    )
endif()

if(STUB_CAMERA_TRACKING)
    install(TARGETS libwc
        LIBRARY DESTINATION lib/linuxtrack
    )
endif()

if(STUB_WIIMOTE_TRACKING)
    install(TARGETS libjoy
        LIBRARY DESTINATION lib/linuxtrack
    )
endif()

message(STATUS "Core library configured with ${CMAKE_CURRENT_SOURCE_DIR}") 