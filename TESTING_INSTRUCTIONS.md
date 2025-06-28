# LinuxTrack Testing Instructions

## Local Git Repository Testing

Your LinuxTrack Qt5 repository is now ready for testing! The changes have been committed and pushed to your local git repository.

### 🔧 **Repository Location**

The prepared repository is available at:
```
/mnt/Local_Git/git-repos/linuxtrack-modernized (branch: trackir2winebridge)
```

### 📋 **Testing Process for Another OS**

#### Step 1: Clone the Repository
```bash
# From your other OS, clone the repository
git clone /mnt/Local_Git/git-repos/linuxtrack-modernized linuxtrack-test
cd linuxtrack-test

# Switch to the prepared branch
git checkout trackir2winebridge
```

#### Step 2: Quick Test Build (Automated)
```bash
# Test the one-command build process
./build.sh --deps --clean --install

# If successful, launch the application
./run_qt5_gui.sh
```

#### Step 3: Manual Build Test (Alternative)
```bash
# Test manual build process as documented in README
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
sudo ./setup.sh
```

### 🧪 **What to Test**

#### ✅ **Build System Testing**
- [ ] Dependencies install correctly on target OS
- [ ] Autotools configuration completes without errors
- [ ] Qt5 is properly detected (not Qt6)
- [ ] All libraries build successfully
- [ ] GUI application compiles and links

#### ✅ **TrackIR Hardware Testing**
- [ ] TrackIR 5 device is detected by system
- [ ] Device permissions are correct
- [ ] LinuxTrack GUI detects TrackIR without "permission errors"
- [ ] Tracking works and responds to head movement

### 🎯 **Success Confirmation**

**The test is successful if:**
- Build completes with `./build.sh --deps --clean --install`
- GUI launches with `./run_qt5_gui.sh`
- TrackIR 5 is detected and shows **no permission errors**
- Head tracking works smoothly

The repository is professionally prepared and ready for public distribution once testing confirms everything works as expected! 🎮✈️🏎️


Testing noted on Kubuntu 25.10
We to encourage installing Wine first  
Do we need wine development installed?
If wine is installed they should use ./configure --prefix=/usr/local --with-wine-libs=-L/opt/wine-devel/bin
Is /use/local the best location?  Previous builds on the old app were ./configure --prefix=$HOME/linuxtrack --with-wine-libs=-L/opt/wine-devel/bin

After the dependencies were installed.
Logfile from autoreconf -fiv
autoreconf -fiv
autoreconf: export WARNINGS=
autoreconf: Entering directory '.'
autoreconf: configure.ac: not using Gettext
autoreconf: running: aclocal --force 
autoreconf: configure.ac: tracing
autoreconf: running: libtoolize --copy --force
libtoolize: putting auxiliary files in '.'.
libtoolize: copying file './ltmain.sh'
libtoolize: putting macros in AC_CONFIG_MACRO_DIRS, 'm4'.
libtoolize: copying file 'm4/libtool.m4'
libtoolize: copying file 'm4/ltoptions.m4'
libtoolize: copying file 'm4/ltsugar.m4'
libtoolize: copying file 'm4/ltversion.m4'
libtoolize: copying file 'm4/lt~obsolete.m4'
libtoolize: Consider adding '-I m4' to ACLOCAL_AMFLAGS in Makefile.am.
autoreconf: configure.ac: not using Intltool
autoreconf: configure.ac: not using Gtkdoc
autoreconf: running: aclocal --force 
autoreconf: running: /usr/bin/autoconf --force
autoreconf: running: /usr/bin/autoheader --force
autoreconf: running: automake --add-missing --copy --force-missing
configure.ac:25: installing './compile'
configure.ac:6: installing './missing'
autoreconf: Leaving directory '.'

