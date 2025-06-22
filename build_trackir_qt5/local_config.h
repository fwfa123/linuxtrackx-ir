#ifndef LOCAL_CONFIG_H
#define LOCAL_CONFIG_H

/* Configuration for LinuxTrack - TrackIR Focus Build */

/* Version information */
#define PACKAGE_VERSION "0.99.0"
#define PACKAGE_NAME "linuxtrack"

/* Hardware support */
#define HAVE_TRACKIR 1

/* Build configuration */
#define BUILD_QT5_GUI 1
#define STUB_CAMERA_TRACKING 1
#define STUB_WIIMOTE_TRACKING 1

/* System capabilities */
#define HAVE_LIBUSB 1
#define HAVE_PTHREAD 1

/* Paths */
#define CMAKE_INSTALL_PREFIX "/usr/local"
#define DATADIR "/usr/local/share"

/* Platform detection */
#ifdef __APPLE__
    #define DARWIN 1
#endif

/* Configuration flags for LinuxTrack core */
#ifndef DARWIN
    #define DATA_PATH "/share/linuxtrack/"
    #define LIB_SUFFIX ".so.0"
    #define LIB_PATH "/lib/linuxtrack/"
#else
    #define DATA_PATH "/../Resources/linuxtrack/"
    #define LIB_PATH "/../Frameworks/"
    #define LIB_SUFFIX ".0.dylib"
#endif

#endif /* LOCAL_CONFIG_H */
