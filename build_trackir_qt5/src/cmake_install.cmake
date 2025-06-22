# Install script for directory: /mnt/Local_Git/git-repos/linuxtrack-clean-june14/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/qt_gui/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/liblinuxtrack_core.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/ltr_server1")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/ltr_recenter")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libtir.so.0.0.0"
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libtir.so.0"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libtir.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libltusb1.so.0.0.0"
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libltusb1.so.0"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libltusb1.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libfakeusb.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libfakeusb.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libfakeusb.so.0.0.0"
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libfakeusb.so.0"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libfakeusb.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libfakeusb.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libfakeusb.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libwc.so.0.0.0"
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libwc.so.0"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libwc.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libjoy.so.0.0.0"
    "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libjoy.so.0"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/libjoy.so")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/build_trackir_qt5/src/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
