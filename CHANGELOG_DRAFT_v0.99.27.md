# Changelog Draft for v0.99.27

Review these numbered items and indicate which ones to include in the release post.

## Fixed Issues

1. **System Tab - Memory Display**: Fixed memory category always showing blank by properly parsing /proc/meminfo values (extracting numeric part before 'kB' suffix)

2. **System Tab - Build Components Display**: Fixed LinuxTrack Components showing hardcoded values by adding build configuration macros in configure.ac and using #ifdef checks to display actual build options instead of development OS values

3. **System Tab - Device Support Removal**: Removed Device Support section (removed getDeviceSupportInfo() call and function definition/declaration)

4. **32-bit Library Build**: Fixed missing liblinuxtrack32.la build target when --enable-ltr-32lib-on-x64 is used, which caused 32-bit winebridge to fail with err_NOT_FOUND (library not found) error

5. **32-bit Library Installation**: Fixed 32-bit library installation to multiarch directory when --with-lib32-dir is set (Debian/Ubuntu: /usr/lib/i386-linux-gnu/linuxtrack/), creating proper symlinks and removing library from standard location to avoid conflicts

6. **Winebridge Client Mode**: Restored slave process spawning in client mode that was removed in commit dcc32d6, which broke winebridge tracking data communication. Now tracking data flows correctly from master to client.

7. **X-Plane Plugin - Undefined Symbol**: Fixed undefined symbol `ltr_int_get_default_file_name` by removing -DLIBLINUXTRACK_SRC flag and adding utils.c/utils.h to X-Plane plugin sources

8. **X-Plane Plugin - Library Path Lookup**: Fixed ltr_int_get_lib_path() to return full absolute paths on Linux, enabling the installer to find and copy plugin libraries correctly

9. **Wine Bridge Components**: Applied same fixes (utils.c compilation, -fPIC flag) to Wine bridge components (NPClient, Controller, FreeTrackClient) to resolve relocation issues

10. **TrackIR Permission Dialog**: Improved TrackIR setup dialog to eliminate redundant password prompts and make users aware they can use terminal and manual sudo commands to install UDEV rules and Groups instead of the built-in application tool. The dialog now removes the password input field, makes the confirmation checkbox unchecked by default, and lets the system's native sudo dialog handle password prompting, while clearly communicating that manual terminal installation is an available option.

11. **Fedora 42 Client Mode Initialization**: Fixed LinuxTrack client mode initialization for Fedora 42 by fixing client mode initialization in ltr_int_init_helper() to allow reconnection, enhancing library path detection, and improving socket-based server detection

12. **Glibc Compatibility**: Reduced FORTIFY_SOURCE from 2 to 1 to avoid glibc 2.38+ functions (__isoc23_sscanf, __isoc23_fscanf), resolving build failures on Debian 12

13. **Minimal Build Support**: Fixed webcam library loading in minimal builds with WEBCAM_SUPPORT conditional compilation, added /usr/lib64/linuxtrack/ to library search paths, and improved ltr_pipe client initialization

14. **Automake Errors**: Fixed duplicate all-local target by using proper conditional structure and removed libwc.la from EXTRA_pkglib_LTLIBRARIES to avoid conflict

15. **Webcam Driver Compilation**: Fixed 'libv4l2.h: No such file or directory' error in minimal builds by making webcam support disabled by default (requires --enable-webcam) and fixing configure.ac webcam support logic

16. **Basic Build Philosophy**: Fixed configure.ac to properly implement basic build philosophy - changed cwiid and liblo library detection to use AC_MSG_WARN instead of AC_MSG_FAILURE, allowing optional components to gracefully disable instead of failing configure

## Added Features

17. **DESTDIR Support**: Implemented DESTDIR support for staging installations, fixing install-exec-hook and uninstall-hook to respect DESTDIR for proper packaging system compatibility

18. **--disable-ldconfig Option**: Added --disable-ldconfig configure option to skip library configuration entirely for distributions that handle library paths automatically

19. **Library Installation Verification**: Enhanced error handling with proper exit codes, user-friendly messages, and library installation verification with status indicators

20. **Qt5 Tools PATH Documentation**: Enhanced README with comprehensive Qt5 tools PATH fixes for Fedora/RHEL, Arch Linux, and OpenSUSE, including prominent sections and troubleshooting entries for qhelpgenerator issues

21. **Ldconfig Warning Documentation**: Added documentation note that ldconfig may fail during installation with manual ldconfig command provided for post-installation

22. **Minimal Build Requirements Documentation**: Updated minimal build requirements documentation with qt5-qttools-devel, mesa-libGL-devel, mesa-libGLU-devel for Fedora/RHEL, and build commands with Qt tools PATH export

23. **BUILD_OPTIONS.md Guide**: Added comprehensive BUILD_OPTIONS.md guide with 7 progressive build levels from TrackIR-only to full-featured, distribution-specific library installation commands, detailed troubleshooting, Wine configuration details, and testing procedures

## Changed/Improved

24. **Install Hook Error Handling**: Improved install-exec-hook error handling to make ldconfig failure non-fatal with warning message, check for core libraries instead of webcam libraries in minimal builds, and provide informative messages

25. **Library Path Resolution**: Improved library path resolution and prefix detection for better cross-distro compatibility (Fedora, Arch, Debian)

26. **Error Checking and Logging**: Added error checking and logging in ltlib.c for better diagnostics - check for NULL server path before fork, verify executable exists and is accessible, add informative log messages

27. **Socket-Based Server Detection**: Enhanced socket-based server detection to replace unreliable lock file detection for better reliability across distributions

28. **Configure.ac Refactoring**: Refactored configure.ac to implement basic build philosophy with TrackIR and Wine components enabled by default when dependencies found, optional components disabled by default, and PIE/security features enabled by default

29. **Wiimote/Wii Server Build Control**: Fixed Makefile.am so Wiimote/Wii server is properly controlled by CWIID conditional for build/install/clean targets, making linuxtrack-wii.desktop conditional on CWIID support

30. **Repository Cleanup**: Removed generated files (autotools generated files, Qt qmake generated project files, build artifacts) and updated .gitignore to prevent future commits of generated files

31. **README Updates**: Multiple README updates including Fedora 42 issues documentation, Qt5 PATH fixes, build options merged from separate file, and comprehensive installation instructions

---

**Total Items: 31**

Instructions: Review each numbered item above and indicate which ones to include in the release post.