Logfile from ./configure --prefix=/usr/local --with-wine-libs=-L/opt/wine-devel/bin
checking for a BSD-compatible install... /usr/bin/install -c
checking whether sleep supports fractional seconds... yes
checking filesystem timestamp resolution... 0.01
checking whether build environment is sane... yes
checking for a race-free mkdir -p... /usr/bin/mkdir -p
checking for gawk... no
checking for mawk... mawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking xargs -n works... yes
checking for g++... g++
checking whether the C++ compiler works... yes
checking for C++ compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether the compiler supports GNU C++... yes
checking whether g++ accepts -g... yes
checking for g++ option to enable C++11 features... none needed
checking for gcc... gcc
checking whether the compiler supports GNU Objective C... no
checking whether gcc accepts -g... no
checking for bison... yes
checking for bison... bison -y
checking for gcc... gcc
checking whether the compiler supports GNU C... yes
checking whether gcc accepts -g... yes
checking for gcc option to enable C11 features... none needed
checking whether gcc understands -c and -o together... yes
checking for flex... flex
checking for lex output file root... lex.yy
checking for lex library... none needed
checking whether yytext is a pointer... yes
checking for ar... ar
checking the archiver (ar) interface... ar
checking build system type... x86_64-pc-linux-gnu
checking host system type... x86_64-pc-linux-gnu
checking how to print strings... printf
checking for a sed that does not truncate output... /usr/bin/sed
checking for grep that handles long lines and -e... /usr/bin/grep
checking for egrep... /usr/bin/grep -E
checking for fgrep... /usr/bin/grep -F
checking for ld used by gcc... /usr/bin/ld
checking if the linker (/usr/bin/ld) is GNU ld... yes
checking for BSD- or MS-compatible name lister (nm)... /usr/bin/nm -B
checking the name lister (/usr/bin/nm -B) interface... BSD nm
checking whether ln -s works... yes
checking the maximum length of command line arguments... 1572864
checking how to convert x86_64-pc-linux-gnu file names to x86_64-pc-linux-gnu format... func_convert_file_noop
checking how to convert x86_64-pc-linux-gnu file names to toolchain format... func_convert_file_noop
checking for /usr/bin/ld option to reload object files... -r
checking for file... file
checking for objdump... objdump
checking how to recognize dependent libraries... pass_all
checking for dlltool... no
checking how to associate runtime and link libraries... printf %s\n
checking for ranlib... ranlib
checking for archiver @FILE support... @
checking for strip... strip
checking command to parse /usr/bin/nm -B output from gcc object... ok
checking for sysroot... no
checking for a working dd... /usr/bin/dd
checking how to truncate binary pipes... /usr/bin/dd bs=4096 count=1
checking for mt... mt
checking if mt is a manifest tool... no
checking for stdio.h... yes
checking for stdlib.h... yes
checking for string.h... yes
checking for inttypes.h... yes
checking for stdint.h... yes
checking for strings.h... yes
checking for sys/stat.h... yes
checking for sys/types.h... yes
checking for unistd.h... yes
checking for dlfcn.h... yes
checking for objdir... .libs
checking if gcc supports -fno-rtti -fno-exceptions... no
checking for gcc option to produce PIC... -fPIC -DPIC
checking if gcc PIC flag -fPIC -DPIC works... yes
checking if gcc static flag -static works... yes
checking if gcc supports -c -o file.o... yes
checking if gcc supports -c -o file.o... (cached) yes
checking whether the gcc linker (/usr/bin/ld -m elf_x86_64) supports shared libraries... yes
checking whether -lc should be explicitly linked in... no
checking dynamic linker characteristics... GNU/Linux ld.so
checking how to hardcode library paths into programs... immediate
checking whether stripping libraries is possible... yes
checking if libtool supports shared libraries... yes
checking whether to build shared libraries... yes
checking whether to build static libraries... yes
checking how to run the C++ preprocessor... g++ -E
checking for ld used by g++... /usr/bin/ld -m elf_x86_64
checking if the linker (/usr/bin/ld -m elf_x86_64) is GNU ld... yes
checking whether the g++ linker (/usr/bin/ld -m elf_x86_64) supports shared libraries... yes
checking for g++ option to produce PIC... -fPIC -DPIC
checking if g++ PIC flag -fPIC -DPIC works... yes
checking if g++ static flag -static works... yes
checking if g++ supports -c -o file.o... yes
checking if g++ supports -c -o file.o... (cached) yes
checking whether the g++ linker (/usr/bin/ld -m elf_x86_64) supports shared libraries... yes
checking dynamic linker characteristics... (cached) GNU/Linux ld.so
checking how to hardcode library paths into programs... immediate
checking for pkg-config... /usr/bin/pkg-config
checking pkg-config is at least version 0.9.0... yes
checking for sqrt in -lm... yes
checking for pthread_create in -lpthread... yes
checking for mxmlNewXML in -lmxml... yes
checking for mxml.h... yes
checking for qmake-qt5... no
checking for qmake5... no
checking for qmake... /usr/bin/qmake
checking for gzopen in -lz... no
checking for libusb_init in -lusb-1.0... yes
checking for cwiid_open in -lcwiid... yes
checking for lo_send_bundle in -llo... yes
checking for lo_bundle_free_recursive in -llo... yes
checking for stdint.h... (cached) yes
checking for assert.h... yes
checking for stdlib.h... (cached) yes
checking for unistd.h... (cached) yes
checking for netdb.h... yes
checking for sys/socket.h... yes
checking for fcntl.h... yes
checking for sys/ioctl.h... yes
checking for atexit... yes
checking for memset... yes
checking for select... yes
checking for socket... yes
checking for strerror... yes
checking for gettimeofday... yes
checking for _Bool... yes
checking for stdbool.h that conforms to C99 or later... yes
checking for uint16_t... yes
checking for uint32_t... yes
checking for uint8_t... yes
checking for int8_t... yes
checking for int32_t... yes
checking for size_t... yes
checking for inline... inline
checking for struct v4l2_frmsizeenum... yes
checking for libv4l2.h... yes
checking for linux/uinput.h... yes
checking for wine64... no
checking for makensis... no
checking for XPLMPlugin.h... no
checking 32bit winegcc... no
checking 32bit wineg++... no
checking for opencv4 >= 0.29.0... no
Opencl not present.
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating src/Makefile
config.status: creating src/pathconfig.h
config.status: creating src/mac/Makefile
config.status: creating src/mac/Info.plist
config.status: creating src/osc_server_mac/Makefile
config.status: creating src/osc_server_mac/OscServer-Info.plist
config.status: creating src/local_config.h
config.status: creating src/qt_gui/ltr_gui.pro
config.status: creating src/wii_server/wii_server.pro
config.status: creating src/wine_bridge/Makefile
config.status: creating src/wine_bridge/client/Makefile
config.status: creating src/wine_bridge/ft_client/Makefile
config.status: creating src/wine_bridge/controller/Makefile
config.status: creating src/wine_bridge/tester/Makefile
config.status: creating src/wine_bridge/ft_tester/Makefile
config.status: creating src/wine_bridge/tester/npview.rc
config.status: creating src/wine_bridge/ft_tester/fttester.rc
config.status: creating src/wine_bridge/views/Makefile
config.status: creating src/wine_bridge/ltr_wine64.nsi
config.status: creating src/wine_bridge/ltr_wine.nsi
config.status: creating doc/Makefile
config.status: creating src/linuxtrack.pc
config.status: creating src/mickey/mickey.pro
config.status: creating config.h
config.status: config.h is unchanged
config.status: executing libtool commands
configure: ===============================================================
configure: ===                         Summary                         ===
configure: ===============================================================
checking Webcam support... yes
checking Wiimote support... yes
checking TrackIR support... no
checking Facetracker support... no
checking XPlane plugin... no
checking Mickey... yes
checking Wine plugin... no
checking OSC support... yes

NOTE no wine support
which wine returns
/usr/bin/wine
wine --version 
wine-9.0 (Ubuntu 9.0~repack-4build3)

Logfile from make -j$(nproc)
make -j$(nproc)
make  all-recursive
make[1]: Entering directory '/home/mario/linuxtrack-test'
Making all in src
make[2]: Entering directory '/home/mario/linuxtrack-test/src'
make  all-am
make[3]: Entering directory '/home/mario/linuxtrack-test/src'
/bin/bash ../libtool  --tag=CXX   --mode=link g++  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols-regex '^ltr_int'     -o libltr.la -rpath /usr/local/lib/linuxtrack libltr_la-cal.lo libltr_la-list.lo libltr_la-dyn_load.lo libltr_la-math_utils.lo libltr_la-pose.lo libltr_la-pref.lo libltr_la-pref_bison.lo libltr_la-pref_flex.lo libltr_la-pref_global.lo libltr_la-utils.lo libltr_la-image_process.lo libltr_la-tracking.lo libltr_la-ltlib_int.lo libltr_la-spline.lo libltr_la-axis.lo libltr_la-wii_driver_prefs.lo libltr_la-tir_driver_prefs.lo libltr_la-wc_driver_prefs.lo libltr_la-ipc_utils.lo libltr_la-com_proc.lo libltr_la-wii_com.lo libltr_la-joy_driver_prefs.lo libltr_la-ps3_prefs.lo -lm -lpthread -ldl 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -fpic -fPIC '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./liblt.sym"   -o liblinuxtrack.la -rpath /usr/local/lib/linuxtrack liblinuxtrack_la-ltlib.lo liblinuxtrack_la-utils.lo liblinuxtrack_la-ipc_utils.lo  
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack32_la-ltlib.lo `test -f 'ltlib.c' || echo './'`ltlib.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack32_la-utils.lo `test -f 'utils.c' || echo './'`utils.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack32_la-ipc_utils.lo `test -f 'ipc_utils.c' || echo './'`ipc_utils.c
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -o liblinuxtrack_int.la  linuxtrack.lo  
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2   -pthread -llo -ldl  -o osc_server osc_server-osc_server.o osc_server-linuxtrack.o  
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-macwii_driver.lo `test -f 'macwii_driver.c' || echo './'`macwii_driver.c
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -pthread  -o ltr_extractor ltr_extractor-hashing.o ltr_extractor-digest.o ltr_extractor-game_data.o ltr_extractor-utils.o ltr_extractor-extract.o -lmxml 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -ldl  -o ltr_pipe ltr_pipe-ltr_pipe.o ltr_pipe-utils.o ltr_pipe-linuxtrack.o  
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-wii_driver_prefs.lo `test -f 'wii_driver_prefs.c' || echo './'`wii_driver_prefs.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o runloop.lo runloop.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o joy.lo joy.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwii_la-wiimote_driver.lo `test -f 'wiimote_driver.c' || echo './'`wiimote_driver.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwii_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c  -fPIC -DPIC -o .libs/liblinuxtrack32_la-ipc_utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c  -fPIC -DPIC -o .libs/liblinuxtrack32_la-utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy.c  -fPIC -DPIC -o .libs/joy.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/runloop.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c  -fPIC -DPIC -o .libs/libmacwii_la-wii_driver_prefs.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c macwii_driver.c  -fPIC -DPIC -o .libs/libmacwii_la-macwii_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib.c  -fPIC -DPIC -o .libs/liblinuxtrack32_la-ltlib.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wiimote_driver.c  -fPIC -DPIC -o .libs/libwii_la-wiimote_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libmacwii_la-runloop.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libwii_la-runloop.o
In file included from /usr/include/features.h:415,
                 from /usr/include/unistd.h:25,
                 from ipc_utils.c:8:
/usr/include/features-time64.h:20:10: fatal error: bits/wordsize.h: No such file or directory
   20 | #include <bits/wordsize.h>
      |          ^~~~~~~~~~~~~~~~~
compilation terminated.
In file included from utils.c:9:
/usr/include/stdio.h:28:10: fatal error: bits/libc-header-start.h: No such file or directory
   28 | #include <bits/libc-header-start.h>
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
make[3]: *** [Makefile:1138: liblinuxtrack32_la-ipc_utils.lo] Error 1
make[3]: *** Waiting for unfinished jobs....
In file included from ltlib.c:1:
/usr/include/stdlib.h:26:10: fatal error: bits/libc-header-start.h: No such file or directory
   26 | #include <bits/libc-header-start.h>
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
make[3]: *** [Makefile:1135: liblinuxtrack32_la-utils.lo] Error 1
make[3]: *** [Makefile:1132: liblinuxtrack32_la-ltlib.lo] Error 1
libtool: link: gcc -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o osc_server osc_server-osc_server.o osc_server-linuxtrack.o  -llo -ldl -pthread
libtool: link: gcc -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o ltr_pipe ltr_pipe-ltr_pipe.o ltr_pipe-utils.o ltr_pipe-linuxtrack.o  -ldl
libtool: link: gcc -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o ltr_extractor ltr_extractor-hashing.o ltr_extractor-digest.o ltr_extractor-game_data.o ltr_extractor-utils.o ltr_extractor-extract.o  -lmxml -pthread
libtool: link: ar cr .libs/liblinuxtrack_int.a .libs/linuxtrack.o 
libtool: link: ranlib .libs/liblinuxtrack_int.a
libtool: link: rm -fr  .libs/liblinuxtrack.a .libs/liblinuxtrack.lai .libs/liblinuxtrack.so .libs/liblinuxtrack.so.0 .libs/liblinuxtrack.so.0.0.0 .libs/liblinuxtrack.ver
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack_int.la" && ln -s "../liblinuxtrack_int.la" "liblinuxtrack_int.la" )
libtool: link: echo "{ global:" > .libs/liblinuxtrack.ver
libtool: link:  cat ./liblt.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/liblinuxtrack.ver
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c -o libmacwii_la-wii_driver_prefs.o >/dev/null 2>&1
libtool: link:  echo "local: *; };" >> .libs/liblinuxtrack.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/liblinuxtrack_la-ltlib.o .libs/liblinuxtrack_la-utils.o .libs/liblinuxtrack_la-ipc_utils.o    -g -O2 -fstack-protector   -Wl,-soname -Wl,liblinuxtrack.so.0 -Wl,-version-script -Wl,.libs/liblinuxtrack.ver -o .libs/liblinuxtrack.so.0.0.0
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c macwii_driver.c -o libmacwii_la-macwii_driver.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libmacwii_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libwii_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o runloop.o >/dev/null 2>&1
libtool: link: (cd ".libs" && rm -f "liblinuxtrack.so.0" && ln -s "liblinuxtrack.so.0.0.0" "liblinuxtrack.so.0")
libtool: link: (cd ".libs" && rm -f "liblinuxtrack.so" && ln -s "liblinuxtrack.so.0.0.0" "liblinuxtrack.so")
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wiimote_driver.c -o libwii_la-wiimote_driver.o >/dev/null 2>&1
libtool: link: ar cr .libs/liblinuxtrack.a  liblinuxtrack_la-ltlib.o liblinuxtrack_la-utils.o liblinuxtrack_la-ipc_utils.o
libtool: link: ranlib .libs/liblinuxtrack.a
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack.la" && ln -s "../liblinuxtrack.la" "liblinuxtrack.la" )
libtool: link: rm -fr  .libs/libltr.a .libs/libltr.exp .libs/libltr.lai .libs/libltr.so .libs/libltr.so.0 .libs/libltr.so.0.0.0
libtool: link: /usr/bin/nm -B  .libs/libltr_la-cal.o .libs/libltr_la-list.o .libs/libltr_la-dyn_load.o .libs/libltr_la-math_utils.o .libs/libltr_la-pose.o .libs/libltr_la-pref.o .libs/libltr_la-pref_bison.o .libs/libltr_la-pref_flex.o .libs/libltr_la-pref_global.o .libs/libltr_la-utils.o .libs/libltr_la-image_process.o .libs/libltr_la-tracking.o .libs/libltr_la-ltlib_int.o .libs/libltr_la-spline.o .libs/libltr_la-axis.o .libs/libltr_la-wii_driver_prefs.o .libs/libltr_la-tir_driver_prefs.o .libs/libltr_la-wc_driver_prefs.o .libs/libltr_la-ipc_utils.o .libs/libltr_la-com_proc.o .libs/libltr_la-wii_com.o .libs/libltr_la-joy_driver_prefs.o .libs/libltr_la-ps3_prefs.o   | /usr/bin/sed -n -e 's/^.*[       ]\([ABCDGIRSTW][ABCDGIRSTW]*\)[         ][      ]*\([_A-Za-z][_A-Za-z0-9]*\)$/\1 \2 \2/p' | /usr/bin/sed '/ __gnu_lto/d' | /usr/bin/sed 's/.* //' | sort | uniq > .libs/libltr.exp
libtool: link: /usr/bin/grep -E -e "^ltr_int" ".libs/libltr.exp" > ".libs/libltr.expT"
libtool: link: mv -f ".libs/libltr.expT" ".libs/libltr.exp"
libtool: link: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libltr_la-cal.o .libs/libltr_la-list.o .libs/libltr_la-dyn_load.o .libs/libltr_la-math_utils.o .libs/libltr_la-pose.o .libs/libltr_la-pref.o .libs/libltr_la-pref_bison.o .libs/libltr_la-pref_flex.o .libs/libltr_la-pref_global.o .libs/libltr_la-utils.o .libs/libltr_la-image_process.o .libs/libltr_la-tracking.o .libs/libltr_la-ltlib_int.o .libs/libltr_la-spline.o .libs/libltr_la-axis.o .libs/libltr_la-wii_driver_prefs.o .libs/libltr_la-tir_driver_prefs.o .libs/libltr_la-wc_driver_prefs.o .libs/libltr_la-ipc_utils.o .libs/libltr_la-com_proc.o .libs/libltr_la-wii_com.o .libs/libltr_la-joy_driver_prefs.o .libs/libltr_la-ps3_prefs.o   -lpthread -ldl -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libltr.so.0 -Wl,-retain-symbols-file -Wl,.libs/libltr.exp -o .libs/libltr.so.0.0.0
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy.c -o joy.o >/dev/null 2>&1
libtool: link: (cd ".libs" && rm -f "libltr.so.0" && ln -s "libltr.so.0.0.0" "libltr.so.0")
libtool: link: (cd ".libs" && rm -f "libltr.so" && ln -s "libltr.so.0.0.0" "libltr.so")
libtool: link: ar cr .libs/libltr.a  libltr_la-cal.o libltr_la-list.o libltr_la-dyn_load.o libltr_la-math_utils.o libltr_la-pose.o libltr_la-pref.o libltr_la-pref_bison.o libltr_la-pref_flex.o libltr_la-pref_global.o libltr_la-utils.o libltr_la-image_process.o libltr_la-tracking.o libltr_la-ltlib_int.o libltr_la-spline.o libltr_la-axis.o libltr_la-wii_driver_prefs.o libltr_la-tir_driver_prefs.o libltr_la-wc_driver_prefs.o libltr_la-ipc_utils.o libltr_la-com_proc.o libltr_la-wii_com.o libltr_la-joy_driver_prefs.o libltr_la-ps3_prefs.o
libtool: link: ranlib .libs/libltr.a
libtool: link: ( cd ".libs" && rm -f "libltr.la" && ln -s "../libltr.la" "libltr.la" )
make[3]: Leaving directory '/home/mario/linuxtrack-test/src'
make[2]: *** [Makefile:862: all] Error 2
make[2]: Leaving directory '/home/mario/linuxtrack-test/src'
make[1]: *** [Makefile:505: all-recursive] Error 1
make[1]: Leaving directory '/home/mario/linuxtrack-test'
make: *** [Makefile:410: all] Error 2

Key Points

    It seems likely that the installation of LinuxTrack on Ubuntu 25.10 is failing due to missing 32-bit development libraries, as the build process is attempting to compile 32-bit components.
    Research suggests that installing packages like gcc-multilib and libc6-dev-i386 may resolve the issue by providing necessary headers like bits/wordsize.h and bits/libc-header-start.h.
    The evidence leans toward compatibility issues with Ubuntu 25.10, a future release, potentially affecting 32-bit support, but standard fixes for earlier versions should still apply.

Direct Answer
Overview

The log file indicates that the installation of LinuxTrack on Ubuntu 25.10 is failing due to missing header files, specifically bits/wordsize.h and bits/libc-header-start.h, during the compilation of 32-bit libraries. This suggests that the system lacks necessary 32-bit development packages.
Likely Cause

It appears that the build process is trying to compile 32-bit components (liblinuxtrack32_la-*.lo), but the required 32-bit development libraries are not installed. These headers are part of the GNU C Library (glibc), and their absence typically points to missing multilib support.

## Recommended Improvements

### Ubuntu/Debian Build Requirements

Based on testing on Kubuntu 25.10, the following packages are required before building:

#### Essential Dependencies
```bash
# Install wine first (required for wine bridge support)
sudo apt install wine

# Install 32-bit development libraries
sudo apt install gcc-multilib libc6-dev-i386

# Install other build dependencies
sudo apt install build-essential autotools-dev libtool pkg-config
sudo apt install libmxml-dev libusb-1.0-0-dev libcwiid-dev liblo-dev
sudo apt install qt5-qmake qtbase5-dev qtbase5-dev-tools
sudo apt install libv4l-dev
```

#### Wine Development Support
For full wine bridge functionality, consider installing wine development packages:
```bash
sudo apt install wine-dev
# or
sudo apt install winehq-devel
```

### Build Process Improvements Needed

1. **Configure Script Enhancement**
   - Improve wine detection logic in `configure.ac`
   - Add proper 32-bit library dependency checks
   - Better error messages for missing multilib support

2. **Documentation Updates**
   - Add distro-specific dependency installation guides
   - Clarify wine installation requirements
   - Document multilib requirements

3. **Build Script Enhancement**
   - Add dependency checking in `build.sh`
   - Automatic package installation prompts
   - Better error handling for missing 32-bit support

### Troubleshooting Ubuntu 25.10+ Issues

If you encounter the `bits/wordsize.h` or `bits/libc-header-start.h` errors:

```bash
# Install 32-bit development support
sudo apt install gcc-multilib libc6-dev-i386

# Verify 32-bit headers are available
ls /usr/include/bits/wordsize.h
ls /usr/include/bits/libc-header-start.h
```

If wine bridge support is needed but not detected:
```bash
# Verify wine installation
wine --version
which wine

# Check wine development packages
dpkg -l | grep wine
```

### Next Testing Priority

The build system needs updating to handle these Ubuntu/Debian requirements automatically. Testing should focus on:

1. Enhanced dependency detection in configure script
2. Automatic package installation prompts
3. Better error messages for missing dependencies
4. Wine bridge functionality once build succeeds

Your testing has identified critical gaps in cross-platform compatibility that will significantly improve the user experience for Ubuntu/Debian users! 🎯
