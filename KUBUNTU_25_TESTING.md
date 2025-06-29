# 🐧 LinuxTrack Qt5 Testing - Kubuntu 25.10 Quick Start

**Target**: Kubuntu 25.10  
**Phase**: Quick validation testing  
**Time Required**: ~30-45 minutes

---

## ⚡ **QUICK START - Kubuntu 25.10**

### **Step 1: System Preparation (5 minutes)**
```bash
# Update system and install dependencies
sudo apt update && sudo apt upgrade -y

# Install Qt5 and build tools
sudo apt install -y build-essential autoconf automake libtool pkg-config
sudo apt install -y qtbase5-dev qttools5-dev qttools5-dev-tools
sudo apt install -y libqt5help5 qt5-assistant libopencv-dev libusb-1.0-0-dev

# Verify Qt5 installation
qmake-qt5 --version && qhelpgenerator-qt5 --help | head -2
```
RESUlTS
qmake-qt5: command not found
Replaced with
sudo app install qt5-qmake
qt5-qmake --version
installed but qt5-qmake command not found
qhelpgenerator-qt5: command not found

### **Step 2: Source Setup (2 minutes)**
```bash
# Copy source to test location
cd /tmp
cp -r /media/mario/Local_Git/git-repos/linuxtrack-clean-june14 ./linuxtrack-kubuntu-test
cd linuxtrack-kubuntu-test

# Clean previous builds
make clean 2>/dev/null || true
```

### **Step 3: Build Test (5-10 minutes)**
```bash
# Configure
./configure --prefix=/opt/linuxtrack

# Build (time it)
echo "Build started: $(date)"
time make -j$(nproc)
echo "Build finished: $(date)"

# Quick verification
echo "=== BUILD CHECK ==="
ls -la src/qt_gui/ltr_gui 2>/dev/null && echo "✅ ltr_gui built" || echo "❌ ltr_gui missing"
ls -la src/mickey/mickey 2>/dev/null && echo "✅ mickey built" || echo "❌ mickey missing"
ls -la src/qt_gui/help.qch 2>/dev/null && echo "✅ help files built" || echo "❌ help files missing"
```
Results
# Clean previous builds
make clean 2>/dev/null || true
CDPATH="${ZSH_VERSION+.}:" && cd . && /bin/sh '/mnt/Local_Git/git-repos/linuxtrack-clean-june14/missing' aclocal-1.17 
mario@mario-g707:/tmp/linuxtrack-kubuntu-test$ # Configure
./configure --prefix=/opt/linuxtrack

# Build (time it)
echo "Build started: $(date)"
time make -j$(nproc)
echo "Build finished: $(date)"

# Quick verification
echo "=== BUILD CHECK ==="
ls -la src/qt_gui/ltr_gui 2>/dev/null && echo "✅ ltr_gui built" || echo "❌ ltr_gui missing"
ls -la src/mickey/mickey 2>/dev/null && echo "✅ mickey built" || echo "❌ mickey missing"
ls -la src/qt_gui/help.qch 2>/dev/null && echo "✅ help files built" || echo "❌ help files missing"
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
checking for gzopen in -lz... yes
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
checking for opencv4 >= 0.29.0... yes
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
checking TrackIR support... yes
checking Facetracker support... yes
checking XPlane plugin... no
checking Mickey... yes
checking Wine plugin... no
checking OSC support... yes
Build started: Sun Jun 29 08:30:21 AM EDT 2025
CDPATH="${ZSH_VERSION+.}:" && cd . && /bin/bash '/tmp/linuxtrack-kubuntu-test/missing' aclocal-1.17 
CDPATH="${ZSH_VERSION+.}:" && cd . && /bin/bash '/tmp/linuxtrack-kubuntu-test/missing' autoconf
 cd . && /bin/bash /tmp/linuxtrack-kubuntu-test/missing automake-1.17 --gnu --ignore-deps
/bin/bash ./config.status --recheck
running CONFIG_SHELL=/bin/bash /bin/bash ./configure --prefix=/opt/linuxtrack --no-create --no-recursion
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
checking for gzopen in -lz... yes
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
checking for opencv4 >= 0.29.0... yes
checking that generated files are newer than configure... done
configure: creating ./config.status
configure: ===============================================================
configure: ===                         Summary                         ===
configure: ===============================================================
checking Webcam support... yes
checking Wiimote support... yes
checking TrackIR support... yes
checking Facetracker support... yes
checking XPlane plugin... no
checking Mickey... yes
checking Wine plugin... no
checking OSC support... yes
 /bin/bash ./config.status
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
(CDPATH="${ZSH_VERSION+.}:" && cd . && /bin/bash '/tmp/linuxtrack-kubuntu-test/missing' autoheader)
rm -f stamp-h1
touch config.h.in
rm -f stamp-h1
cd . && /bin/bash ./config.status config.h
config.status: creating config.h
config.status: config.h is unchanged
make  all-recursive
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test'
Making all in src
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
/bin/bash ../ylwrap pref_flex.lpp lex.yy.c pref_flex.cpp -- flex -i 
/bin/bash ../ylwrap pref_bison.ypp y.tab.c pref_bison.cpp y.tab.h `echo pref_bison.cpp | sed -e s/cc$/hh/ -e s/cpp$/hpp/ -e s/cxx$/hxx/ -e s/c++$/h++/ -e s/c$/h/` y.output pref_bison.output -- bison -y -d --name-prefix ltr_int_parser_ 
/tmp/linuxtrack-kubuntu-test/src/pref_bison.ypp:11.1-7: warning: POSIX Yacc does not support %define [-Wyacc]
   11 | %define api.pure
      | ^~~~~~~
/tmp/linuxtrack-kubuntu-test/src/pref_bison.ypp:13.1-7: warning: POSIX Yacc does not support %define [-Wyacc]
   13 | %define parse.error verbose 
      | ^~~~~~~
/tmp/linuxtrack-kubuntu-test/src/pref_bison.ypp:33.13-25: warning: POSIX yacc reserves %type to nonterminals [-Wyacc]
   33 | %type <str> TOKEN_COMMENT TOKEN_KEY TOKEN_VALUE TOKEN_SECNAME
      |             ^~~~~~~~~~~~~
/tmp/linuxtrack-kubuntu-test/src/pref_bison.ypp:33.27-35: warning: POSIX yacc reserves %type to nonterminals [-Wyacc]
   33 | %type <str> TOKEN_COMMENT TOKEN_KEY TOKEN_VALUE TOKEN_SECNAME
      |                           ^~~~~~~~~
/tmp/linuxtrack-kubuntu-test/src/pref_bison.ypp:33.37-47: warning: POSIX yacc reserves %type to nonterminals [-Wyacc]
   33 | %type <str> TOKEN_COMMENT TOKEN_KEY TOKEN_VALUE TOKEN_SECNAME
      |                                     ^~~~~~~~~~~
/tmp/linuxtrack-kubuntu-test/src/pref_bison.ypp:33.49-61: warning: POSIX yacc reserves %type to nonterminals [-Wyacc]
   33 | %type <str> TOKEN_COMMENT TOKEN_KEY TOKEN_VALUE TOKEN_SECNAME
      |                                                 ^~~~~~~~~~~~~
pref_bison.hpp is unchanged
make  all-am
make[3]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_server1-ltr_srv_comm.o `test -f 'ltr_srv_comm.c' || echo './'`ltr_srv_comm.c
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_server1-ltr_srv_slave.o `test -f 'ltr_srv_slave.c' || echo './'`ltr_srv_slave.c
g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_srv_master.o ltr_srv_master.cpp
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_server1-ltr_server1.o `test -f 'ltr_server1.c' || echo './'`ltr_server1.c
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_server1-ipc_utils.o `test -f 'ipc_utils.c' || echo './'`ipc_utils.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-cal.lo `test -f 'cal.c' || echo './'`cal.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-list.lo `test -f 'list.c' || echo './'`list.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-dyn_load.lo `test -f 'dyn_load.c' || echo './'`dyn_load.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-math_utils.lo `test -f 'math_utils.c' || echo './'`math_utils.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-pose.lo `test -f 'pose.c' || echo './'`pose.c
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-pref.lo `test -f 'pref.cpp' || echo './'`pref.cpp
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-pref_bison.lo `test -f 'pref_bison.cpp' || echo './'`pref_bison.cpp
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-pref_flex.lo `test -f 'pref_flex.cpp' || echo './'`pref_flex.cpp
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-pref_global.lo `test -f 'pref_global.c' || echo './'`pref_global.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-utils.lo `test -f 'utils.c' || echo './'`utils.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-image_process.lo `test -f 'image_process.c' || echo './'`image_process.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c cal.c  -fPIC -DPIC -o .libs/libltr_la-cal.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c  -fPIC -DPIC -o .libs/libltr_la-utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c dyn_load.c  -fPIC -DPIC -o .libs/libltr_la-dyn_load.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c list.c  -fPIC -DPIC -o .libs/libltr_la-list.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c math_utils.c  -fPIC -DPIC -o .libs/libltr_la-math_utils.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-tracking.lo `test -f 'tracking.c' || echo './'`tracking.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_global.c  -fPIC -DPIC -o .libs/libltr_la-pref_global.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pose.c  -fPIC -DPIC -o .libs/libltr_la-pose.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c image_process.c  -fPIC -DPIC -o .libs/libltr_la-image_process.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref.cpp  -fPIC -DPIC -o .libs/libltr_la-pref.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_bison.cpp  -fPIC -DPIC -o .libs/libltr_la-pref_bison.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_flex.cpp  -fPIC -DPIC -o .libs/libltr_la-pref_flex.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-ltlib_int.lo `test -f 'ltlib_int.c' || echo './'`ltlib_int.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c dyn_load.c -o libltr_la-dyn_load.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tracking.c  -fPIC -DPIC -o .libs/libltr_la-tracking.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib_int.c  -fPIC -DPIC -o .libs/libltr_la-ltlib_int.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c list.c -o libltr_la-list.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-spline.lo `test -f 'spline.c' || echo './'`spline.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-axis.lo `test -f 'axis.c' || echo './'`axis.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c cal.c -o libltr_la-cal.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_global.c -o libltr_la-pref_global.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-wii_driver_prefs.lo `test -f 'wii_driver_prefs.c' || echo './'`wii_driver_prefs.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c spline.c  -fPIC -DPIC -o .libs/libltr_la-spline.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c axis.c  -fPIC -DPIC -o .libs/libltr_la-axis.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c -o libltr_la-utils.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib_int.c -o libltr_la-ltlib_int.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c math_utils.c -o libltr_la-math_utils.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-tir_driver_prefs.lo `test -f 'tir_driver_prefs.c' || echo './'`tir_driver_prefs.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c spline.c -o libltr_la-spline.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-wc_driver_prefs.lo `test -f 'wc_driver_prefs.c' || echo './'`wc_driver_prefs.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c  -fPIC -DPIC -o .libs/libltr_la-wii_driver_prefs.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c image_process.c -o libltr_la-image_process.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pose.c -o libltr_la-pose.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tracking.c -o libltr_la-tracking.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wc_driver_prefs.c  -fPIC -DPIC -o .libs/libltr_la-wc_driver_prefs.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-ipc_utils.lo `test -f 'ipc_utils.c' || echo './'`ipc_utils.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c -o libltr_la-wii_driver_prefs.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_driver_prefs.c  -fPIC -DPIC -o .libs/libltr_la-tir_driver_prefs.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-com_proc.lo `test -f 'com_proc.c' || echo './'`com_proc.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-wii_com.lo `test -f 'wii_com.c' || echo './'`wii_com.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c  -fPIC -DPIC -o .libs/libltr_la-ipc_utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c com_proc.c  -fPIC -DPIC -o .libs/libltr_la-com_proc.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-joy_driver_prefs.lo `test -f 'joy_driver_prefs.c' || echo './'`joy_driver_prefs.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_driver_prefs.c -o libltr_la-tir_driver_prefs.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_com.c  -fPIC -DPIC -o .libs/libltr_la-wii_com.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltr_la-ps3_prefs.lo `test -f 'ps3_prefs.c' || echo './'`ps3_prefs.c
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o osc_server-osc_server.o `test -f 'osc_server.c' || echo './'`osc_server.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wc_driver_prefs.c -o libltr_la-wc_driver_prefs.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c axis.c -o libltr_la-axis.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy_driver_prefs.c  -fPIC -DPIC -o .libs/libltr_la-joy_driver_prefs.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_com.c -o libltr_la-wii_com.o >/dev/null 2>&1
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o osc_server-linuxtrack.o `test -f 'linuxtrack.c' || echo './'`linuxtrack.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3_prefs.c  -fPIC -DPIC -o .libs/libltr_la-ps3_prefs.o
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_recenter-ltr_recenter.o `test -f 'ltr_recenter.c' || echo './'`ltr_recenter.c
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_recenter-ltr_srv_comm.o `test -f 'ltr_srv_comm.c' || echo './'`ltr_srv_comm.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c com_proc.c -o libltr_la-com_proc.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -fpic -fPIC '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack_la-ltlib.lo `test -f 'ltlib.c' || echo './'`ltlib.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -fpic -fPIC '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack_la-utils.lo `test -f 'utils.c' || echo './'`utils.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy_driver_prefs.c -o libltr_la-joy_driver_prefs.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -fpic -fPIC '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack_la-ipc_utils.lo `test -f 'ipc_utils.c' || echo './'`ipc_utils.c
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_extractor-hashing.o `test -f 'hashing.c' || echo './'`hashing.c
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_extractor-digest.o `test -f 'digest.c' || echo './'`digest.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3_prefs.c -o libltr_la-ps3_prefs.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c -o libltr_la-ipc_utils.o >/dev/null 2>&1
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_extractor-game_data.o `test -f 'game_data.c' || echo './'`game_data.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c  -fPIC -DPIC -o .libs/liblinuxtrack_la-utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib.c  -fPIC -DPIC -o .libs/liblinuxtrack_la-ltlib.o
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_extractor-utils.o `test -f 'utils.c' || echo './'`utils.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c  -fPIC -DPIC -o .libs/liblinuxtrack_la-ipc_utils.o
gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_extractor-extract.o `test -f 'extract.c' || echo './'`extract.c
ltlib.c:279:23: warning: conflicting types for 'ltr_wakeup' due to enum/integer mismatch; have 'linuxtrack_state_type(void)' [-Wenum-int-mismatch]
  279 | linuxtrack_state_type ltr_wakeup(void)
      |                       ^~~~~~~~~~
ltlib.c:33:5: note: previous declaration of 'ltr_wakeup' with type 'int(void)'
   33 | int ltr_wakeup(void);
      |     ^~~~~~~~~~
gcc -DHAVE_CONFIG_H -I. -I..  -DLINUX -D_GNU_SOURCE -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_pipe-ltr_pipe.o `test -f 'ltr_pipe.c' || echo './'`ltr_pipe.c
gcc -DHAVE_CONFIG_H -I. -I..  -DLINUX -D_GNU_SOURCE -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_pipe-utils.o `test -f 'utils.c' || echo './'`utils.c
gcc -DHAVE_CONFIG_H -I. -I..  -DLINUX -D_GNU_SOURCE -D_FORTIFY_SOURCE=2 -O2   '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o ltr_pipe-linuxtrack.o `test -f 'linuxtrack.c' || echo './'`linuxtrack.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack32_la-ltlib.lo `test -f 'ltlib.c' || echo './'`ltlib.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack32_la-utils.lo `test -f 'utils.c' || echo './'`utils.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib.c  -fPIC -DPIC -o .libs/liblinuxtrack32_la-ltlib.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c  -fPIC -DPIC -o .libs/liblinuxtrack32_la-utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c -o liblinuxtrack_la-utils.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack32_la-ipc_utils.lo `test -f 'ipc_utils.c' || echo './'`ipc_utils.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c -o liblinuxtrack_la-ipc_utils.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o linuxtrack.lo linuxtrack.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib.c -o liblinuxtrack_la-ltlib.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  -I/usr/include/opencv4  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfacetrack_int_la-facetrack.lo `test -f 'facetrack.cpp' || echo './'`facetrack.cpp
ltlib.c:279:23: warning: conflicting types for 'ltr_wakeup' due to enum/integer mismatch; have 'linuxtrack_state_type(void)' [-Wenum-int-mismatch]
  279 | linuxtrack_state_type ltr_wakeup(void)
      |                       ^~~~~~~~~~
ltlib.c:33:5: note: previous declaration of 'ltr_wakeup' with type 'int(void)'
   33 | int ltr_wakeup(void);
      |     ^~~~~~~~~~
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-macwii_driver.lo `test -f 'macwii_driver.c' || echo './'`macwii_driver.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-wii_driver_prefs.lo `test -f 'wii_driver_prefs.c' || echo './'`wii_driver_prefs.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c  -fPIC -DPIC -o .libs/liblinuxtrack32_la-ipc_utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c linuxtrack.c  -fPIC -DPIC -o .libs/linuxtrack.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c facetrack.cpp  -fPIC -DPIC -o .libs/libfacetrack_int_la-facetrack.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c macwii_driver.c  -fPIC -DPIC -o .libs/libmacwii_la-macwii_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c  -fPIC -DPIC -o .libs/libmacwii_la-wii_driver_prefs.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libmacwii_la-runloop.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o runloop.lo runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib.c -o liblinuxtrack32_la-ltlib.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o joy.lo joy.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwii_la-wiimote_driver.lo `test -f 'wiimote_driver.c' || echo './'`wiimote_driver.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c -o libmacwii_la-wii_driver_prefs.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c macwii_driver.c -o libmacwii_la-macwii_driver.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/runloop.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_flex.cpp -o libltr_la-pref_flex.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c -o liblinuxtrack32_la-utils.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwii_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwc_la-webcam_driver.lo `test -f 'webcam_driver.c' || echo './'`webcam_driver.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c linuxtrack.c -o linuxtrack.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libmacwii_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy.c  -fPIC -DPIC -o .libs/joy.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wiimote_driver.c  -fPIC -DPIC -o .libs/libwii_la-wiimote_driver.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwc_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libwii_la-runloop.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c webcam_driver.c  -fPIC -DPIC -o .libs/libwc_la-webcam_driver.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o tir_hw.lo tir_hw.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o runloop.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_bison.cpp -o libltr_la-pref_bison.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c -o liblinuxtrack32_la-ipc_utils.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libwc_la-runloop.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o tir_img.lo tir_img.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libwii_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_hw.c  -fPIC -DPIC -o .libs/tir_hw.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_img.c  -fPIC -DPIC -o .libs/tir_img.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o tir_driver.lo tir_driver.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libltusb1_la-libusb_ifc.lo `test -f 'libusb_ifc.c' || echo './'`libusb_ifc.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -DOPENCV -I/usr/include/opencv4  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libft_la-webcam_driver.lo `test -f 'webcam_driver.c' || echo './'`webcam_driver.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libwc_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wiimote_driver.c -o libwii_la-wiimote_driver.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -DOPENCV -I/usr/include/opencv4  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libft_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c libusb_ifc.c  -fPIC -DPIC -o .libs/libltusb1_la-libusb_ifc.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_driver.c  -fPIC -DPIC -o .libs/tir_driver.o
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -DOPENCV -I/usr/include/opencv4  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libft_la-facetrack.lo `test -f 'facetrack.cpp' || echo './'`facetrack.cpp
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c webcam_driver.c  -fPIC -DPIC -o .libs/libft_la-webcam_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libft_la-runloop.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -DOPENCV -I/usr/include/opencv4  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libp3eft_la-ps3eye_driver.lo `test -f 'ps3eye_driver.c' || echo './'`ps3eye_driver.c
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c facetrack.cpp  -fPIC -DPIC -o .libs/libft_la-facetrack.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -DOPENCV -I/usr/include/opencv4  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libp3eft_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -DOPENCV -I/usr/include/opencv4  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libp3eft_la-facetrack.lo `test -f 'facetrack.cpp' || echo './'`facetrack.cpp
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_driver.c -o tir_driver.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy.c -o joy.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3eye_driver.c  -fPIC -DPIC -o .libs/libp3eft_la-ps3eye_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libft_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_img.c -o tir_img.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libp3eft_la-runloop.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c webcam_driver.c -o libwc_la-webcam_driver.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c facetrack.cpp  -fPIC -DPIC -o .libs/libp3eft_la-facetrack.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c libusb_ifc.c -o libltusb1_la-libusb_ifc.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfakeusb_la-fakeusb.lo `test -f 'fakeusb.c' || echo './'`fakeusb.c
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfakeusb_la-tir_model.lo `test -f 'tir_model.cpp' || echo './'`tir_model.cpp
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libp3eft_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c fakeusb.c  -fPIC -DPIC -o .libs/libfakeusb_la-fakeusb.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_model.cpp  -fPIC -DPIC -o .libs/libfakeusb_la-tir_model.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libp3e_la-ps3eye_driver.lo `test -f 'ps3eye_driver.c' || echo './'`ps3eye_driver.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c webcam_driver.c -o libft_la-webcam_driver.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libp3e_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd wii_server; /usr/bin/qmake -spec linux-g++ "LIBDIR=/opt/linuxtrack/lib/linuxtrack"; make; popd
/tmp/linuxtrack-kubuntu-test/src/wii_server /tmp/linuxtrack-kubuntu-test/src
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c fakeusb.c -o libfakeusb_la-fakeusb.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2   -pthread -llo -ldl  -o osc_server osc_server-osc_server.o osc_server-linuxtrack.o  
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3eye_driver.c  -fPIC -DPIC -o .libs/libp3e_la-ps3eye_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libp3e_la-runloop.o
Info: creating stash file /tmp/linuxtrack-kubuntu-test/src/wii_server/.qmake.stash
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3eye_driver.c -o libp3eft_la-ps3eye_driver.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_hw.c -o tir_hw.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -fpic -fPIC '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./liblt.sym"   -o liblinuxtrack.la -rpath /opt/linuxtrack/lib/linuxtrack liblinuxtrack_la-ltlib.lo liblinuxtrack_la-utils.lo liblinuxtrack_la-ipc_utils.lo  
libtool: link: gcc -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o osc_server osc_server-osc_server.o osc_server-linuxtrack.o  -llo -ldl -pthread
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libp3e_la-runloop.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -pthread  -o ltr_extractor ltr_extractor-hashing.o ltr_extractor-digest.o ltr_extractor-game_data.o ltr_extractor-utils.o ltr_extractor-extract.o -lmxml 
make[4]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/wii_server'
/usr/lib/qt5/bin/uic wii_server.ui -o ui_wii_server.h
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -ldl  -o ltr_pipe ltr_pipe-ltr_pipe.o ltr_pipe-utils.o ltr_pipe-linuxtrack.o  
g++ -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -dM -E -o moc_predefs.h /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/data/dummy.cpp
libtool: link: rm -fr  .libs/liblinuxtrack.a .libs/liblinuxtrack.la .libs/liblinuxtrack.lai .libs/liblinuxtrack.so .libs/liblinuxtrack.so.0 .libs/liblinuxtrack.so.0.0.0 .libs/liblinuxtrack.ver
/usr/lib/qt5/bin/rcc -name wii_server wii_server.qrc -o qrc_wii_server.cpp
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o main.o main.cpp
libtool: link: echo "{ global:" > .libs/liblinuxtrack.ver
libtool: link:  cat ./liblt.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/liblinuxtrack.ver
libtool: link:  echo "local: *; };" >> .libs/liblinuxtrack.ver
/bin/bash ../libtool  --tag=CC   --mode=link gcc '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./liblt.sym" -m32   -o liblinuxtrack32.la -rpath /opt/linuxtrack/lib32/linuxtrack liblinuxtrack32_la-ltlib.lo liblinuxtrack32_la-utils.lo liblinuxtrack32_la-ipc_utils.lo  
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/liblinuxtrack_la-ltlib.o .libs/liblinuxtrack_la-utils.o .libs/liblinuxtrack_la-ipc_utils.o    -g -O2 -fstack-protector   -Wl,-soname -Wl,liblinuxtrack.so.0 -Wl,-version-script -Wl,.libs/liblinuxtrack.ver -o .libs/liblinuxtrack.so.0.0.0
libtool: link: gcc -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o ltr_extractor ltr_extractor-hashing.o ltr_extractor-digest.o ltr_extractor-game_data.o ltr_extractor-utils.o ltr_extractor-extract.o  -lmxml -pthread
libtool: link: (cd ".libs" && rm -f "liblinuxtrack.so.0" && ln -s "liblinuxtrack.so.0.0.0" "liblinuxtrack.so.0")
libtool: link: (cd ".libs" && rm -f "liblinuxtrack.so" && ln -s "liblinuxtrack.so.0.0.0" "liblinuxtrack.so")
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -o liblinuxtrack_int.la  linuxtrack.lo  
/usr/lib/qt5/bin/moc -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/wii_server/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/wii_server -I/tmp/linuxtrack-kubuntu-test/src/wii_server -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/14 -I/usr/include/x86_64-linux-gnu/c++/14 -I/usr/include/c++/14/backward -I/usr/lib/gcc/x86_64-linux-gnu/14/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include wii_server.h -o moc_wii_server.cpp
libtool: link: ar cr .libs/liblinuxtrack.a  liblinuxtrack_la-ltlib.o liblinuxtrack_la-utils.o liblinuxtrack_la-ipc_utils.o
libtool: link: gcc -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o ltr_pipe ltr_pipe-ltr_pipe.o ltr_pipe-utils.o ltr_pipe-linuxtrack.o  -ldl
libtool: link: ranlib .libs/liblinuxtrack.a
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack.la" && ln -s "../liblinuxtrack.la" "liblinuxtrack.la" )
/usr/lib/qt5/bin/moc -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/wii_server/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/wii_server -I/tmp/linuxtrack-kubuntu-test/src/wii_server -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/14 -I/usr/include/x86_64-linux-gnu/c++/14 -I/usr/include/c++/14/backward -I/usr/lib/gcc/x86_64-linux-gnu/14/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include wiimote.h -o moc_wiimote.cpp
libtool: link: rm -fr  .libs/liblinuxtrack32.a .libs/liblinuxtrack32.la .libs/liblinuxtrack32.lai .libs/liblinuxtrack32.so .libs/liblinuxtrack32.so.0 .libs/liblinuxtrack32.so.0.0.0 .libs/liblinuxtrack32.ver
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./joy_driver.sym"   -o libjoy.la -rpath /opt/linuxtrack/lib/linuxtrack runloop.lo joy.lo  
libtool: link: echo "{ global:" > .libs/liblinuxtrack32.ver
libtool: link:  cat ./liblt.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/liblinuxtrack32.ver
libtool: link:  echo "local: *; };" >> .libs/liblinuxtrack32.ver
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3eye_driver.c -o libp3e_la-ps3eye_driver.o >/dev/null 2>&1
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o qrc_wii_server.o qrc_wii_server.cpp
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/liblinuxtrack32_la-ltlib.o .libs/liblinuxtrack32_la-utils.o .libs/liblinuxtrack32_la-ipc_utils.o    -m32 -g -O2 -fstack-protector -m32   -Wl,-soname -Wl,liblinuxtrack32.so.0 -Wl,-version-script -Wl,.libs/liblinuxtrack32.ver -o .libs/liblinuxtrack32.so.0.0.0
libtool: link: rm -fr  .libs/liblinuxtrack_int.a .libs/liblinuxtrack_int.la
libtool: link: ar cr .libs/liblinuxtrack_int.a .libs/linuxtrack.o 
libtool: link: ranlib .libs/liblinuxtrack_int.a
libtool: link: (cd ".libs" && rm -f "liblinuxtrack32.so.0" && ln -s "liblinuxtrack32.so.0.0.0" "liblinuxtrack32.so.0")
libtool: link: (cd ".libs" && rm -f "liblinuxtrack32.so" && ln -s "liblinuxtrack32.so.0.0.0" "liblinuxtrack32.so")
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack_int.la" && ln -s "../liblinuxtrack_int.la" "liblinuxtrack_int.la" )
libtool: link: ar cr .libs/liblinuxtrack32.a  liblinuxtrack32_la-ltlib.o liblinuxtrack32_la-utils.o liblinuxtrack32_la-ipc_utils.o
libtool: link: rm -fr  .libs/libjoy.a .libs/libjoy.la .libs/libjoy.lai .libs/libjoy.so .libs/libjoy.so.0 .libs/libjoy.so.0.0.0 .libs/libjoy.ver
libtool: link: ranlib .libs/liblinuxtrack32.a
libtool: link: echo "{ global:" > .libs/libjoy.ver
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack32.la" && ln -s "../liblinuxtrack32.la" "liblinuxtrack32.la" )
libtool: link:  cat ./joy_driver.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libjoy.ver
libtool: link:  echo "local: *; };" >> .libs/libjoy.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/runloop.o .libs/joy.o    -g -O2 -fstack-protector   -Wl,-soname -Wl,libjoy.so.0 -Wl,-version-script -Wl,.libs/libjoy.ver -o .libs/libjoy.so.0.0.0
libtool: link: (cd ".libs" && rm -f "libjoy.so.0" && ln -s "libjoy.so.0.0.0" "libjoy.so.0")
libtool: link: (cd ".libs" && rm -f "libjoy.so" && ln -s "libjoy.so.0.0.0" "libjoy.so")
libtool: link: ar cr .libs/libjoy.a  runloop.o joy.o
libtool: link: ranlib .libs/libjoy.a
libtool: link: ( cd ".libs" && rm -f "libjoy.la" && ln -s "../libjoy.la" "libjoy.la" )
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wiimote.o wiimote.cpp
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wii_server.o wii_server.cpp
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_model.cpp -o libfakeusb_la-tir_model.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref.cpp -o libltr_la-pref.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c facetrack.cpp -o libfacetrack_int_la-facetrack.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c facetrack.cpp -o libft_la-facetrack.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c facetrack.cpp -o libp3eft_la-facetrack.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CXX   --mode=link g++ -I/usr/include/opencv4  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -o libfacetrack_int.la  libfacetrack_int_la-facetrack.lo  
libtool: link: rm -fr  .libs/libfacetrack_int.a .libs/libfacetrack_int.la
libtool: link: ar cr .libs/libfacetrack_int.a .libs/libfacetrack_int_la-facetrack.o 
libtool: link: ranlib .libs/libfacetrack_int.a
libtool: link: ( cd ".libs" && rm -f "libfacetrack_int.la" && ln -s "../libfacetrack_int.la" "libfacetrack_int.la" )
g++  -o wii_server main.o wii_server.o wiimote.o qrc_wii_server.o   -L../.libs -L/opt/linuxtrack/lib/linuxtrack -lm -lltr -lcwiid -Wl,-rpath,/opt/linuxtrack/lib/linuxtrack /usr/lib/x86_64-linux-gnu/libQt5Widgets.so /usr/lib/x86_64-linux-gnu/libQt5Gui.so /usr/lib/x86_64-linux-gnu/libQt5Core.so -lGL -lpthread   
make[4]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/wii_server'
/tmp/linuxtrack-kubuntu-test/src
/bin/bash ../libtool  --tag=CXX   --mode=link g++  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols-regex '^ltr_int'     -o libltr.la -rpath /opt/linuxtrack/lib/linuxtrack libltr_la-cal.lo libltr_la-list.lo libltr_la-dyn_load.lo libltr_la-math_utils.lo libltr_la-pose.lo libltr_la-pref.lo libltr_la-pref_bison.lo libltr_la-pref_flex.lo libltr_la-pref_global.lo libltr_la-utils.lo libltr_la-image_process.lo libltr_la-tracking.lo libltr_la-ltlib_int.lo libltr_la-spline.lo libltr_la-axis.lo libltr_la-wii_driver_prefs.lo libltr_la-tir_driver_prefs.lo libltr_la-wc_driver_prefs.lo libltr_la-ipc_utils.lo libltr_la-com_proc.lo libltr_la-wii_com.lo libltr_la-joy_driver_prefs.lo libltr_la-ps3_prefs.lo -lm -lpthread -ldl 
libtool: link: rm -fr  .libs/libltr.a .libs/libltr.exp .libs/libltr.la .libs/libltr.lai .libs/libltr.so .libs/libltr.so.0 .libs/libltr.so.0.0.0
libtool: link: /usr/bin/nm -B  .libs/libltr_la-cal.o .libs/libltr_la-list.o .libs/libltr_la-dyn_load.o .libs/libltr_la-math_utils.o .libs/libltr_la-pose.o .libs/libltr_la-pref.o .libs/libltr_la-pref_bison.o .libs/libltr_la-pref_flex.o .libs/libltr_la-pref_global.o .libs/libltr_la-utils.o .libs/libltr_la-image_process.o .libs/libltr_la-tracking.o .libs/libltr_la-ltlib_int.o .libs/libltr_la-spline.o .libs/libltr_la-axis.o .libs/libltr_la-wii_driver_prefs.o .libs/libltr_la-tir_driver_prefs.o .libs/libltr_la-wc_driver_prefs.o .libs/libltr_la-ipc_utils.o .libs/libltr_la-com_proc.o .libs/libltr_la-wii_com.o .libs/libltr_la-joy_driver_prefs.o .libs/libltr_la-ps3_prefs.o   | /usr/bin/sed -n -e 's/^.*[       ]\([ABCDGIRSTW][ABCDGIRSTW]*\)[         ][      ]*\([_A-Za-z][_A-Za-z0-9]*\)$/\1 \2 \2/p' | /usr/bin/sed '/ __gnu_lto/d' | /usr/bin/sed 's/.* //' | sort | uniq > .libs/libltr.exp
libtool: link: /usr/bin/grep -E -e "^ltr_int" ".libs/libltr.exp" > ".libs/libltr.expT"
libtool: link: mv -f ".libs/libltr.expT" ".libs/libltr.exp"
libtool: link: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libltr_la-cal.o .libs/libltr_la-list.o .libs/libltr_la-dyn_load.o .libs/libltr_la-math_utils.o .libs/libltr_la-pose.o .libs/libltr_la-pref.o .libs/libltr_la-pref_bison.o .libs/libltr_la-pref_flex.o .libs/libltr_la-pref_global.o .libs/libltr_la-utils.o .libs/libltr_la-image_process.o .libs/libltr_la-tracking.o .libs/libltr_la-ltlib_int.o .libs/libltr_la-spline.o .libs/libltr_la-axis.o .libs/libltr_la-wii_driver_prefs.o .libs/libltr_la-tir_driver_prefs.o .libs/libltr_la-wc_driver_prefs.o .libs/libltr_la-ipc_utils.o .libs/libltr_la-com_proc.o .libs/libltr_la-wii_com.o .libs/libltr_la-joy_driver_prefs.o .libs/libltr_la-ps3_prefs.o   -lm -lpthread -ldl -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libltr.so.0 -Wl,-retain-symbols-file -Wl,.libs/libltr.exp -o .libs/libltr.so.0.0.0
libtool: link: (cd ".libs" && rm -f "libltr.so.0" && ln -s "libltr.so.0.0.0" "libltr.so.0")
libtool: link: (cd ".libs" && rm -f "libltr.so" && ln -s "libltr.so.0.0.0" "libltr.so")
libtool: link: ar cr .libs/libltr.a  libltr_la-cal.o libltr_la-list.o libltr_la-dyn_load.o libltr_la-math_utils.o libltr_la-pose.o libltr_la-pref.o libltr_la-pref_bison.o libltr_la-pref_flex.o libltr_la-pref_global.o libltr_la-utils.o libltr_la-image_process.o libltr_la-tracking.o libltr_la-ltlib_int.o libltr_la-spline.o libltr_la-axis.o libltr_la-wii_driver_prefs.o libltr_la-tir_driver_prefs.o libltr_la-wc_driver_prefs.o libltr_la-ipc_utils.o libltr_la-com_proc.o libltr_la-wii_com.o libltr_la-joy_driver_prefs.o libltr_la-ps3_prefs.o
libtool: link: ranlib .libs/libltr.a
libtool: link: ( cd ".libs" && rm -f "libltr.la" && ln -s "../libltr.la" "libltr.la" )
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./drivers.sym"    -o libmacwii.la -rpath /opt/linuxtrack/lib/linuxtrack libmacwii_la-macwii_driver.lo libmacwii_la-runloop.lo libmacwii_la-wii_driver_prefs.lo libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./drivers.sym"    -o libwii.la -rpath /opt/linuxtrack/lib/linuxtrack libwii_la-wiimote_driver.lo libwii_la-runloop.lo -lcwiid libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./webcam_driver.sym"    -o libwc.la -rpath /opt/linuxtrack/lib/linuxtrack libwc_la-webcam_driver.lo libwc_la-runloop.lo libltr.la -lv4l2 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./tir.sym"    -o libtir.la -rpath /opt/linuxtrack/lib/linuxtrack tir_hw.lo tir_img.lo tir_driver.lo runloop.lo -lz libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2     -o libltusb1.la -rpath /opt/linuxtrack/lib/linuxtrack libltusb1_la-libusb_ifc.lo -lusb-1.0 libltr.la 
/bin/bash ../libtool  --tag=CXX   --mode=link g++  -DOPENCV -I/usr/include/opencv4  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./webcam_driver.sym"   '-Wl,-rpath,/opt/linuxtrack/lib'  -o libft.la -rpath /opt/linuxtrack/lib/linuxtrack libft_la-webcam_driver.lo libft_la-runloop.lo libft_la-facetrack.lo libltr.la -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core  
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/opt/linuxtrack/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -pthread  -o ltr_recenter ltr_recenter-ltr_recenter.o ltr_recenter-ltr_srv_comm.o -ldl liblinuxtrack.la libltr.la 
/bin/bash ../libtool  --tag=CXX   --mode=link g++  -DOPENCV -I/usr/include/opencv4  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./ps3eye.sym"   '-Wl,-rpath,/opt/linuxtrack/lib'  -o libp3eft.la -rpath /opt/linuxtrack/lib/linuxtrack libp3eft_la-ps3eye_driver.lo libp3eft_la-runloop.lo libp3eft_la-facetrack.lo libltr.la -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core  
/bin/bash ../libtool  --tag=CXX   --mode=link g++  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2      -o ltr_server1 ltr_server1-ltr_srv_comm.o ltr_server1-ltr_srv_slave.o ltr_srv_master.o ltr_server1-ltr_server1.o ltr_server1-ipc_utils.o -lpthread libltr.la 
/bin/bash ../libtool  --tag=CXX   --mode=link g++  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2      -o libfakeusb.la -rpath /opt/linuxtrack/lib/linuxtrack libfakeusb_la-fakeusb.lo libfakeusb_la-tir_model.lo libfakeusb_la-tir4_model.lo libfakeusb_la-sn4_model.lo libfakeusb_la-sn3_model.lo libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./ps3eye.sym"    -o libp3e.la -rpath /opt/linuxtrack/lib/linuxtrack libp3e_la-ps3eye_driver.lo libp3e_la-runloop.lo libltr.la 
libtool: link: rm -fr  .libs/libp3e.a .libs/libp3e.la .libs/libp3e.lai .libs/libp3e.so .libs/libp3e.so.0 .libs/libp3e.so.0.0.0 .libs/libp3e.so.0.0.0T .libs/libp3e.ver
libtool: link: rm -fr  .libs/libmacwii.a .libs/libmacwii.la .libs/libmacwii.lai .libs/libmacwii.so .libs/libmacwii.so.0 .libs/libmacwii.so.0.0.0 .libs/libmacwii.so.0.0.0T .libs/libmacwii.ver
libtool: link: rm -fr  .libs/libltusb1.a .libs/libltusb1.la .libs/libltusb1.lai .libs/libltusb1.so .libs/libltusb1.so.0 .libs/libltusb1.so.0.0.0 .libs/libltusb1.so.0.0.0T
libtool: link: rm -fr  .libs/libtir.a .libs/libtir.la .libs/libtir.lai .libs/libtir.so .libs/libtir.so.0 .libs/libtir.so.0.0.0 .libs/libtir.so.0.0.0T .libs/libtir.ver
libtool: link: rm -fr  .libs/libwc.a .libs/libwc.la .libs/libwc.lai .libs/libwc.so .libs/libwc.so.0 .libs/libwc.so.0.0.0 .libs/libwc.so.0.0.0T .libs/libwc.ver
libtool: link: echo "{ global:" > .libs/libp3e.ver
libtool: link: echo "{ global:" > .libs/libmacwii.ver
libtool: link:  cat ./ps3eye.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libp3e.ver
libtool: link: echo "{ global:" > .libs/libwii.ver
libtool: link: gcc -shared  -fPIC -DPIC  .libs/libltusb1_la-libusb_ifc.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lusb-1.0 ./.libs/libltr.so -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libltusb1.so.0 -o .libs/libltusb1.so.0.0.0
libtool: link: echo "{ global:" > .libs/libwc.ver
libtool: link:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libmacwii.ver
libtool: link: echo "{ global:" > .libs/libtir.ver
libtool: link: gcc -DLIB_PATH=\"/opt/linuxtrack/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o .libs/ltr_recenter ltr_recenter-ltr_recenter.o ltr_recenter-ltr_srv_comm.o  ./.libs/liblinuxtrack.so ./.libs/libltr.so -lpthread -ldl -pthread -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack
libtool: link:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwii.ver
libtool: link:  cat ./webcam_driver.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwc.ver
libtool: link:  echo "local: *; };" >> .libs/libp3e.ver
libtool: link:  cat ./tir.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libtir.ver
libtool: link:  echo "local: *; };" >> .libs/libmacwii.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libp3e_la-ps3eye_driver.o .libs/libp3e_la-runloop.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack ./.libs/libltr.so -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libp3e.so.0 -Wl,-version-script -Wl,.libs/libp3e.ver -o .libs/libp3e.so.0.0.0
libtool: link:  echo "local: *; };" >> .libs/libwii.ver
libtool: link:  echo "local: *; };" >> .libs/libwc.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libmacwii_la-macwii_driver.o .libs/libmacwii_la-runloop.o .libs/libmacwii_la-wii_driver_prefs.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack ./.libs/libltr.so -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libmacwii.so.0 -Wl,-version-script -Wl,.libs/libmacwii.ver -o .libs/libmacwii.so.0.0.0
libtool: link:  echo "local: *; };" >> .libs/libtir.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libwii_la-wiimote_driver.o .libs/libwii_la-runloop.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lcwiid ./.libs/libltr.so -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwii.so.0 -Wl,-version-script -Wl,.libs/libwii.ver -o .libs/libwii.so.0.0.0
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libwc_la-webcam_driver.o .libs/libwc_la-runloop.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack ./.libs/libltr.so -lpthread -ldl -lv4l2  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwc.so.0 -Wl,-version-script -Wl,.libs/libwc.ver -o .libs/libwc.so.0.0.0
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/tir_hw.o .libs/tir_img.o .libs/tir_driver.o .libs/runloop.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lz ./.libs/libltr.so -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libtir.so.0 -Wl,-version-script -Wl,.libs/libtir.ver -o .libs/libtir.so.0.0.0
libtool: link: rm -fr  .libs/libfakeusb.a .libs/libfakeusb.la .libs/libfakeusb.lai .libs/libfakeusb.so .libs/libfakeusb.so.0 .libs/libfakeusb.so.0.0.0 .libs/libfakeusb.so.0.0.0T
libtool: link: g++ -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o .libs/ltr_server1 ltr_server1-ltr_srv_comm.o ltr_server1-ltr_srv_slave.o ltr_srv_master.o ltr_server1-ltr_server1.o ltr_server1-ipc_utils.o  ./.libs/libltr.so -lpthread -ldl -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack
libtool: link: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libfakeusb_la-fakeusb.o .libs/libfakeusb_la-tir_model.o .libs/libfakeusb_la-tir4_model.o .libs/libfakeusb_la-sn4_model.o .libs/libfakeusb_la-sn3_model.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack ./.libs/libltr.so -lpthread -ldl -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libfakeusb.so.0 -o .libs/libfakeusb.so.0.0.0
libtool: link: (cd ".libs" && rm -f "libltusb1.so.0" && ln -s "libltusb1.so.0.0.0" "libltusb1.so.0")
libtool: link: (cd ".libs" && rm -f "libltusb1.so" && ln -s "libltusb1.so.0.0.0" "libltusb1.so")
libtool: link: (cd ".libs" && rm -f "libp3e.so.0" && ln -s "libp3e.so.0.0.0" "libp3e.so.0")
libtool: link: (cd ".libs" && rm -f "libwc.so.0" && ln -s "libwc.so.0.0.0" "libwc.so.0")
libtool: link: (cd ".libs" && rm -f "libmacwii.so.0" && ln -s "libmacwii.so.0.0.0" "libmacwii.so.0")
libtool: link: (cd ".libs" && rm -f "libwii.so.0" && ln -s "libwii.so.0.0.0" "libwii.so.0")
libtool: link: (cd ".libs" && rm -f "libtir.so.0" && ln -s "libtir.so.0.0.0" "libtir.so.0")
libtool: link: (cd ".libs" && rm -f "libp3e.so" && ln -s "libp3e.so.0.0.0" "libp3e.so")
libtool: link: (cd ".libs" && rm -f "libwc.so" && ln -s "libwc.so.0.0.0" "libwc.so")
libtool: link: ar cr .libs/libltusb1.a  libltusb1_la-libusb_ifc.o
libtool: link: (cd ".libs" && rm -f "libmacwii.so" && ln -s "libmacwii.so.0.0.0" "libmacwii.so")
libtool: link: (cd ".libs" && rm -f "libwii.so" && ln -s "libwii.so.0.0.0" "libwii.so")
libtool: link: (cd ".libs" && rm -f "libtir.so" && ln -s "libtir.so.0.0.0" "libtir.so")
libtool: link: ranlib .libs/libltusb1.a
libtool: link: ar cr .libs/libp3e.a  libp3e_la-ps3eye_driver.o libp3e_la-runloop.o
libtool: link: ar cr .libs/libwc.a  libwc_la-webcam_driver.o libwc_la-runloop.o
libtool: link: ar cr .libs/libwii.a  libwii_la-wiimote_driver.o libwii_la-runloop.o
libtool: link: ar cr .libs/libmacwii.a  libmacwii_la-macwii_driver.o libmacwii_la-runloop.o libmacwii_la-wii_driver_prefs.o
libtool: link: ar cr .libs/libtir.a  tir_hw.o tir_img.o tir_driver.o runloop.o
libtool: link: ranlib .libs/libwc.a
libtool: link: ranlib .libs/libp3e.a
libtool: link: ranlib .libs/libwii.a
libtool: link: ranlib .libs/libmacwii.a
libtool: link: ( cd ".libs" && rm -f "libltusb1.la" && ln -s "../libltusb1.la" "libltusb1.la" )
libtool: link: ranlib .libs/libtir.a
libtool: link: ( cd ".libs" && rm -f "libp3e.la" && ln -s "../libp3e.la" "libp3e.la" )
libtool: link: ( cd ".libs" && rm -f "libwc.la" && ln -s "../libwc.la" "libwc.la" )
libtool: link: rm -fr  .libs/libp3eft.a .libs/libp3eft.la .libs/libp3eft.lai .libs/libp3eft.so .libs/libp3eft.so.0 .libs/libp3eft.so.0.0.0 .libs/libp3eft.so.0.0.0T
libtool: link: ( cd ".libs" && rm -f "libwii.la" && ln -s "../libwii.la" "libwii.la" )
libtool: link: rm -fr  .libs/libft.a .libs/libft.la .libs/libft.lai .libs/libft.so .libs/libft.so.0 .libs/libft.so.0.0.0 .libs/libft.so.0.0.0T
libtool: link: ( cd ".libs" && rm -f "libmacwii.la" && ln -s "../libmacwii.la" "libmacwii.la" )
libtool: link: ( cd ".libs" && rm -f "libtir.la" && ln -s "../libtir.la" "libtir.la" )
libtool: link: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libp3eft_la-ps3eye_driver.o .libs/libp3eft_la-runloop.o .libs/libp3eft_la-facetrack.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack ./.libs/libltr.so -ldl -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector -Wl,-rpath -Wl,/opt/linuxtrack/lib   -Wl,-soname -Wl,libp3eft.so.0 -Wl,-retain-symbols-file -Wl,./ps3eye.sym -o .libs/libp3eft.so.0.0.0
libtool: link: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libft_la-webcam_driver.o .libs/libft_la-runloop.o .libs/libft_la-facetrack.o   -Wl,-rpath -Wl,/tmp/linuxtrack-kubuntu-test/src/.libs -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack ./.libs/libltr.so -ldl -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector -Wl,-rpath -Wl,/opt/linuxtrack/lib   -Wl,-soname -Wl,libft.so.0 -Wl,-retain-symbols-file -Wl,./webcam_driver.sym -o .libs/libft.so.0.0.0
libtool: link: (cd ".libs" && rm -f "libfakeusb.so.0" && ln -s "libfakeusb.so.0.0.0" "libfakeusb.so.0")
libtool: link: (cd ".libs" && rm -f "libfakeusb.so" && ln -s "libfakeusb.so.0.0.0" "libfakeusb.so")
libtool: link: ar cr .libs/libfakeusb.a  libfakeusb_la-fakeusb.o libfakeusb_la-tir_model.o libfakeusb_la-tir4_model.o libfakeusb_la-sn4_model.o libfakeusb_la-sn3_model.o
libtool: link: ranlib .libs/libfakeusb.a
libtool: link: ( cd ".libs" && rm -f "libfakeusb.la" && ln -s "../libfakeusb.la" "libfakeusb.la" )
libtool: link: (cd ".libs" && rm -f "libft.so.0" && ln -s "libft.so.0.0.0" "libft.so.0")
libtool: link: (cd ".libs" && rm -f "libft.so" && ln -s "libft.so.0.0.0" "libft.so")
libtool: link: ar cr .libs/libft.a  libft_la-webcam_driver.o libft_la-runloop.o libft_la-facetrack.o
libtool: link: (cd ".libs" && rm -f "libp3eft.so.0" && ln -s "libp3eft.so.0.0.0" "libp3eft.so.0")
libtool: link: ranlib .libs/libft.a
libtool: link: (cd ".libs" && rm -f "libp3eft.so" && ln -s "libp3eft.so.0.0.0" "libp3eft.so")
libtool: link: ar cr .libs/libp3eft.a  libp3eft_la-ps3eye_driver.o libp3eft_la-runloop.o libp3eft_la-facetrack.o
libtool: link: ( cd ".libs" && rm -f "libft.la" && ln -s "../libft.la" "libft.la" )
libtool: link: ranlib .libs/libp3eft.a
libtool: link: ( cd ".libs" && rm -f "libp3eft.la" && ln -s "../libp3eft.la" "libp3eft.la" )
make[3]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
Making all in src/mac
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/mac'
make[2]: Nothing to be done for 'all'.
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/mac'
Making all in doc
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/doc'
make[2]: Nothing to be done for 'all'.
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/doc'
Making all in src/osc_server_mac
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[2]: Nothing to be done for 'all'.
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test'

real    0m8.973s
user    0m34.424s
sys     0m9.470s
Build finished: Sun Jun 29 08:30:30 AM EDT 2025
=== BUILD CHECK ===
-rwxr-xr-x 1 mario mario 23709704 Jun 29 08:29 src/qt_gui/ltr_gui
✅ ltr_gui built
-rwxr-xr-x 1 mario mario 4992208 Jun 29 08:29 src/mickey/mickey
✅ mickey built
-rw-r--r-- 1 mario mario 2072576 Jun 29 08:29 src/qt_gui/help.qch
✅ help files built


### **Step 4: Test Installation (5 minutes)**
```bash
# Test install to temp directory
sudo rm -rf /tmp/kubuntu-install-test
make install DESTDIR=/tmp/kubuntu-install-test

echo "=== INSTALL CHECK ==="
ls -la /tmp/kubuntu-install-test/opt/linuxtrack/bin/ | grep -E "(ltr_gui|mickey)"
ls -la /tmp/kubuntu-install-test/usr/share/applications/*linux* 2>/dev/null
```
Results
Test install to temp directory
sudo rm -rf /tmp/kubuntu-install-test
make install DESTDIR=/tmp/kubuntu-install-test

echo "=== INSTALL CHECK ==="
ls -la /tmp/kubuntu-install-test/opt/linuxtrack/bin/ | grep -E "(ltr_gui|mickey)"
ls -la /tmp/kubuntu-install-test/usr/share/applications/*linux* 2>/dev/null
Making install in src
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
make  install-am
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
make[3]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/bin'
  /bin/bash ../libtool   --mode=install /usr/bin/install -c ltr_server1 osc_server ltr_recenter ltr_extractor ltr_pipe '/tmp/kubuntu-install-test/opt/linuxtrack/bin'
libtool: warning: 'libltr.la' has not been installed in '/opt/linuxtrack/lib/linuxtrack'
libtool: install: /usr/bin/install -c .libs/ltr_server1 /tmp/kubuntu-install-test/opt/linuxtrack/bin/ltr_server1
libtool: install: /usr/bin/install -c osc_server /tmp/kubuntu-install-test/opt/linuxtrack/bin/osc_server
libtool: warning: 'liblinuxtrack.la' has not been installed in '/opt/linuxtrack/lib/linuxtrack'
libtool: warning: 'libltr.la' has not been installed in '/opt/linuxtrack/lib/linuxtrack'
libtool: install: /usr/bin/install -c .libs/ltr_recenter /tmp/kubuntu-install-test/opt/linuxtrack/bin/ltr_recenter
libtool: install: /usr/bin/install -c ltr_extractor /tmp/kubuntu-install-test/opt/linuxtrack/bin/ltr_extractor
libtool: install: /usr/bin/install -c ltr_pipe /tmp/kubuntu-install-test/opt/linuxtrack/bin/ltr_pipe
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/bin'
 /usr/bin/install -c ../verify_installation.sh '/tmp/kubuntu-install-test/opt/linuxtrack/bin'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack'
 /bin/bash ../libtool   --mode=install /usr/bin/install -c   libltr.la libmacwii.la liblinuxtrack.la libjoy.la libwii.la libwc.la libtir.la libltusb1.la libft.la libp3eft.la libfakeusb.la libp3e.la '/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack'
libtool: install: /usr/bin/install -c .libs/libltr.so.0.0.0 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltr.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libltr.so.0.0.0 libltr.so.0 || { rm -f libltr.so.0 && ln -s libltr.so.0.0.0 libltr.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libltr.so.0.0.0 libltr.so || { rm -f libltr.so && ln -s libltr.so.0.0.0 libltr.so; }; })
libtool: install: /usr/bin/install -c .libs/libltr.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltr.la
libtool: warning: relinking 'libmacwii.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./drivers.sym -o libmacwii.la -rpath /opt/linuxtrack/lib/linuxtrack libmacwii_la-macwii_driver.lo libmacwii_la-runloop.lo libmacwii_la-wii_driver_prefs.lo libltr.la -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: echo "{ global:" > .libs/libmacwii.ver
libtool: relink:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libmacwii.ver
libtool: relink:  echo "local: *; };" >> .libs/libmacwii.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libmacwii_la-macwii_driver.o .libs/libmacwii_la-runloop.o .libs/libmacwii_la-wii_driver_prefs.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libmacwii.so.0 -Wl,-version-script -Wl,.libs/libmacwii.ver -o .libs/libmacwii.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libmacwii.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libmacwii.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libmacwii.so.0.0.0 libmacwii.so.0 || { rm -f libmacwii.so.0 && ln -s libmacwii.so.0.0.0 libmacwii.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libmacwii.so.0.0.0 libmacwii.so || { rm -f libmacwii.so && ln -s libmacwii.so.0.0.0 libmacwii.so; }; })
libtool: install: /usr/bin/install -c .libs/libmacwii.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libmacwii.la
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.so.0.0.0 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/liblinuxtrack.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f liblinuxtrack.so.0.0.0 liblinuxtrack.so.0 || { rm -f liblinuxtrack.so.0 && ln -s liblinuxtrack.so.0.0.0 liblinuxtrack.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f liblinuxtrack.so.0.0.0 liblinuxtrack.so || { rm -f liblinuxtrack.so && ln -s liblinuxtrack.so.0.0.0 liblinuxtrack.so; }; })
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/liblinuxtrack.la
libtool: install: /usr/bin/install -c .libs/libjoy.so.0.0.0 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libjoy.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libjoy.so.0.0.0 libjoy.so.0 || { rm -f libjoy.so.0 && ln -s libjoy.so.0.0.0 libjoy.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libjoy.so.0.0.0 libjoy.so || { rm -f libjoy.so && ln -s libjoy.so.0.0.0 libjoy.so; }; })
libtool: install: /usr/bin/install -c .libs/libjoy.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libjoy.la
libtool: warning: relinking 'libwii.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./drivers.sym -o libwii.la -rpath /opt/linuxtrack/lib/linuxtrack libwii_la-wiimote_driver.lo libwii_la-runloop.lo -lcwiid libltr.la -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: echo "{ global:" > .libs/libwii.ver
libtool: relink:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwii.ver
libtool: relink:  echo "local: *; };" >> .libs/libwii.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libwii_la-wiimote_driver.o .libs/libwii_la-runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lcwiid -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwii.so.0 -Wl,-version-script -Wl,.libs/libwii.ver -o .libs/libwii.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libwii.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwii.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libwii.so.0.0.0 libwii.so.0 || { rm -f libwii.so.0 && ln -s libwii.so.0.0.0 libwii.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libwii.so.0.0.0 libwii.so || { rm -f libwii.so && ln -s libwii.so.0.0.0 libwii.so; }; })
libtool: install: /usr/bin/install -c .libs/libwii.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwii.la
libtool: warning: relinking 'libwc.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./webcam_driver.sym -o libwc.la -rpath /opt/linuxtrack/lib/linuxtrack libwc_la-webcam_driver.lo libwc_la-runloop.lo libltr.la -lv4l2 -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: echo "{ global:" > .libs/libwc.ver
libtool: relink:  cat ./webcam_driver.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwc.ver
libtool: relink:  echo "local: *; };" >> .libs/libwc.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libwc_la-webcam_driver.o .libs/libwc_la-runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl -lv4l2  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwc.so.0 -Wl,-version-script -Wl,.libs/libwc.ver -o .libs/libwc.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libwc.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwc.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libwc.so.0.0.0 libwc.so.0 || { rm -f libwc.so.0 && ln -s libwc.so.0.0.0 libwc.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libwc.so.0.0.0 libwc.so || { rm -f libwc.so && ln -s libwc.so.0.0.0 libwc.so; }; })
libtool: install: /usr/bin/install -c .libs/libwc.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwc.la
libtool: warning: relinking 'libtir.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./tir.sym -o libtir.la -rpath /opt/linuxtrack/lib/linuxtrack tir_hw.lo tir_img.lo tir_driver.lo runloop.lo -lz libltr.la -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: echo "{ global:" > .libs/libtir.ver
libtool: relink:  cat ./tir.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libtir.ver
libtool: relink:  echo "local: *; };" >> .libs/libtir.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/tir_hw.o .libs/tir_img.o .libs/tir_driver.o .libs/runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lz -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libtir.so.0 -Wl,-version-script -Wl,.libs/libtir.ver -o .libs/libtir.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libtir.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libtir.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libtir.so.0.0.0 libtir.so.0 || { rm -f libtir.so.0 && ln -s libtir.so.0.0.0 libtir.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libtir.so.0.0.0 libtir.so || { rm -f libtir.so && ln -s libtir.so.0.0.0 libtir.so; }; })
libtool: install: /usr/bin/install -c .libs/libtir.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libtir.la
libtool: warning: relinking 'libltusb1.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o libltusb1.la -rpath /opt/linuxtrack/lib/linuxtrack libltusb1_la-libusb_ifc.lo -lusb-1.0 libltr.la -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: gcc -shared  -fPIC -DPIC  .libs/libltusb1_la-libusb_ifc.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lusb-1.0 -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libltusb1.so.0 -o .libs/libltusb1.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libltusb1.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltusb1.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libltusb1.so.0.0.0 libltusb1.so.0 || { rm -f libltusb1.so.0 && ln -s libltusb1.so.0.0.0 libltusb1.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libltusb1.so.0.0.0 libltusb1.so || { rm -f libltusb1.so && ln -s libltusb1.so.0.0.0 libltusb1.so; }; })
libtool: install: /usr/bin/install -c .libs/libltusb1.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltusb1.la
libtool: warning: relinking 'libft.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CXX --mode=relink g++ -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./webcam_driver.sym -Wl,-rpath,/opt/linuxtrack/lib -o libft.la -rpath /opt/linuxtrack/lib/linuxtrack libft_la-webcam_driver.lo libft_la-runloop.lo libft_la-facetrack.lo libltr.la -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libft_la-webcam_driver.o .libs/libft_la-runloop.o .libs/libft_la-facetrack.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -ldl -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector -Wl,-rpath -Wl,/opt/linuxtrack/lib   -Wl,-soname -Wl,libft.so.0 -Wl,-retain-symbols-file -Wl,./webcam_driver.sym -o .libs/libft.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libft.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libft.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libft.so.0.0.0 libft.so.0 || { rm -f libft.so.0 && ln -s libft.so.0.0.0 libft.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libft.so.0.0.0 libft.so || { rm -f libft.so && ln -s libft.so.0.0.0 libft.so; }; })
libtool: install: /usr/bin/install -c .libs/libft.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libft.la
libtool: warning: relinking 'libp3eft.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CXX --mode=relink g++ -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./ps3eye.sym -Wl,-rpath,/opt/linuxtrack/lib -o libp3eft.la -rpath /opt/linuxtrack/lib/linuxtrack libp3eft_la-ps3eye_driver.lo libp3eft_la-runloop.lo libp3eft_la-facetrack.lo libltr.la -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libp3eft_la-ps3eye_driver.o .libs/libp3eft_la-runloop.o .libs/libp3eft_la-facetrack.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -ldl -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector -Wl,-rpath -Wl,/opt/linuxtrack/lib   -Wl,-soname -Wl,libp3eft.so.0 -Wl,-retain-symbols-file -Wl,./ps3eye.sym -o .libs/libp3eft.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libp3eft.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3eft.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3eft.so.0.0.0 libp3eft.so.0 || { rm -f libp3eft.so.0 && ln -s libp3eft.so.0.0.0 libp3eft.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3eft.so.0.0.0 libp3eft.so || { rm -f libp3eft.so && ln -s libp3eft.so.0.0.0 libp3eft.so; }; })
libtool: install: /usr/bin/install -c .libs/libp3eft.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3eft.la
libtool: warning: relinking 'libfakeusb.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CXX --mode=relink g++ -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o libfakeusb.la -rpath /opt/linuxtrack/lib/linuxtrack libfakeusb_la-fakeusb.lo libfakeusb_la-tir_model.lo libfakeusb_la-tir4_model.lo libfakeusb_la-sn4_model.lo libfakeusb_la-sn3_model.lo libltr.la -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libfakeusb_la-fakeusb.o .libs/libfakeusb_la-tir_model.o .libs/libfakeusb_la-tir4_model.o .libs/libfakeusb_la-sn4_model.o .libs/libfakeusb_la-sn3_model.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libfakeusb.so.0 -o .libs/libfakeusb.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libfakeusb.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libfakeusb.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libfakeusb.so.0.0.0 libfakeusb.so.0 || { rm -f libfakeusb.so.0 && ln -s libfakeusb.so.0.0.0 libfakeusb.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libfakeusb.so.0.0.0 libfakeusb.so || { rm -f libfakeusb.so && ln -s libfakeusb.so.0.0.0 libfakeusb.so; }; })
libtool: install: /usr/bin/install -c .libs/libfakeusb.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libfakeusb.la
libtool: warning: relinking 'libp3e.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./ps3eye.sym -o libp3e.la -rpath /opt/linuxtrack/lib/linuxtrack libp3e_la-ps3eye_driver.lo libp3e_la-runloop.lo libltr.la -inst-prefix-dir /tmp/kubuntu-install-test)
libtool: relink: echo "{ global:" > .libs/libp3e.ver
libtool: relink:  cat ./ps3eye.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libp3e.ver
libtool: relink:  echo "local: *; };" >> .libs/libp3e.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libp3e_la-ps3eye_driver.o .libs/libp3e_la-runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libp3e.so.0 -Wl,-version-script -Wl,.libs/libp3e.ver -o .libs/libp3e.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libp3e.so.0.0.0T /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3e.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3e.so.0.0.0 libp3e.so.0 || { rm -f libp3e.so.0 && ln -s libp3e.so.0.0.0 libp3e.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3e.so.0.0.0 libp3e.so || { rm -f libp3e.so && ln -s libp3e.so.0.0.0 libp3e.so; }; })
libtool: install: /usr/bin/install -c .libs/libp3e.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3e.la
libtool: install: /usr/bin/install -c .libs/libltr.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltr.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltr.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltr.a
libtool: install: /usr/bin/install -c .libs/libmacwii.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libmacwii.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libmacwii.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libmacwii.a
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/liblinuxtrack.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/liblinuxtrack.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/liblinuxtrack.a
libtool: install: /usr/bin/install -c .libs/libjoy.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libjoy.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libjoy.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libjoy.a
libtool: install: /usr/bin/install -c .libs/libwii.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwii.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwii.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwii.a
libtool: install: /usr/bin/install -c .libs/libwc.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwc.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwc.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libwc.a
libtool: install: /usr/bin/install -c .libs/libtir.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libtir.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libtir.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libtir.a
libtool: install: /usr/bin/install -c .libs/libltusb1.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltusb1.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltusb1.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libltusb1.a
libtool: install: /usr/bin/install -c .libs/libft.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libft.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libft.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libft.a
libtool: install: /usr/bin/install -c .libs/libp3eft.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3eft.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3eft.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3eft.a
libtool: install: /usr/bin/install -c .libs/libfakeusb.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libfakeusb.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libfakeusb.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libfakeusb.a
libtool: install: /usr/bin/install -c .libs/libp3e.a /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3e.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3e.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib/linuxtrack/libp3e.a
libtool: warning: remember to run 'libtool --finish /opt/linuxtrack/lib/linuxtrack'
make  install-exec-hook
make[4]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd qt_gui;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/opt/linuxtrack/lib/linuxtrack"; make INSTALL_ROOT=/tmp/kubuntu-install-test install; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
make[5]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/qt_gui'
/usr/lib/qt5/bin/uic ltr_gui.ui -o ui_ltr_gui.h
/usr/lib/qt5/bin/uic dev_help.ui -o ui_dev_help.h
/usr/lib/qt5/bin/uic ltr.ui -o ui_ltr.h
/usr/lib/qt5/bin/uic model_creation.ui -o ui_model_creation.h
/usr/lib/qt5/bin/uic scurve.ui -o ui_scurve.h
/usr/lib/qt5/bin/uic scp_form.ui -o ui_scp_form.h
/usr/lib/qt5/bin/uic logview.ui -o ui_logview.h
/usr/lib/qt5/bin/uic wii_setup.ui -o ui_wii_setup.h
/usr/lib/qt5/bin/uic tir_setup.ui -o ui_tir_setup.h
/usr/lib/qt5/bin/uic cap_edit.ui -o ui_cap_edit.h
/usr/lib/qt5/bin/uic clip_edit.ui -o ui_clip_edit.h
/usr/lib/qt5/bin/uic single_edit.ui -o ui_single_edit.h
/usr/lib/qt5/bin/uic model_edit.ui -o ui_model_edit.h
/usr/lib/qt5/bin/uic device_setup.ui -o ui_device_setup.h
/usr/lib/qt5/bin/uic profile_setup.ui -o ui_profile_setup.h
/usr/lib/qt5/bin/uic clip_tweaking.ui -o ui_clip_tweaking.h
/usr/lib/qt5/bin/uic m_ps3eye_setup.ui -o ui_m_ps3eye_setup.h
/usr/lib/qt5/bin/uic m_ps3eye_ft_setup.ui -o ui_m_ps3eye_ft_setup.h
/usr/lib/qt5/bin/uic l_wc_setup.ui -o ui_l_wc_setup.h
g++ -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -x c++-header -c precomp_headers.h -o ltr_gui.gch/c++
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o main.o main.cpp
g++ -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -dM -E -o moc_predefs.h /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/data/dummy.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_gui.h -o moc_ltr_gui.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_gui.o ltr_gui.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_show.h -o moc_ltr_show.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_show.o ltr_show.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_dev_help.h -o moc_ltr_dev_help.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_dev_help.o ltr_dev_help.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_gui_prefs.o ltr_gui_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include tir_prefs.h -o moc_tir_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o tir_prefs.o tir_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o help_view.o help_view.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o buffering.o buffering.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_model.h -o moc_ltr_model.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_model.o ltr_model.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include window.h -o moc_window.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o window.o window.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include glwidget.h -o moc_glwidget.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o glwidget.o glwidget.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o objreader.o objreader.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_profiles.h -o moc_ltr_profiles.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_profiles.o ltr_profiles.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include log_view.h -o moc_log_view.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o log_view.o log_view.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_state.h -o moc_ltr_state.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_state.o ltr_state.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include scp_form.h -o moc_scp_form.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o scp_form.o scp_form.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include guardian.h -o moc_guardian.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o guardian.o guardian.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include scurve.h -o moc_scurve.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o scurve.o scurve.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include scview.h -o moc_scview.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o scview.o scview.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include wiimote_prefs.h -o moc_wiimote_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wiimote_prefs.o wiimote_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_srv_master.o ../ltr_srv_master.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include device_setup.h -o moc_device_setup.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o device_setup.o device_setup.cpp
gcc -pipe -Wall -Wextra -DLTR_GUI -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -x c-header -c precomp_headers.h -o ltr_gui.gch/c
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_srv_slave.o ../ltr_srv_slave.c
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_srv_comm.o ../ltr_srv_comm.c
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include plugin_install.h -o moc_plugin_install.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o plugin_install.o plugin_install.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include tracker.h -o moc_tracker.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include profile_setup.h -o moc_profile_setup.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o profile_setup.o profile_setup.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include profile_selector.h -o moc_profile_selector.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o profile_selector.o profile_selector.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include xplugin.h -o moc_xplugin.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o xplugin.o xplugin.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include wine_warn.h -o moc_wine_warn.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wine_warn.o wine_warn.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include progress.h -o moc_progress.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o progress.o progress.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include extractor.h -o moc_extractor.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o extractor.o extractor.cpp
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o game_data.o ../game_data.c
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o hashing.o hashing.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include downloading.h -o moc_downloading.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o downloading.o downloading.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include wine_launcher.h -o moc_wine_launcher.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wine_launcher.o wine_launcher.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include macps3eye_prefs.h -o moc_macps3eye_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o macps3eye_prefs.o macps3eye_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o macwebcam_info.o macwebcam_info.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include macps3eyeft_prefs.h -o moc_macps3eyeft_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o macps3eyeft_prefs.o macps3eyeft_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o help_viewer.o help_viewer.cpp
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o extract.o ../extract.c
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o digest.o ../digest.c
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o webcam_info.o webcam_info.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include webcam_prefs.h -o moc_webcam_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o webcam_prefs.o webcam_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include webcam_ft_prefs.h -o moc_webcam_ft_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o webcam_ft_prefs.o webcam_ft_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include joy_prefs.h -o moc_joy_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o joy_prefs.o joy_prefs.cpp
/usr/lib/qt5/bin/rcc -name ltr_rc ltr_rc.qrc -o qrc_ltr_rc.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o qrc_ltr_rc.o qrc_ltr_rc.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include help_view.h -o moc_help_view.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o moc_help_view.o moc_help_view.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include /tmp/linuxtrack-kubuntu-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/tmp/linuxtrack-kubuntu-test/src/qt_gui -I/tmp/linuxtrack-kubuntu-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include help_viewer.h -o moc_help_viewer.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /opt/linuxtrack/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o moc_help_viewer.o moc_help_viewer.cpp
g++  -o ltr_gui main.o ltr_gui.o ltr_show.o ltr_dev_help.o ltr_gui_prefs.o tir_prefs.o help_view.o buffering.o ltr_model.o window.o glwidget.o objreader.o ltr_profiles.o log_view.o ltr_state.o scp_form.o guardian.o scurve.o scview.o wiimote_prefs.o ltr_srv_master.o device_setup.o ltr_srv_slave.o ltr_srv_comm.o plugin_install.o profile_setup.o profile_selector.o xplugin.o wine_warn.o progress.o extractor.o game_data.o hashing.o downloading.o wine_launcher.o macps3eye_prefs.o macwebcam_info.o macps3eyeft_prefs.o help_viewer.o extract.o digest.o webcam_info.o webcam_prefs.o webcam_ft_prefs.o joy_prefs.o qrc_ltr_rc.o moc_help_view.o moc_help_viewer.o   -L../.libs -L/opt/linuxtrack/lib -L/opt/linuxtrack/lib/linuxtrack -lm -lltr -lGLU -lmxml -Wl,-rpath,/opt/linuxtrack/lib/linuxtrack -Wl,-rpath,/opt/linuxtrack/lib /usr/lib/x86_64-linux-gnu/libQt5OpenGL.so /usr/lib/x86_64-linux-gnu/libQt5Help.so /usr/lib/x86_64-linux-gnu/libQt5Widgets.so /usr/lib/x86_64-linux-gnu/libQt5Gui.so /usr/lib/x86_64-linux-gnu/libQt5Network.so /usr/lib/x86_64-linux-gnu/libQt5Sql.so /usr/lib/x86_64-linux-gnu/libQt5Core.so -lGL -lpthread   
/usr/lib/qt5/bin/qmake -install qinstall -exe ltr_gui /tmp/kubuntu-install-test/opt/linuxtrack/bin/ltr_gui
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sparow_opaq.obj /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/sparow_opaq.obj
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sparow_glass.obj /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/sparow_glass.obj
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/xm8_detail.png /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/xm8_detail.png
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sources.txt /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/sources.txt
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/spec.txt /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/spec.txt
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sphere.obj /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/sphere.obj
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sphere.png /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/sphere.png
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sources_mfc.txt /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/sources_mfc.txt
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/win7.reg /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/win7.reg
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/help.qhc /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/help/ltr_gui/help.qhc
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/help.qch /tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack/help/ltr_gui/help.qch
make[5]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/qt_gui'
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/opt/linuxtrack/lib/linuxtrack"; make INSTALL_ROOT=/tmp/kubuntu-install-test install; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
Project ERROR: Unknown module(s) in QT: x11extras
make[5]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/mickey'
/usr/bin/qmake-qt5 -o Makefile mickey.pro -spec linux-g++ LIBDIR=/opt/linuxtrack/lib/linuxtrack
make[5]: /usr/bin/qmake-qt5: No such file or directory
make[5]: *** [Makefile:319: Makefile] Error 127
make[5]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/mickey'
/tmp/linuxtrack-kubuntu-test/src
pushd wii_server;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/opt/linuxtrack/lib/linuxtrack"; make INSTALL_ROOT=/tmp/kubuntu-install-test install; popd
/tmp/linuxtrack-kubuntu-test/src/wii_server /tmp/linuxtrack-kubuntu-test/src
make[5]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/wii_server'
/usr/lib/qt5/bin/qmake -install qinstall -exe wii_server /tmp/kubuntu-install-test/opt/linuxtrack/bin/wii_server
make[5]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/wii_server'
/tmp/linuxtrack-kubuntu-test/src
Installing udev rules for TrackIR devices...
/usr/bin/mkdir -p /tmp/kubuntu-install-test/lib/udev/rules.d/
/usr/bin/install -c -m 644 ./99-TIR.rules /tmp/kubuntu-install-test/lib/udev/rules.d/
/usr/bin/install -c -m 644 ./99-Mickey.rules /tmp/kubuntu-install-test/lib/udev/rules.d/
Reloading udev rules...
udevadm control --reload-rules 2>/dev/null || true
udev rules installation complete.
make[4]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/applications'
 /usr/bin/install -c -m 644 linuxtrack.desktop linuxtrack-wii.desktop mickey.desktop '/tmp/kubuntu-install-test/opt/linuxtrack/share/applications'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack'
 /usr/bin/install -c -m 644 linuxtrack1.conf linuxtrack.c linuxtrack_hello_world.c linuxtrack_hello_world_adv.c 99-TIR.rules 99-Mickey.rules haarcascade_frontalface_alt2.xml '/tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/icons/hicolor/48x48/apps  '
 /usr/bin/install -c -m 644 linuxtrack.png linuxtrack-wii.png '/tmp/kubuntu-install-test/opt/linuxtrack/share/icons/hicolor/48x48/apps  '
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/icons/hicolor/scalable/apps'
 /usr/bin/install -c -m 644 linuxtrack.svg linuxtrack-wii.svg '/tmp/kubuntu-install-test/opt/linuxtrack/share/icons/hicolor/scalable/apps'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/pixmaps'
 /usr/bin/install -c -m 644 linuxtrack.svg linuxtrack.png linuxtrack.xpm linuxtrack-wii.svg linuxtrack-wii.png linuxtrack-wii.xpm '/tmp/kubuntu-install-test/opt/linuxtrack/share/pixmaps'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/include'
 /usr/bin/install -c -m 644 linuxtrack.h '/tmp/kubuntu-install-test/opt/linuxtrack/include'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack'
 /bin/bash ../libtool   --mode=install /usr/bin/install -c   liblinuxtrack32.la '/tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack'
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.so.0.0.0 /tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.so.0.0.0
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack && { ln -s -f liblinuxtrack32.so.0.0.0 liblinuxtrack32.so.0 || { rm -f liblinuxtrack32.so.0 && ln -s liblinuxtrack32.so.0.0.0 liblinuxtrack32.so.0; }; })
libtool: install: (cd /tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack && { ln -s -f liblinuxtrack32.so.0.0.0 liblinuxtrack32.so || { rm -f liblinuxtrack32.so && ln -s liblinuxtrack32.so.0.0.0 liblinuxtrack32.so; }; })
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.lai /tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.la
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.a /tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.a
libtool: install: chmod 644 /tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.a
libtool: install: ranlib /tmp/kubuntu-install-test/opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.a
libtool: warning: remember to run 'libtool --finish /opt/linuxtrack/lib32/linuxtrack'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/man/man1'
 /usr/bin/install -c -m 644 ltr_gui.1 ltr_server1.1 mickey.1 wii_server.1 ltr_extractor.1 ltr_pipe.1 '/tmp/kubuntu-install-test/opt/linuxtrack/share/man/man1'
make  install-data-hook
make[4]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
Updating desktop database...
update-desktop-database /tmp/kubuntu-install-test/opt/linuxtrack/share/applications 2>/dev/null || true
Updating icon cache...
gtk-update-icon-cache -f -t /tmp/kubuntu-install-test/opt/linuxtrack/share/icons/hicolor 2>/dev/null || true
Desktop integration update complete.
make[4]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
make[3]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
Making install in src/mac
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/mac'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/mac'
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/mac'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/mac'
Making install in doc
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/doc'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/doc'
make[2]: Nothing to be done for 'install-exec-am'.
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack'
 /usr/bin/install -c -m 644 README.ltr_pipe debug_flags.txt '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs'
 /usr/bin/install -c -m 644  fgfs/README '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs/Nasal/LinuxTrack'
 /usr/bin/install -c -m 644  fgfs/Nasal/LinuxTrack/linuxtrack.nas '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs/Nasal/LinuxTrack'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs/Input/Joysticks/LinuxTrack'
 /usr/bin/install -c -m 644  fgfs/Input/Joysticks/LinuxTrack/uinput-abs.xml '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs/Input/Joysticks/LinuxTrack'
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs/Protocol'
 /usr/bin/install -c -m 644  fgfs/Protocol/linuxtrack.nas.xml fgfs/Protocol/linuxtrack.xml '/tmp/kubuntu-install-test/opt/linuxtrack/share/doc/linuxtrack/fgfs/Protocol'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/doc'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/doc'
Making install in src/osc_server_mac
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test'
make[2]: Nothing to be done for 'install-exec-am'.
 /usr/bin/mkdir -p '/tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack'
 /usr/bin/install -c -m 644 README README.devel README.xplane '/tmp/kubuntu-install-test/opt/linuxtrack/share/linuxtrack'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test'
=== INSTALL CHECK ===
-rwxr-xr-x 1 mario mario 24097448 Jun 29 08:32 ltr_gui



### **Step 5: System Installation (5 minutes)**
```bash
# Install to system (CAREFUL - modifies your system!)
echo "Installing to system in 5 seconds... Press Ctrl+C to cancel"
sleep 5
sudo make install

# Refresh desktop
sudo update-desktop-database
sudo gtk-update-icon-cache -f -t /usr/share/icons/hicolor

echo "✅ Installation complete!"
```
RESUlTS
Install to system (CAREFUL - modifies your system!)
echo "Installing to system in 5 seconds... Press Ctrl+C to cancel"
sleep 5
sudo make install

# Refresh desktop
sudo update-desktop-database
sudo gtk-update-icon-cache -f -t /usr/share/icons/hicolor
Installing to system in 5 seconds... Press Ctrl+C to cancel
Making install in src
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
make  install-am
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
make[3]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
 /usr/bin/mkdir -p '/opt/linuxtrack/bin'
  /bin/bash ../libtool   --mode=install /usr/bin/install -c ltr_server1 osc_server ltr_recenter ltr_extractor ltr_pipe '/opt/linuxtrack/bin'
libtool: warning: 'libltr.la' has not been installed in '/opt/linuxtrack/lib/linuxtrack'
libtool: install: /usr/bin/install -c .libs/ltr_server1 /opt/linuxtrack/bin/ltr_server1
libtool: install: /usr/bin/install -c osc_server /opt/linuxtrack/bin/osc_server
libtool: warning: 'liblinuxtrack.la' has not been installed in '/opt/linuxtrack/lib/linuxtrack'
libtool: warning: 'libltr.la' has not been installed in '/opt/linuxtrack/lib/linuxtrack'
libtool: install: /usr/bin/install -c .libs/ltr_recenter /opt/linuxtrack/bin/ltr_recenter
libtool: install: /usr/bin/install -c ltr_extractor /opt/linuxtrack/bin/ltr_extractor
libtool: install: /usr/bin/install -c ltr_pipe /opt/linuxtrack/bin/ltr_pipe
 /usr/bin/mkdir -p '/opt/linuxtrack/bin'
 /usr/bin/install -c ../verify_installation.sh '/opt/linuxtrack/bin'
 /usr/bin/mkdir -p '/opt/linuxtrack/lib/linuxtrack'
 /bin/bash ../libtool   --mode=install /usr/bin/install -c   libltr.la libmacwii.la liblinuxtrack.la libjoy.la libwii.la libwc.la libtir.la libltusb1.la libft.la libp3eft.la libfakeusb.la libp3e.la '/opt/linuxtrack/lib/linuxtrack'
libtool: install: /usr/bin/install -c .libs/libltr.so.0.0.0 /opt/linuxtrack/lib/linuxtrack/libltr.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libltr.so.0.0.0 libltr.so.0 || { rm -f libltr.so.0 && ln -s libltr.so.0.0.0 libltr.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libltr.so.0.0.0 libltr.so || { rm -f libltr.so && ln -s libltr.so.0.0.0 libltr.so; }; })
libtool: install: /usr/bin/install -c .libs/libltr.lai /opt/linuxtrack/lib/linuxtrack/libltr.la
libtool: warning: relinking 'libmacwii.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./drivers.sym -o libmacwii.la -rpath /opt/linuxtrack/lib/linuxtrack libmacwii_la-macwii_driver.lo libmacwii_la-runloop.lo libmacwii_la-wii_driver_prefs.lo libltr.la )
libtool: relink: echo "{ global:" > .libs/libmacwii.ver
libtool: relink:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libmacwii.ver
libtool: relink:  echo "local: *; };" >> .libs/libmacwii.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libmacwii_la-macwii_driver.o .libs/libmacwii_la-runloop.o .libs/libmacwii_la-wii_driver_prefs.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libmacwii.so.0 -Wl,-version-script -Wl,.libs/libmacwii.ver -o .libs/libmacwii.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libmacwii.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libmacwii.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libmacwii.so.0.0.0 libmacwii.so.0 || { rm -f libmacwii.so.0 && ln -s libmacwii.so.0.0.0 libmacwii.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libmacwii.so.0.0.0 libmacwii.so || { rm -f libmacwii.so && ln -s libmacwii.so.0.0.0 libmacwii.so; }; })
libtool: install: /usr/bin/install -c .libs/libmacwii.lai /opt/linuxtrack/lib/linuxtrack/libmacwii.la
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.so.0.0.0 /opt/linuxtrack/lib/linuxtrack/liblinuxtrack.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f liblinuxtrack.so.0.0.0 liblinuxtrack.so.0 || { rm -f liblinuxtrack.so.0 && ln -s liblinuxtrack.so.0.0.0 liblinuxtrack.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f liblinuxtrack.so.0.0.0 liblinuxtrack.so || { rm -f liblinuxtrack.so && ln -s liblinuxtrack.so.0.0.0 liblinuxtrack.so; }; })
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.lai /opt/linuxtrack/lib/linuxtrack/liblinuxtrack.la
libtool: install: /usr/bin/install -c .libs/libjoy.so.0.0.0 /opt/linuxtrack/lib/linuxtrack/libjoy.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libjoy.so.0.0.0 libjoy.so.0 || { rm -f libjoy.so.0 && ln -s libjoy.so.0.0.0 libjoy.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libjoy.so.0.0.0 libjoy.so || { rm -f libjoy.so && ln -s libjoy.so.0.0.0 libjoy.so; }; })
libtool: install: /usr/bin/install -c .libs/libjoy.lai /opt/linuxtrack/lib/linuxtrack/libjoy.la
libtool: warning: relinking 'libwii.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./drivers.sym -o libwii.la -rpath /opt/linuxtrack/lib/linuxtrack libwii_la-wiimote_driver.lo libwii_la-runloop.lo -lcwiid libltr.la )
libtool: relink: echo "{ global:" > .libs/libwii.ver
libtool: relink:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwii.ver
libtool: relink:  echo "local: *; };" >> .libs/libwii.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libwii_la-wiimote_driver.o .libs/libwii_la-runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lcwiid -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwii.so.0 -Wl,-version-script -Wl,.libs/libwii.ver -o .libs/libwii.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libwii.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libwii.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libwii.so.0.0.0 libwii.so.0 || { rm -f libwii.so.0 && ln -s libwii.so.0.0.0 libwii.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libwii.so.0.0.0 libwii.so || { rm -f libwii.so && ln -s libwii.so.0.0.0 libwii.so; }; })
libtool: install: /usr/bin/install -c .libs/libwii.lai /opt/linuxtrack/lib/linuxtrack/libwii.la
libtool: warning: relinking 'libwc.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./webcam_driver.sym -o libwc.la -rpath /opt/linuxtrack/lib/linuxtrack libwc_la-webcam_driver.lo libwc_la-runloop.lo libltr.la -lv4l2 )
libtool: relink: echo "{ global:" > .libs/libwc.ver
libtool: relink:  cat ./webcam_driver.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwc.ver
libtool: relink:  echo "local: *; };" >> .libs/libwc.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libwc_la-webcam_driver.o .libs/libwc_la-runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl -lv4l2  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwc.so.0 -Wl,-version-script -Wl,.libs/libwc.ver -o .libs/libwc.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libwc.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libwc.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libwc.so.0.0.0 libwc.so.0 || { rm -f libwc.so.0 && ln -s libwc.so.0.0.0 libwc.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libwc.so.0.0.0 libwc.so || { rm -f libwc.so && ln -s libwc.so.0.0.0 libwc.so; }; })
libtool: install: /usr/bin/install -c .libs/libwc.lai /opt/linuxtrack/lib/linuxtrack/libwc.la
libtool: warning: relinking 'libtir.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./tir.sym -o libtir.la -rpath /opt/linuxtrack/lib/linuxtrack tir_hw.lo tir_img.lo tir_driver.lo runloop.lo -lz libltr.la )
libtool: relink: echo "{ global:" > .libs/libtir.ver
libtool: relink:  cat ./tir.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libtir.ver
libtool: relink:  echo "local: *; };" >> .libs/libtir.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/tir_hw.o .libs/tir_img.o .libs/tir_driver.o .libs/runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lz -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libtir.so.0 -Wl,-version-script -Wl,.libs/libtir.ver -o .libs/libtir.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libtir.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libtir.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libtir.so.0.0.0 libtir.so.0 || { rm -f libtir.so.0 && ln -s libtir.so.0.0.0 libtir.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libtir.so.0.0.0 libtir.so || { rm -f libtir.so && ln -s libtir.so.0.0.0 libtir.so; }; })
libtool: install: /usr/bin/install -c .libs/libtir.lai /opt/linuxtrack/lib/linuxtrack/libtir.la
libtool: warning: relinking 'libltusb1.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o libltusb1.la -rpath /opt/linuxtrack/lib/linuxtrack libltusb1_la-libusb_ifc.lo -lusb-1.0 libltr.la )
libtool: relink: gcc -shared  -fPIC -DPIC  .libs/libltusb1_la-libusb_ifc.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -lusb-1.0 -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libltusb1.so.0 -o .libs/libltusb1.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libltusb1.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libltusb1.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libltusb1.so.0.0.0 libltusb1.so.0 || { rm -f libltusb1.so.0 && ln -s libltusb1.so.0.0.0 libltusb1.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libltusb1.so.0.0.0 libltusb1.so || { rm -f libltusb1.so && ln -s libltusb1.so.0.0.0 libltusb1.so; }; })
libtool: install: /usr/bin/install -c .libs/libltusb1.lai /opt/linuxtrack/lib/linuxtrack/libltusb1.la
libtool: warning: relinking 'libft.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CXX --mode=relink g++ -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./webcam_driver.sym -Wl,-rpath,/opt/linuxtrack/lib -o libft.la -rpath /opt/linuxtrack/lib/linuxtrack libft_la-webcam_driver.lo libft_la-runloop.lo libft_la-facetrack.lo libltr.la -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core )
libtool: relink: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libft_la-webcam_driver.o .libs/libft_la-runloop.o .libs/libft_la-facetrack.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -ldl -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector -Wl,-rpath -Wl,/opt/linuxtrack/lib   -Wl,-soname -Wl,libft.so.0 -Wl,-retain-symbols-file -Wl,./webcam_driver.sym -o .libs/libft.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libft.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libft.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libft.so.0.0.0 libft.so.0 || { rm -f libft.so.0 && ln -s libft.so.0.0.0 libft.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libft.so.0.0.0 libft.so || { rm -f libft.so && ln -s libft.so.0.0.0 libft.so; }; })
libtool: install: /usr/bin/install -c .libs/libft.lai /opt/linuxtrack/lib/linuxtrack/libft.la
libtool: warning: relinking 'libp3eft.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CXX --mode=relink g++ -DOPENCV -I/usr/include/opencv4 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./ps3eye.sym -Wl,-rpath,/opt/linuxtrack/lib -o libp3eft.la -rpath /opt/linuxtrack/lib/linuxtrack libp3eft_la-ps3eye_driver.lo libp3eft_la-runloop.lo libp3eft_la-facetrack.lo libltr.la -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core )
libtool: relink: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libp3eft_la-ps3eye_driver.o .libs/libp3eft_la-runloop.o .libs/libp3eft_la-facetrack.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -ldl -lv4l2 -lpthread -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_signal -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector -Wl,-rpath -Wl,/opt/linuxtrack/lib   -Wl,-soname -Wl,libp3eft.so.0 -Wl,-retain-symbols-file -Wl,./ps3eye.sym -o .libs/libp3eft.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libp3eft.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libp3eft.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3eft.so.0.0.0 libp3eft.so.0 || { rm -f libp3eft.so.0 && ln -s libp3eft.so.0.0.0 libp3eft.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3eft.so.0.0.0 libp3eft.so || { rm -f libp3eft.so && ln -s libp3eft.so.0.0.0 libp3eft.so; }; })
libtool: install: /usr/bin/install -c .libs/libp3eft.lai /opt/linuxtrack/lib/linuxtrack/libp3eft.la
libtool: warning: relinking 'libfakeusb.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CXX --mode=relink g++ -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o libfakeusb.la -rpath /opt/linuxtrack/lib/linuxtrack libfakeusb_la-fakeusb.lo libfakeusb_la-tir_model.lo libfakeusb_la-tir4_model.lo libfakeusb_la-sn4_model.lo libfakeusb_la-sn3_model.lo libltr.la )
libtool: relink: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libfakeusb_la-fakeusb.o .libs/libfakeusb_la-tir_model.o .libs/libfakeusb_la-tir4_model.o .libs/libfakeusb_la-sn4_model.o .libs/libfakeusb_la-sn3_model.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lgcc_s -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libfakeusb.so.0 -o .libs/libfakeusb.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libfakeusb.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libfakeusb.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libfakeusb.so.0.0.0 libfakeusb.so.0 || { rm -f libfakeusb.so.0 && ln -s libfakeusb.so.0.0.0 libfakeusb.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libfakeusb.so.0.0.0 libfakeusb.so || { rm -f libfakeusb.so && ln -s libfakeusb.so.0.0.0 libfakeusb.so; }; })
libtool: install: /usr/bin/install -c .libs/libfakeusb.lai /opt/linuxtrack/lib/linuxtrack/libfakeusb.la
libtool: warning: relinking 'libp3e.la'
libtool: install: (cd /tmp/linuxtrack-kubuntu-test/src; /bin/bash "/tmp/linuxtrack-kubuntu-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./ps3eye.sym -o libp3e.la -rpath /opt/linuxtrack/lib/linuxtrack libp3e_la-ps3eye_driver.lo libp3e_la-runloop.lo libltr.la )
libtool: relink: echo "{ global:" > .libs/libp3e.ver
libtool: relink:  cat ./ps3eye.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libp3e.ver
libtool: relink:  echo "local: *; };" >> .libs/libp3e.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libp3e_la-ps3eye_driver.o .libs/libp3e_la-runloop.o   -Wl,-rpath -Wl,/opt/linuxtrack/lib/linuxtrack -L/opt/linuxtrack/lib/linuxtrack -lltr -lpthread -ldl  -g -O2 -fstack-protector   -Wl,-soname -Wl,libp3e.so.0 -Wl,-version-script -Wl,.libs/libp3e.ver -o .libs/libp3e.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libp3e.so.0.0.0T /opt/linuxtrack/lib/linuxtrack/libp3e.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3e.so.0.0.0 libp3e.so.0 || { rm -f libp3e.so.0 && ln -s libp3e.so.0.0.0 libp3e.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib/linuxtrack && { ln -s -f libp3e.so.0.0.0 libp3e.so || { rm -f libp3e.so && ln -s libp3e.so.0.0.0 libp3e.so; }; })
libtool: install: /usr/bin/install -c .libs/libp3e.lai /opt/linuxtrack/lib/linuxtrack/libp3e.la
libtool: install: /usr/bin/install -c .libs/libltr.a /opt/linuxtrack/lib/linuxtrack/libltr.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libltr.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libltr.a
libtool: install: /usr/bin/install -c .libs/libmacwii.a /opt/linuxtrack/lib/linuxtrack/libmacwii.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libmacwii.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libmacwii.a
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.a /opt/linuxtrack/lib/linuxtrack/liblinuxtrack.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/liblinuxtrack.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/liblinuxtrack.a
libtool: install: /usr/bin/install -c .libs/libjoy.a /opt/linuxtrack/lib/linuxtrack/libjoy.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libjoy.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libjoy.a
libtool: install: /usr/bin/install -c .libs/libwii.a /opt/linuxtrack/lib/linuxtrack/libwii.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libwii.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libwii.a
libtool: install: /usr/bin/install -c .libs/libwc.a /opt/linuxtrack/lib/linuxtrack/libwc.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libwc.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libwc.a
libtool: install: /usr/bin/install -c .libs/libtir.a /opt/linuxtrack/lib/linuxtrack/libtir.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libtir.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libtir.a
libtool: install: /usr/bin/install -c .libs/libltusb1.a /opt/linuxtrack/lib/linuxtrack/libltusb1.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libltusb1.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libltusb1.a
libtool: install: /usr/bin/install -c .libs/libft.a /opt/linuxtrack/lib/linuxtrack/libft.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libft.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libft.a
libtool: install: /usr/bin/install -c .libs/libp3eft.a /opt/linuxtrack/lib/linuxtrack/libp3eft.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libp3eft.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libp3eft.a
libtool: install: /usr/bin/install -c .libs/libfakeusb.a /opt/linuxtrack/lib/linuxtrack/libfakeusb.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libfakeusb.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libfakeusb.a
libtool: install: /usr/bin/install -c .libs/libp3e.a /opt/linuxtrack/lib/linuxtrack/libp3e.a
libtool: install: chmod 644 /opt/linuxtrack/lib/linuxtrack/libp3e.a
libtool: install: ranlib /opt/linuxtrack/lib/linuxtrack/libp3e.a
libtool: finish: PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/sbin" ldconfig -n /opt/linuxtrack/lib/linuxtrack
----------------------------------------------------------------------
Libraries have been installed in:
   /opt/linuxtrack/lib/linuxtrack

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
make  install-exec-hook
make[4]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd qt_gui; qhelpgenerator-qt5 ltr_gui.qhp -o help.qch && qhelpgenerator-qt5 ltr_gui.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd qt_gui;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/opt/linuxtrack/lib/linuxtrack"; make INSTALL_ROOT= install; popd
/tmp/linuxtrack-kubuntu-test/src/qt_gui /tmp/linuxtrack-kubuntu-test/src
make[5]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/qt_gui'
/usr/lib/qt5/bin/qmake -install qinstall -exe ltr_gui /opt/linuxtrack/bin/ltr_gui
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sparow_opaq.obj /opt/linuxtrack/share/linuxtrack/sparow_opaq.obj
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sparow_glass.obj /opt/linuxtrack/share/linuxtrack/sparow_glass.obj
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/xm8_detail.png /opt/linuxtrack/share/linuxtrack/xm8_detail.png
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sources.txt /opt/linuxtrack/share/linuxtrack/sources.txt
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/spec.txt /opt/linuxtrack/share/linuxtrack/spec.txt
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sphere.obj /opt/linuxtrack/share/linuxtrack/sphere.obj
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sphere.png /opt/linuxtrack/share/linuxtrack/sphere.png
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/sources_mfc.txt /opt/linuxtrack/share/linuxtrack/sources_mfc.txt
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/win7.reg /opt/linuxtrack/share/linuxtrack/win7.reg
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/help.qhc /opt/linuxtrack/share/linuxtrack/help/ltr_gui/help.qhc
/usr/lib/qt5/bin/qmake -install qinstall /tmp/linuxtrack-kubuntu-test/src/qt_gui/help.qch /opt/linuxtrack/share/linuxtrack/help/ltr_gui/help.qch
make[5]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/qt_gui'
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey; qhelpgenerator-qt5 mickey.qhp -o help.qch && qhelpgenerator-qt5 mickey.qhcp -o help.qhc; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
/bin/bash: line 1: qhelpgenerator-qt5: command not found
/tmp/linuxtrack-kubuntu-test/src
pushd mickey;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/opt/linuxtrack/lib/linuxtrack"; make INSTALL_ROOT= install; popd
/tmp/linuxtrack-kubuntu-test/src/mickey /tmp/linuxtrack-kubuntu-test/src
Project ERROR: Unknown module(s) in QT: x11extras
make[5]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/mickey'
/usr/bin/qmake-qt5 -o Makefile mickey.pro -spec linux-g++ LIBDIR=/opt/linuxtrack/lib/linuxtrack
make[5]: /usr/bin/qmake-qt5: No such file or directory
make[5]: *** [Makefile:319: Makefile] Error 127
make[5]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/mickey'
/tmp/linuxtrack-kubuntu-test/src
pushd wii_server;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/opt/linuxtrack/lib/linuxtrack"; make INSTALL_ROOT= install; popd
/tmp/linuxtrack-kubuntu-test/src/wii_server /tmp/linuxtrack-kubuntu-test/src
make[5]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/wii_server'
/usr/lib/qt5/bin/qmake -install qinstall -exe wii_server /opt/linuxtrack/bin/wii_server
make[5]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/wii_server'
/tmp/linuxtrack-kubuntu-test/src
Installing udev rules for TrackIR devices...
/usr/bin/mkdir -p /lib/udev/rules.d/
/usr/bin/install -c -m 644 ./99-TIR.rules /lib/udev/rules.d/
/usr/bin/install -c -m 644 ./99-Mickey.rules /lib/udev/rules.d/
Reloading udev rules...
udevadm control --reload-rules 2>/dev/null || true
udev rules installation complete.
make[4]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/applications'
 /usr/bin/install -c -m 644 linuxtrack.desktop linuxtrack-wii.desktop mickey.desktop '/opt/linuxtrack/share/applications'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/linuxtrack'
 /usr/bin/install -c -m 644 linuxtrack1.conf linuxtrack.c linuxtrack_hello_world.c linuxtrack_hello_world_adv.c 99-TIR.rules 99-Mickey.rules haarcascade_frontalface_alt2.xml '/opt/linuxtrack/share/linuxtrack'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/icons/hicolor/48x48/apps  '
 /usr/bin/install -c -m 644 linuxtrack.png linuxtrack-wii.png '/opt/linuxtrack/share/icons/hicolor/48x48/apps  '
 /usr/bin/mkdir -p '/opt/linuxtrack/share/icons/hicolor/scalable/apps'
 /usr/bin/install -c -m 644 linuxtrack.svg linuxtrack-wii.svg '/opt/linuxtrack/share/icons/hicolor/scalable/apps'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/pixmaps'
 /usr/bin/install -c -m 644 linuxtrack.svg linuxtrack.png linuxtrack.xpm linuxtrack-wii.svg linuxtrack-wii.png linuxtrack-wii.xpm '/opt/linuxtrack/share/pixmaps'
 /usr/bin/mkdir -p '/opt/linuxtrack/include'
 /usr/bin/install -c -m 644 linuxtrack.h '/opt/linuxtrack/include'
 /usr/bin/mkdir -p '/opt/linuxtrack/lib32/linuxtrack'
 /bin/bash ../libtool   --mode=install /usr/bin/install -c   liblinuxtrack32.la '/opt/linuxtrack/lib32/linuxtrack'
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.so.0.0.0 /opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.so.0.0.0
libtool: install: (cd /opt/linuxtrack/lib32/linuxtrack && { ln -s -f liblinuxtrack32.so.0.0.0 liblinuxtrack32.so.0 || { rm -f liblinuxtrack32.so.0 && ln -s liblinuxtrack32.so.0.0.0 liblinuxtrack32.so.0; }; })
libtool: install: (cd /opt/linuxtrack/lib32/linuxtrack && { ln -s -f liblinuxtrack32.so.0.0.0 liblinuxtrack32.so || { rm -f liblinuxtrack32.so && ln -s liblinuxtrack32.so.0.0.0 liblinuxtrack32.so; }; })
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.lai /opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.la
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.a /opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.a
libtool: install: chmod 644 /opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.a
libtool: install: ranlib /opt/linuxtrack/lib32/linuxtrack/liblinuxtrack32.a
libtool: finish: PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/sbin" ldconfig -n /opt/linuxtrack/lib32/linuxtrack
----------------------------------------------------------------------
Libraries have been installed in:
   /opt/linuxtrack/lib32/linuxtrack

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
 /usr/bin/mkdir -p '/opt/linuxtrack/share/man/man1'
 /usr/bin/install -c -m 644 ltr_gui.1 ltr_server1.1 mickey.1 wii_server.1 ltr_extractor.1 ltr_pipe.1 '/opt/linuxtrack/share/man/man1'
make  install-data-hook
make[4]: Entering directory '/tmp/linuxtrack-kubuntu-test/src'
Updating desktop database...
update-desktop-database /opt/linuxtrack/share/applications 2>/dev/null || true
Updating icon cache...
gtk-update-icon-cache -f -t /opt/linuxtrack/share/icons/hicolor 2>/dev/null || true
Desktop integration update complete.
make[4]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
make[3]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src'
Making install in src/mac
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/mac'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/mac'
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/mac'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/mac'
Making install in doc
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/doc'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/doc'
make[2]: Nothing to be done for 'install-exec-am'.
 /usr/bin/mkdir -p '/opt/linuxtrack/share/doc/linuxtrack'
 /usr/bin/install -c -m 644 README.ltr_pipe debug_flags.txt '/opt/linuxtrack/share/doc/linuxtrack'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/doc/linuxtrack'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/doc/linuxtrack/fgfs'
 /usr/bin/install -c -m 644  fgfs/README '/opt/linuxtrack/share/doc/linuxtrack/fgfs'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/doc/linuxtrack/fgfs/Nasal/LinuxTrack'
 /usr/bin/install -c -m 644  fgfs/Nasal/LinuxTrack/linuxtrack.nas '/opt/linuxtrack/share/doc/linuxtrack/fgfs/Nasal/LinuxTrack'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/doc/linuxtrack/fgfs/Input/Joysticks/LinuxTrack'
 /usr/bin/install -c -m 644  fgfs/Input/Joysticks/LinuxTrack/uinput-abs.xml '/opt/linuxtrack/share/doc/linuxtrack/fgfs/Input/Joysticks/LinuxTrack'
 /usr/bin/mkdir -p '/opt/linuxtrack/share/doc/linuxtrack/fgfs/Protocol'
 /usr/bin/install -c -m 644  fgfs/Protocol/linuxtrack.nas.xml fgfs/Protocol/linuxtrack.xml '/opt/linuxtrack/share/doc/linuxtrack/fgfs/Protocol'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/doc'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/doc'
Making install in src/osc_server_mac
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test/src/osc_server_mac'
make[1]: Entering directory '/tmp/linuxtrack-kubuntu-test'
make[2]: Entering directory '/tmp/linuxtrack-kubuntu-test'
make[2]: Nothing to be done for 'install-exec-am'.
 /usr/bin/mkdir -p '/opt/linuxtrack/share/linuxtrack'
 /usr/bin/install -c -m 644 README README.devel README.xplane '/opt/linuxtrack/share/linuxtrack'
make[2]: Leaving directory '/tmp/linuxtrack-kubuntu-test'
make[1]: Leaving directory '/tmp/linuxtrack-kubuntu-test'
gtk-update-icon-cache: Cache file created successfully.


### **Step 6: Functionality Test (10 minutes)**

#### **Test 1: Desktop Integration**
```bash
echo "=== DESKTOP INTEGRATION TEST ==="
echo "1. Open K-Menu (Application Launcher)"
echo "2. Search for 'LinuxTrack'"
echo "3. You should see:"
echo "   - LinuxTrack (main app)"
echo "   - LinuxTrack Wii Server"
echo ""
echo "Result: [PASS/FAIL] - Applications visible in menu: PASS"
```
Results PASS
But the icon did not find the application so you could not launch it from here
#### **Test 2: Application Launch**
```bash
echo "=== APPLICATION LAUNCH TEST ==="

# Test ltr_gui
echo "Testing ltr_gui launch..."
timeout 10s /opt/linuxtrack/bin/ltr_gui --version
if [ $? -eq 0 ]; then
    echo "✅ ltr_gui version check passed"
else
    echo "❌ ltr_gui version check failed"
fi

# Test mickey  
echo "Testing mickey launch..."
timeout 10s /opt/linuxtrack/bin/mickey --version 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✅ mickey version check passed"
else
    echo "❌ mickey version check failed or no --version option"
fi

echo ""
echo "Manual Test Required:"
echo "1. Launch from menu: LinuxTrack"
echo "2. Application should start without errors"
echo "3. Press F1 to test help system"
echo "4. Help window should open with Qt5 help content"
echo ""
echo "Result: [PASS/FAIL] - GUI applications work: ____"
```
RESULTS
ltr_gui launches
F1 did not launch the help menu but the button on each TAB did bring up the relevant HELP section. There is a little formatting issues with large spaces between text and images but not major
When selecting the Trackir out of the list the console stated Found device 8 Tyep 8
When selecting the Model Name from the Model Setup tab I could select the correct trackir Models and the console stated
qt.qpa.wayland: Wayland does not support QWindow::requestActivate()
Which we know that the system will not dispaly realtime tracking correctly under Wayland at this time and we have to use X11 for it to work properly
In the tracking window the Camera View does show the blob tracking when paused...as per Wayland issue stated above.
3D View is blank

Testing mickey launch...
❌ mickey version check failed or no --version option

ltr_gui I re ran the app using this command XDG_SESSION_TYPE=x11 QT_QPA_PLATFORM=xcb /opt/linuxtrack/bin/ltr_gui --version
The realtime tracking and display in the Tracking Window Camera View tab worked very well
The 3D View did show up and you could see the 3d view is working but the camera orientation in the view appears to be flipped where forward is down, possibly as it was hard to tell

UPDATE, when I ran it the second time with this command the 3D View worked PERFECT!

Logfile for ltr_gui
Linuxtrack version 0.99.20
[Sun 2025-06-29 08:37:55 EDT] Starting ltr_gui
[Sun 2025-06-29 08:37:56 EDT] Preferences read OK!
[Sun 2025-06-29 08:37:56 EDT] Dumping prefs:
[Sun 2025-06-29 08:37:56 EDT] [Global]
Model = NP TrackClip
Prefix = "/opt/linuxtrack/bin"


[Wiimote]
Capture-device = Wiimote
Capture-device-id = Wiimote
Running-indication = 0100
Paused-indication = 0010


[TrackIR]
Capture-device = Tir
Capture-device-id = Tir
Threshold = 140
Min-blob = 4
Max-blob = 2500
Status-led-brightness = 0
Ir-led-brightness = 7
Status-signals = On


[NP TrackClip]
Model-type = Cap
Cap-X = 67
Cap-Y = 54
Cap-Z = 96
Head-Y = 160
Head-Z = 50
Active = no


[NP TrackClip Pro]
Model-type = Clip
Clip-Y1 = 40
Clip-Y2 = 110
Clip-Z1 = 30
Clip-Z2 = 50
Head-X = -100
Head-Y = -100
Head-Z = 50
Active = yes


[Face]
Model-type = Face


[Single Point Reflective]
Model-type = SinglePoint
Active = no


[Single LED]
Model-type = SinglePoint
Active = yes


[Default]
Title = Default
Pitch-enabled = Yes
Pitch-deadzone = 0.0
Pitch-left-curvature = 0.5
Pitch-right-curvature = 0.5
Pitch-sensitivity = 5.000000
Pitch-left-limit = 80.000000
Pitch-right-limit = 80.000000
Pitch-filter = 0.2
Pitch-inverted = No
Yaw-enabled = Yes
Yaw-deadzone = 0.0
Yaw-left-curvature = 0.5
Yaw-right-curvature = 0.5
Yaw-sensitivity = 5.000000
Yaw-left-limit = 130.000000
Yaw-right-limit = 130.000000
Yaw-filter = 0.2
Yaw-inverted = No
Roll-enabled = Yes
Roll-deadzone = 0.0
Roll-left-curvature = 0.5
Roll-right-curvature = 0.5
Roll-sensitivity = 1.500000
Roll-left-limit = 45.000000
Roll-right-limit = 45.000000
Roll-filter = 0.2
Roll-inverted = No
Xtranslation-enabled = Yes
Xtranslation-deadzone = 0.0
Xtranslation-left-curvature = 0.5
Xtranslation-right-curvature = 0.5
Xtranslation-sensitivity = 5.000000
Xtranslation-left-limit = 300.000000
Xtranslation-right-limit = 300.000000
Xtranslation-filter = 0.2
Xtranslation-inverted = No
Ytranslation-enabled = Yes
Ytranslation-deadzone = 0.0
Ytranslation-left-curvature = 0.5
Ytranslation-right-curvature = 0.5
Ytranslation-sensitivity = 5.000000
Ytranslation-left-limit = 300.000000
Ytranslation-right-limit = 300.000000
Ytranslation-filter = 0.2
Ytranslation-inverted = No
Ztranslation-enabled = Yes
Ztranslation-deadzone = 0.0
Ztranslation-left-curvature = 0.5
Ztranslation-right-curvature = 0.5
Ztranslation-sensitivity = 2.000000
Ztranslation-left-limit = 300.000000
Ztranslation-right-limit = 1.000000
Ztranslation-filter = 0.5
Ztranslation-inverted = No



[Sun 2025-06-29 08:37:56 EDT] ================================================
[Sun 2025-06-29 08:37:56 EDT] Opening logfile viewer.
[Sun 2025-06-29 08:37:56 EDT] Going to create lock '/home/mario/.config/linuxtrack/ltr_server.lock' => 20!
[Sun 2025-06-29 08:37:56 EDT] Lock 20 success!
[Sun 2025-06-29 08:37:56 EDT] Passing the lock to protect fifo (pid 25173)!
[Sun 2025-06-29 08:37:56 EDT] Initializing axes for profile 'Default'!
[Sun 2025-06-29 08:37:56 EDT] Closing axes!
[Sun 2025-06-29 08:37:56 EDT] Initializing axes for profile 'Default'!
[Sun 2025-06-29 08:37:56 EDT] Closing axes!
[Sun 2025-06-29 08:37:56 EDT] Initializing axes for profile 'Default'!
[Sun 2025-06-29 08:37:56 EDT] Closing axes!
[Sun 2025-06-29 08:37:56 EDT] Initializing axes for profile 'Default'!
[Sun 2025-06-29 08:37:56 EDT]     WINEARCH='win32'
[Sun 2025-06-29 08:37:56 EDT] Launching wine command: 'wine --version'
[Sun 2025-06-29 08:37:56 EDT] Initializing libusb.
[Sun 2025-06-29 08:37:56 EDT] Libusb initialized successfuly.
[Sun 2025-06-29 08:37:56 EDT] Libusb debug level set.
[Sun 2025-06-29 08:37:56 EDT] Requesting device list.
[Sun 2025-06-29 08:37:56 EDT] Device list received (10 devices).
[Sun 2025-06-29 08:37:56 EDT] Device is a TrackIR (131D:0159).
[Sun 2025-06-29 08:37:56 EDT] Opening handle to the device found.
[Sun 2025-06-29 08:37:56 EDT] Handle opened successfully.
[Sun 2025-06-29 08:37:56 EDT] Freeing device list.
[Sun 2025-06-29 08:37:56 EDT] Device list freed.
[Sun 2025-06-29 08:37:56 EDT] Closing TrackIR.
[Sun 2025-06-29 08:37:56 EDT] Closing TrackIR handle.
[Sun 2025-06-29 08:37:56 EDT] Exiting libusb.
[Sun 2025-06-29 08:37:56 EDT] Libusb exited.
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied
[Sun 2025-06-29 08:37:56 EDT] open: Permission denied

[Sun 2025-06-29 08:44:36 EDT] Lock 20 success!
[Sun 2025-06-29 08:44:36 EDT] Socket created!
[Sun 2025-06-29 08:44:36 EDT] Other master gave up, gui master taking over!
[Sun 2025-06-29 08:44:36 EDT] Starting as master!
[Sun 2025-06-29 08:44:36 EDT] Setting up Cap
[Sun 2025-06-29 08:44:36 EDT] Initializing model!
[Sun 2025-06-29 08:44:36 EDT] Tracking initialized!
[Sun 2025-06-29 08:44:36 EDT] Adding fd 39
[Sun 2025-06-29 08:44:36 EDT] Device Type: Track IR
[Sun 2025-06-29 08:44:36 EDT] Changing state to INITIALIZING!
[Sun 2025-06-29 08:44:36 EDT] Loading library 'libtir'
[Sun 2025-06-29 08:44:36 EDT] Running!
[Sun 2025-06-29 08:44:36 EDT] Changing state to INITIALIZING!
[Sun 2025-06-29 08:44:36 EDT] Initializing the tracker.
[Sun 2025-06-29 08:44:36 EDT] Lib loaded, prefs read...
[Sun 2025-06-29 08:44:36 EDT] Initializing libusb.
[Sun 2025-06-29 08:44:36 EDT] Libusb initialized successfuly.
[Sun 2025-06-29 08:44:36 EDT] Libusb debug level set.
[Sun 2025-06-29 08:44:36 EDT] Requesting device list.
[Sun 2025-06-29 08:44:36 EDT] Device list received (10 devices).
[Sun 2025-06-29 08:44:36 EDT] Device is a TrackIR (131D:0159).
[Sun 2025-06-29 08:44:36 EDT] Opening handle to the device found.
[Sun 2025-06-29 08:44:36 EDT] Handle opened successfully.
[Sun 2025-06-29 08:44:36 EDT] Freeing device list.
[Sun 2025-06-29 08:44:36 EDT] Device list freed.
[Sun 2025-06-29 08:44:36 EDT] Requesting TrackIR configuration.
[Sun 2025-06-29 08:44:36 EDT] TrackIR configuration received.
[Sun 2025-06-29 08:44:36 EDT] TrackIR already in requested configuration.
[Sun 2025-06-29 08:44:36 EDT] Trying to claim TrackIR interface.
[Sun 2025-06-29 08:44:36 EDT] TrackIR interface claimed.
[Sun 2025-06-29 08:44:36 EDT] Requesting TrackIR configuration.
[Sun 2025-06-29 08:44:36 EDT] TrackIR configuration received.
[Sun 2025-06-29 08:44:36 EDT] Device configuration is OK.
[Sun 2025-06-29 08:44:36 EDT] Device 8.
[Sun 2025-06-29 08:44:36 EDT] Initializing TrackIR 5 revision 3.
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x1A00
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: A5 67 C6 69 73 41 BF 4A EC 29 CD BA AB F2 FB E3 46 63 C2 54 F8 1B E8 E7
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x1A00
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: 26 63 33 0F 89 9A 66 32 0D B7 31 58 A3 5A 25 5D 05 26 58 E9 5E D4 AB B2
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: B9 54 11 0E 82 74 41 21 3D DC 87 70 E9 3E A1 41 E1 F4 67 3E 01 7E 97 EA

[Sun 2025-06-29 08:44:36 EDT] Requesting status.
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: FB 96 8F 38 5D AA AC B0 3B FB 32 AF 3C 54 EC 18 DB 37 02 1A FE 43 FB FA
[Sun 2025-06-29 08:44:36 EDT] Status packet: 11 20 01 00 01 B7 4E
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x1A01
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: 94 D9 E2 05 3C 7C 94 75 88 FE 61 89 F9 5C BB A8 99 23 95 B1 EB F1 B3 05
[Sun 2025-06-29 08:44:36 EDT] Requesting status.
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: 86 A1 3E E5 CB 6B CB D0 48 47 64 BD 1F 23 1E A8 1C 26 64 C5 14 73 5A C5
[Sun 2025-06-29 08:44:36 EDT] Status packet: 12 20 01 00 01 B1 E7
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x1A02
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: B1 33 70 16 E5 11 9E 09 DC AA D4 AC F2 1B 10 AF 3B 3A CD E3 50 48 47 15
[Sun 2025-06-29 08:44:36 EDT] Requesting status.
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:44:36 EDT] *tir5v3* Packet: 25 11 BA 6B 3D F5 0B E1 1A 1C 7F 23 F8 29 F8 A4 1B AD B5 CA 4E E8 98 32

[Sun 2025-06-29 08:44:37 EDT] Status packet: 0F 20 01 00 01 43 E1
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x1A03
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 49 35 34 BC 2E 0E 77 FA CB 6C 05 AC 86 21 2B AA 1A C3 A2 BE 70 B5 73 3B
[Sun 2025-06-29 08:44:37 EDT] Requesting status.
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 58 94 B7 AD E3 70 A4 9E 4F 32 15 49 FD 82 4E A9 08 FD D4 B2 8A 29 54 48
[Sun 2025-06-29 08:44:37 EDT] Status packet: 11 20 01 01 01 4F 33
[Sun 2025-06-29 08:44:37 EDT] Flushing packets...
[Sun 2025-06-29 08:44:37 EDT] Data receive request timed out!
[Sun 2025-06-29 08:44:37 EDT] Sending get_conf request.
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x17
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 78 0E 18 A8 44 AC 5B F3 8E 4C D7 2D 9B 09 42 E5 06 4E 33 AF CD A3 84 7F

[Sun 2025-06-29 08:44:37 EDT] Requesting data...
[Sun 2025-06-29 08:44:37 EDT] Requesting data...
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x1905012C
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 4F DC 72 47 DF 32 1C EC 4A C4 05 FE 2B 23 85 6C FB 6D 07 04 F4 EC 0B B9
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x23350280
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 3A CB 3A 07 F0 EC D9 67 33 5C EC 77 A3 E3 14 D3 D9 FC F7 5E A0 F2 10 A8
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x23350107
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 00 09 BA B4 BD 44 78 FA 7F 5F 4B 23 D0 1A DA 69 6A BF 4C 7E 51 25 B3 48
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 67 9C FF 31 99 90 32 57 44 EE 9B BC E2 3D E9 CF 08 CF E9 E2 5E 53 60 AA
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x233B8F78
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 47 8D FA 56 D9 35 E8 D4 66 82 05 5E 22 A8 87 75 65 74 5A 8A 3F 62 80 29

[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x233B8E00
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 34 AD 8D 4E 56 59 D3 51 AD AC 86 95 80 82 BE 8C 85 86 8C 0C 66 F1 7C C0
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 2B FC E0 64 DB 61 0B 63 16 F6 26 83 B4 69 2F 3A FF F5 27 16 93 AC 07 1F
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 17 3C 29 8F EF 4F 89 D4 B6 63 96 64 E8 E4 24 83 67 3C ED 96 12 EC 45 39
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 70 0A F8 9F 76 09 D1 A5 96 90 68 AE 95 AA 82 CA 6C 77 AE 90 CD 16 68 BA
[Sun 2025-06-29 08:44:37 EDT] Going to start camera.
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: EA FA B0 AA CB 99 B2 C2 37 2A 03 89 6F 61 C9 C3 80 5A 6E 03 28 DA 4C D7

[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x233502E0
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 4A DA D7 99 4D 79 8B 00 22 56 9A D4 A1 EC A5 E4 D9 99 45 A3 91 C6 01 FF
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x23350115
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 58 1D 05 41 2F EE 15 02 87 61 7C 13 70 47 D8 FC 72 5F CD 71 65 A6 3E AB
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: FF 43 CE 3A 74 67 CB 9B EA 7E 64 FF 81 EB 61 FD FE 69 9B 67 BF 0D E9 8C
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x233B8F5E
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: E2 BD FD 7E 8C 6A C7 5B A4 3C 02 F4 B2 C2 D7 5F EC 5D 01 4D F0 00 10 8B
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Intent: 0x233B8E01
[Sun 2025-06-29 08:44:37 EDT] *tir5v3* Packet: 57 50 5F 17 9E 80 A0 AA 0A 61 03 D1 BC A7 0D BE 9B 83 AB 0E D5 98 01 D6

[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: D0 FE 7D 3E C4 16 8E 21 2E 2D AF 02 C6 96 8F 68 8A 68 70 97 DE 0C 56 89
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: 88 1B 05 05 0C D8 FD 8B 16 C2 A1 34 D7 53 D2 92 D2 26 4B 35 61 D5 55 D1
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: 16 CA B8 F7 ED DE 13 EF E5 20 53 85 1C DD A4 4D 81 25 1C 53 1A EE EB 66
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: 70 71 1E AA AC B2 0E EE F3 58 46 06 47 2B 26 0E 0D 55 EB B2 1F 6C 3A 3B

[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x1A04
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: C8 A3 4A 8E F8 F6 C7 16 9E 73 11 08 DB 04 60 22 0A CE 4D 31 B5 5B 03 A0
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: 94 5D CD 99 87 78 56 D5 70 4C 9C 86 0D E2 52 F2 EB 5A 53 0D A7 FA 5A D8
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: 3E 58 C2 FD 5D 09 5A 2A 32 9E 41 FB B7 13 47 54 9A 4D 63 32 23 4E CE 76
[Sun 2025-06-29 08:44:38 EDT] Setting up Cap
[Sun 2025-06-29 08:44:38 EDT] Changing state to RUNNING!
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:38 EDT] *tir5v3* Packet: E6 BE 4D 23 6A 28 71 2E 25 CF 37 80 F9 1F D2 A8 D7 15 B0 1E 6D 4A 4F D1
[Sun 2025-06-29 08:44:38 EDT] Bad packet size! -167772159 x 336
[Sun 2025-06-29 08:44:38 EDT] ERROR!!! ('68 01')
[Sun 2025-06-29 08:44:38 EDT] ERROR!!! ('68 02')
[Sun 2025-06-29 08:44:38 EDT] ERROR!!! ('68 03')
[Sun 2025-06-29 08:44:38 EDT] Bad packet header!

[Sun 2025-06-29 08:44:42 EDT] Suspending!
[Sun 2025-06-29 08:44:42 EDT] Changing state to PAUSED!
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Intent: 0x1A05
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Packet: C5 42 ED 79 80 1A 31 0D 7B 9C 36 ED CA 5B BC 02 DB BD DE 3D 52 B6 57 02
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Packet: 6D 95 CC 97 B4 CA C1 FF D2 DB 61 E0 56 FD 16 43 C8 BD FF CA 4D B5 A8 8A

[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Packet: 73 09 33 A6 54 57 3B 1D EE F0 8F AE 20 02 49 81 E2 88 7F F8 E3 47 69 E3
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Packet: CE 41 9F 18 22 A8 4B C8 FD A2 04 1A 90 F4 49 FE 15 E8 48 96 2D E8 15 25
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Intent: 0x19040033
[Sun 2025-06-29 08:44:42 EDT] *tir5v3* Packet: C0 8F AE 6D 44 46 27 86 E5 3F A9 6A 71 8F 8A 2C 75 91 BC 6A EE BA 7F 39

[Sun 2025-06-29 08:44:44 EDT] Waking!
[Sun 2025-06-29 08:44:44 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:44 EDT] *tir5v3* Packet: 3A EA 2B 8E B7 87 1B 64 F5 61 A8 17 03 90 5B 90 1E 92 02 A8 11 77 4D CD
[Sun 2025-06-29 08:44:44 EDT] *tir5v3* Intent: 0x233502E0
[Sun 2025-06-29 08:44:44 EDT] *tir5v3* Packet: C3 68 74 8A 76 DB 74 A1 68 2A 28 83 8F 96 0C 2A 39 A7 CA 94 5C E8 79 5E

[Sun 2025-06-29 08:44:44 EDT] *tir5v3* Intent: 0x23350115
[Sun 2025-06-29 08:44:44 EDT] *tir5v3* Packet: 4A DE 53 B7 19 DF 18 8D 69 8E 38 61 60 D1 08 57 54 EE 75 39 D1 AE 05 9B
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: 10 BC C0 17 47 96 F3 9E 4D 0C CC F1 D3 E6 F3 02 C4 5A D3 CC 7A 28 63 EF
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x233B8F5E
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: 45 66 CF E2 C7 53 4B EB B8 E4 1D 5B 82 6B 67 00 D0 79 E6 C4 03 AA E6 D7
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x233B8E01
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: B4 D9 4B 62 0C ED C6 DD CD D2 CF 9C 15 99 4E 32 F4 B3 9D 5C D1 6E 5D B7
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: 85 18 37 D8 78 36 B2 C8 96 BF B5 3D E8 7E EA CD ED B7 66 3C 31 5A 0D CF

[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: C7 1B 91 6D 74 F7 87 BE D0 19 E2 82 C9 78 41 7E D5 CF 01 BF AB EF BE 11
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: 70 38 BA 20 17 FB 35 AB 36 F1 AF F2 55 73 F2 37 F5 EE AF 36 3A 84 14 3B
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: FA 09 D4 57 F0 3C 8D AF 5E A3 AB 93 4F FA 04 60 07 28 65 FA C9 5A B5 78
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x1A04
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: A4 AD 2B 40 65 55 42 35 AB 23 B1 38 E2 CF DC 8D 62 C7 A3 9F 1D AA 31 82
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: 4D 7B 68 49 71 11 74 B0 95 01 88 AB 75 25 1C AD 08 63 89 95 4D B4 38 ED

[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: 37 5B 87 19 2E E1 8C 9C 2B FC AD EA CA E6 69 9F CE AB C4 EA 8C CC D5 15
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:44:45 EDT] *tir5v3* Packet: 71 9A 10 9B 7D 2E EF 05 47 1E 1A B5 A9 11 CF 68 B1 2D 8B 1A 1B 5A F9 DF
[Sun 2025-06-29 08:44:45 EDT] Changing state to RUNNING!
[Sun 2025-06-29 08:44:45 EDT] Stripe ignored. (hstart 724 > img. width 724)
[Sun 2025-06-29 08:44:45 EDT] Stripe ignored. (hstop 724 > img. width 640)
[Sun 2025-06-29 08:44:45 EDT] Couldn't add stripe!
[Sun 2025-06-29 08:44:45 EDT] Bad packet size! 2130706433 x 336
[Sun 2025-06-29 08:44:45 EDT] ERROR!!! ('68 03')
[Sun 2025-06-29 08:44:45 EDT] Bad packet header!
[Sun 2025-06-29 08:44:45 EDT] ERROR!!! ('68 05')
[Sun 2025-06-29 08:44:45 EDT] ERROR!!! ('68 06')
[Sun 2025-06-29 08:44:45 EDT] ERROR!!! ('68 07')

[Sun 2025-06-29 08:44:48 EDT] Suspending!
[Sun 2025-06-29 08:44:48 EDT] Changing state to PAUSED!
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Intent: 0x1A05
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Packet: D4 1A 9E 0C 3D 64 A8 4D 00 26 7B EF 2B C3 4D 11 96 D5 23 66 30 D4 E2 BB
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Packet: B3 DC 5A 6E 89 74 07 96 B1 6B A1 99 68 65 79 5A 90 BF 68 A1 D3 30 C4 39
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Packet: B9 13 87 18 C0 EE F4 8B DB 7D FF E2 13 B0 4D 52 AE 2E B7 27 13 47 64 7B
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Packet: 54 70 0B 46 8B 27 CD A3 58 3B 97 E3 16 14 E2 F8 28 6B 46 7A 40 FF 32 67
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Intent: 0x19040033
[Sun 2025-06-29 08:44:48 EDT] *tir5v3* Packet: 16 CB 8A 62 02 39 10 72 45 4C 3C 46 23 A0 C4 5E 38 C8 75 4C 89 6D 74 1B

[Sun 2025-06-29 08:44:49 EDT] Waking!
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Packet: 7A BA 21 C2 C5 9A 8E 53 FD 90 01 34 A0 D1 63 00 3D 95 A1 01 E4 D9 A8 59
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Intent: 0x233502E0
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Packet: 01 9A 4C 7A 88 A7 2D AA 6A F2 44 F8 45 B3 7B 43 4E BD A3 B1 8C E0 37 2D
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Intent: 0x23350115
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Packet: 4A 8A 75 2B BC 3C C5 08 B7 4E B0 E4 F8 9B 27 6B 12 DD 7E 9A EC CD 26 8F
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Packet: 69 BE DB 50 D2 E5 DC F5 2A 6B 84 D6 A1 3B 24 51 1F B7 6B F5 5A 7D 10 D8
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Intent: 0x233B8F5E
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Packet: 67 3D 88 26 EE FC DA CE 4E 31 A1 EC F3 C4 C3 77 9A 4E B2 BE B5 D1 DB 20

[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Intent: 0x233B8E01
[Sun 2025-06-29 08:44:49 EDT] *tir5v3* Packet: F1 DE 0A B4 D3 B3 F2 5F 18 E8 26 B1 B0 A9 5D 63 D3 2B 27 96 C8 C1 FA 7B
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: B3 5B CB 13 91 6C E9 9F 21 BC 38 DE 07 2A F4 76 DB 6D 1F 39 08 F3 8A 2F
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: CC 8C CD 71 33 1A CC 03 2D 5D 6F 16 99 A6 61 4F A3 5D 79 98 65 54 3C 84
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: 05 1F 70 03 6A 65 85 37 D6 68 91 BB BB 7E 00 2B 95 14 BB 68 4B 5F 56 C4
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: FA 33 44 A6 79 B7 C9 CA 2A 28 51 3D 5F 27 F6 B1 C9 5B 2F C9 DC C4 C6 98

[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x1A04
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: BC 8B D2 EE 3E EF 21 7E 95 99 66 13 85 EE 7B D6 2C 63 FD 22 8C C7 D3 BB
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: B2 40 AC 6A 3E 44 64 90 6E 2D 4E EC C2 E8 22 16 6D 82 91 44 3D 6C 41 5F
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: 29 BC 9D E0 34 EF 2A E0 57 69 10 8D 6E EF C2 E5 6A F1 CD 8D 9B 3A 9E 2C
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:44:50 EDT] *tir5v3* Packet: 4C C8 3A 91 C8 6C 45 00 3A CA 51 79 5A A8 E3 6A 3E 6B E8 00 5E 52 85 2B
[Sun 2025-06-29 08:44:50 EDT] Changing state to RUNNING!
[Sun 2025-06-29 08:44:50 EDT] Stripe ignored. (hstart 724 > img. width 724)
[Sun 2025-06-29 08:44:50 EDT] Stripe ignored. (hstop 724 > img. width 640)
[Sun 2025-06-29 08:44:50 EDT] Couldn't add stripe!
[Sun 2025-06-29 08:44:50 EDT] Bad packet size! -1090519039 x 344
[Sun 2025-06-29 08:44:50 EDT] ERROR!!! ('70 4B')
[Sun 2025-06-29 08:44:50 EDT] ERROR!!! ('70 4C')
[Sun 2025-06-29 08:44:50 EDT] Bad packet size! -486539263 x 1716

[Sun 2025-06-29 08:44:53 EDT] Suspending!
[Sun 2025-06-29 08:44:53 EDT] Changing state to PAUSED!
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Intent: 0x1A05
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Packet: E1 75 4D E4 98 A6 A4 92 77 6D FF BD CE 4E 36 1F C7 6E C8 AA FA 06 24 E2

[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Packet: B5 AE 10 AC 15 92 F9 F8 F8 D1 4A 7D 57 0A 7C 14 D8 F9 4A F8 91 DB C0 3C
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Packet: 10 B0 C8 92 AD 58 90 01 05 A4 93 FE 9D 7E DE 3A FB BC 44 77 49 1C 41 93
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Packet: 30 0E 44 9A 6E FC 67 51 E9 BF E1 EA C4 86 7E C3 23 12 A1 5D F7 D6 A1 6E
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Intent: 0x19040033
[Sun 2025-06-29 08:44:53 EDT] *tir5v3* Packet: 70 AF B2 D2 80 21 A6 90 65 B7 35 1F A8 4F 4A 67 31 DD 2C B7 B2 EF DA AE

[Sun 2025-06-29 08:44:55 EDT] Waking!
[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Packet: BB 6E DC 11 85 95 C2 37 67 44 58 0E 68 17 23 D2 1E D2 22 68 5E 53 9D 8A
[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Intent: 0x233502E0
[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Packet: 5A E4 FE 0B 1B A3 4F CB A2 F4 88 79 2C F0 BD 84 FE 34 42 4D 64 60 44 86
[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Intent: 0x23350115
[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Packet: B1 6E 29 E1 B4 A6 C7 44 B2 58 57 1F D5 0E 14 5D 87 F5 4D 45 C4 4B D6 06

[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:44:55 EDT] *tir5v3* Packet: 4A DC C0 30 7F 99 96 AC 36 17 64 43 FF 02 25 56 22 55 64 36 58 EB 76 A5
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x233B8F5E
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: 2A 0F 45 8B 40 A8 66 02 D8 E5 9B 6E 91 0E 60 B8 5B 8F E6 81 0E 08 7B 72
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x233B8E01
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: FA B4 7D 8E EE 59 26 8D F3 D7 D7 59 D9 AF 3E 74 1D 3E 8C D7 B3 E8 8F 99
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: CF EC 10 DF B8 6E 93 31 FD 27 ED 20 B1 8E 69 88 E8 D7 38 26 B7 55 F6 43
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: FD BC 46 96 5A E3 79 6A C2 31 D9 55 62 D3 6D 93 68 19 8B D2 10 73 14 48

[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: BD 98 0F CA 38 3A 86 7B CF E0 5E 48 4A 4B 71 F9 75 6D F9 4B D8 62 D3 63
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: BE 48 2B 73 15 C8 01 23 92 3A ED 77 36 2C F8 13 74 CE 33 ED 66 A8 C8 60
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x1A04
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: 2A CE 00 F6 A8 DB 3E D4 4C 52 0D 8D 75 2F 87 D3 48 7B FF 40 4F 74 83 82
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: 70 4A 19 C8 0D E4 CE 02 DB 01 C8 62 F5 93 FF 42 08 66 C9 DB 76 05 DB B7
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: DA DE B4 24 01 DA 6E 1E A5 7A 02 73 F0 F5 72 BD F1 27 50 F0 55 58 1F 1E

[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:44:56 EDT] *tir5v3* Packet: A8 0F 4C 78 5E 87 4F 0E AB 4F E9 1E 05 DD 94 6F 01 2E FF 1E CE F0 31 1E
[Sun 2025-06-29 08:44:56 EDT] Changing state to RUNNING!
[Sun 2025-06-29 08:44:56 EDT] Stripe ignored. (hstart 724 > img. width 724)
[Sun 2025-06-29 08:44:56 EDT] Stripe ignored. (hstop 724 > img. width 640)
[Sun 2025-06-29 08:44:56 EDT] Couldn't add stripe!
[Sun 2025-06-29 08:44:56 EDT] Bad packet size! -167772159 x 296
[Sun 2025-06-29 08:44:56 EDT] ERROR!!! ('40 BF')
[Sun 2025-06-29 08:44:56 EDT] ERROR!!! ('40 C0')
[Sun 2025-06-29 08:44:56 EDT] ERROR!!! ('40 C1')
[Sun 2025-06-29 08:44:56 EDT] ERROR!!! ('40 C2')
[Sun 2025-06-29 08:44:56 EDT] ERROR!!! ('40 C3')
[Sun 2025-06-29 08:44:56 EDT] ERROR!!! ('40 C4')

[Sun 2025-06-29 08:44:59 EDT] Suspending!
[Sun 2025-06-29 08:44:59 EDT] Changing state to PAUSED!
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Intent: 0x1A05
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Packet: 73 08 A4 A8 95 C8 B9 E2 41 17 69 90 26 74 9F 0F 2E 66 9D C3 BC 9E D4 BB
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Packet: 65 C0 8D 76 37 8D E7 D1 91 E1 9A 04 22 B2 6D B2 D8 C4 91 E7 B0 DE 84 73
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Packet: 73 5E DF E9 03 CD A9 8F 41 E0 1C 5A C1 2F FB 5B 43 55 0D B1 0F E5 33 A0
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Packet: 18 57 1A 73 9E 70 52 88 73 20 31 02 61 11 1F BB D2 8D F6 2E A1 53 3B 52
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Intent: 0x19040033
[Sun 2025-06-29 08:44:59 EDT] *tir5v3* Packet: 61 8D 03 EF 69 82 3E C0 9C B1 29 1C B6 E6 7F 23 18 81 85 29 A1 40 FC FF

[Sun 2025-06-29 08:45:00 EDT] Waking!
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: 9B 8A 61 F1 ED 86 78 F0 9A 13 E7 B2 A1 0E 63 00 1A 21 E6 9A 8A FE 1C 0F
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x233502E0
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: 4A 2C B8 86 4E 5C 11 B3 4F C1 55 8E EE AE A9 60 60 AA 6E C3 4B 88 29 31
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x23350115
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: C3 3E C2 58 FB 12 7C B7 D4 1E 70 A9 7D 63 A7 B7 2B 71 65 D5 F5 C5 20 63
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: D0 BA 9C 0E 64 CC 4D 28 24 48 3A A0 00 23 49 77 7C AB 77 23 9F A3 B9 05

[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x233B8F5E
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: D8 98 12 51 03 FE 05 9F 14 0A C1 59 92 90 A2 CC 31 91 9F 9B B6 1B 83 2D
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x233B8E01
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: 47 FF 2C 48 A7 F2 E0 B8 45 E3 B6 4A 30 27 7E EF 1C 15 7F BE 14 B0 60 B3
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: 08 CF 43 0C F3 14 04 1A 53 98 C2 3D 62 52 20 18 9D C7 DA 52 92 F6 99 12
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: A2 14 64 0E 2A 3D 1E 52 3B D0 82 3F EB DE E9 F8 1B 8E 4A 3C 63 E7 DF 3D
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: 58 D3 84 EA 80 72 8D F6 55 27 0F 33 95 4A 03 17 89 9A F6 72 E6 11 BD 31

[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:45:00 EDT] *tir5v3* Packet: BC 2D 5E 50 9E 92 25 23 7A A5 69 F5 F2 E5 9F 96 F1 A9 E0 F5 4C 6A E3 42
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Intent: 0x1A04
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Packet: 9C 92 FF A3 BA 13 DE 18 65 6D AA 8A 90 25 30 F9 9C 8B B8 3B 4C A9 70 2D
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Packet: 6B F6 77 4E 51 0D B7 B4 7B 5A C0 E0 C0 2D 8B 4A BD F4 7A B7 4C 31 6F 88
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Packet: E3 AA B7 B4 41 39 B2 B5 85 03 C2 CC F6 6F C6 B6 6B 51 E4 F6 31 A1 A6 AB
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:45:01 EDT] *tir5v3* Packet: 11 CF 7A 5A E1 72 04 97 26 46 47 25 6A 55 DC BD 21 A8 48 47 88 B3 2E 6C

[Sun 2025-06-29 08:45:01 EDT] Changing state to RUNNING!
[Sun 2025-06-29 08:45:01 EDT] Stripe ignored. (hstart 724 > img. width 724)
[Sun 2025-06-29 08:45:01 EDT] Stripe ignored. (hstop 724 > img. width 640)
[Sun 2025-06-29 08:45:01 EDT] Couldn't add stripe!
[Sun 2025-06-29 08:45:01 EDT] Bad packet size! -1124073471 x 3116
[Sun 2025-06-29 08:45:02 EDT] Shutting down tracking!
[Sun 2025-06-29 08:45:02 EDT] Shutting down tracking...
[Sun 2025-06-29 08:45:02 EDT] Closing!
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x1A05
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: 3C 47 0E 66 41 E7 2E 7C 01 0E 2E 45 A5 55 8B 75 2D 49 CA 09 44 EB DB 8C
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: D0 D8 E8 BF DE 37 CA 45 78 9B 2F C3 F2 81 63 37 26 81 C3 9B E5 49 65 EF
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: E8 11 86 2E 58 E3 4F 40 A2 DD 77 6C 22 F0 1D 95 24 E2 16 87 47 3C 3F 0A
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: 85 14 E1 8C DE FA 0F 0D 53 F2 0C 93 94 E9 0B 01 0C 79 1E A1 1F 2E B8 A7
[Sun 2025-06-29 08:45:02 EDT] Closing the TIR5 camera.
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x1905012C
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: B0 EE 7F CE 0A D2 08 1F B3 95 FE AE A4 01 12 06 17 5E 2A 00 9F 2B 0C 9A
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x23350280
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: 40 78 62 61 ED 5A 47 6C A3 95 FB 2F 72 F2 C4 70 A0 1C 7B A1 FE 92 35 28
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x23350107
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: F3 9F 6F 9C 4C 29 16 B3 8A 03 0E 26 56 98 25 AD 63 3E 9F 27 08 3F 8F 83
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:45:02 EDT] *tir5v3* Packet: 4A 1E B2 AB EB 0A 48 5A A8 96 05 D5 8C 0E C1 57 E0 AA 8C 05 DD EF 9D 7D
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x233B8F78
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: BF AE 2C 9D 35 3E B3 EA E7 9E F4 30 94 61 F3 7C 5A DC 8B 1B 67 6B 4B F3
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x233B8E00
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: 98 06 A5 FB B3 62 79 0A 4E D4 1A B2 BE 01 50 B2 31 91 4E F7 C5 A8 07 50
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: AC 0E 65 2C 36 43 3A DC 3D ED 3E 3F E6 E5 B1 E3 EF C6 E4 3F 21 16 87 6F
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: 6E 1C 9C DA 82 58 E8 E3 84 1E 27 BF FA F2 42 70 41 42 75 F2 58 64 61 3D
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: 83 2B F1 62 76 08 75 14 E2 5C A6 66 DB F0 E8 02 AF F2 66 5C 1B A7 D1 91
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: 84 F2 67 99 4C 3A C4 6D 1A 3B 75 8F 4F 57 87 BB 21 F4 AC 09 64 5B EC CA
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: 27 99 0A B3 8E 04 1A 27 80 D4 EB ED EE 27 62 7E 76 3C 05 32 DA 67 DE E3
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Intent: 0x1A00
[Sun 2025-06-29 08:45:03 EDT] *tir5v3* Packet: F2 D8 95 F1 D9 06 7A 71 10 2D FF 14 47 17 94 1B 12 2B 09 39 E3 87 B0 9C
[Sun 2025-06-29 08:45:03 EDT] Closing TrackIR.
[Sun 2025-06-29 08:45:03 EDT] Releasing TrackIR interface.
[Sun 2025-06-29 08:45:03 EDT] TrackIR interface released.
[Sun 2025-06-29 08:45:03 EDT] Closing TrackIR handle.
[Sun 2025-06-29 08:45:03 EDT] Exiting libusb.
[Sun 2025-06-29 08:45:03 EDT] Libusb exited.
[Sun 2025-06-29 08:45:03 EDT] TIR5 camera closed.
[Sun 2025-06-29 08:45:03 EDT] Changing state to STOPPED!
[Sun 2025-06-29 08:45:03 EDT] Master closing socket 39
[Sun 2025-06-29 08:45:03 EDT] Closing semaphore 20 (pid 25173)!

[Sun 2025-06-29 08:45:05 EDT] Going to create lock '/home/mario/.config/linuxtrack/ltr_server.lock' => 21!
[Sun 2025-06-29 08:45:05 EDT] Lock 21 success!
[Sun 2025-06-29 08:45:05 EDT] Passing the lock to protect fifo (pid 25173)!
[Sun 2025-06-29 08:45:05 EDT] Socket created!
[Sun 2025-06-29 08:45:05 EDT] Other master gave up, gui master taking over!
[Sun 2025-06-29 08:45:05 EDT] Starting as master!
[Sun 2025-06-29 08:45:05 EDT] Tracking initialized!
[Sun 2025-06-29 08:45:05 EDT] Adding fd 22
[Sun 2025-06-29 08:45:05 EDT] Device Type: Track IR
[Sun 2025-06-29 08:45:05 EDT] Changing state to INITIALIZING!
[Sun 2025-06-29 08:45:05 EDT] Loading library 'libtir'
[Sun 2025-06-29 08:45:05 EDT] Running!
[Sun 2025-06-29 08:45:05 EDT] Changing state to INITIALIZING!
[Sun 2025-06-29 08:45:05 EDT] Initializing the tracker.
[Sun 2025-06-29 08:45:05 EDT] Lib loaded, prefs read...
[Sun 2025-06-29 08:45:05 EDT] Initializing libusb.
[Sun 2025-06-29 08:45:05 EDT] Libusb initialized successfuly.
[Sun 2025-06-29 08:45:05 EDT] Libusb debug level set.
[Sun 2025-06-29 08:45:05 EDT] Requesting device list.
[Sun 2025-06-29 08:45:05 EDT] Device list received (10 devices).
[Sun 2025-06-29 08:45:05 EDT] Device is a TrackIR (131D:0159).
[Sun 2025-06-29 08:45:05 EDT] Opening handle to the device found.
[Sun 2025-06-29 08:45:05 EDT] Handle opened successfully.
[Sun 2025-06-29 08:45:05 EDT] Freeing device list.
[Sun 2025-06-29 08:45:05 EDT] Device list freed.
[Sun 2025-06-29 08:45:05 EDT] Requesting TrackIR configuration.
[Sun 2025-06-29 08:45:05 EDT] TrackIR configuration received.
[Sun 2025-06-29 08:45:05 EDT] TrackIR already in requested configuration.
[Sun 2025-06-29 08:45:05 EDT] Trying to claim TrackIR interface.
[Sun 2025-06-29 08:45:05 EDT] TrackIR interface claimed.
[Sun 2025-06-29 08:45:05 EDT] Requesting TrackIR configuration.
[Sun 2025-06-29 08:45:05 EDT] TrackIR configuration received.
[Sun 2025-06-29 08:45:05 EDT] Device configuration is OK.
[Sun 2025-06-29 08:45:05 EDT] Device 8.
[Sun 2025-06-29 08:45:05 EDT] Initializing TrackIR 5 revision 3.
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A00
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 59 C8 5D BE F8 29 53 EA 01 59 64 72 69 91 D2 3D D8 E4 11 F3 AB 92 FD E5
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A00
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: AB 11 77 85 D4 F7 E1 C3 30 0A 16 1B 0C 70 7F 7E D9 7B F0 57 E9 89 68 DD

[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: F1 70 5E AF 82 6F 26 09 74 5D EA 37 8D F5 4D A8 01 47 28 7F 97 39 70 EE
[Sun 2025-06-29 08:45:05 EDT] Requesting status.
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 20 56 FF 2C 40 59 48 C2 38 F7 44 A7 1D 4D 1B 7A 38 E6 08 2D A0 B0 2E 5D
[Sun 2025-06-29 08:45:05 EDT] Status packet: 10 20 01 00 01 44 60
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A01
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 1B 15 E6 34 B6 B9 33 45 8A 0D 8E 4C 45 85 90 EC A3 A2 08 1D 16 5A 25 43
[Sun 2025-06-29 08:45:05 EDT] Requesting status.
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 6C AE 1A 4C C1 3C 2F F4 53 08 27 98 F2 34 26 3F 79 83 CF 66 4F AD 6E 6C
[Sun 2025-06-29 08:45:05 EDT] Status packet: 0F 20 01 00 01 5E 53
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A02
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 9A C3 6C 77 1B 16 96 D7 56 D2 96 CA 25 FE F2 BD F1 E4 E4 30 A0 90 FF 06

[Sun 2025-06-29 08:45:05 EDT] Requesting status.
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 90 70 3C DF 77 FF 45 0E 5A 5C 84 82 32 56 18 FD 7B 23 EF 39 08 15 1D 38
[Sun 2025-06-29 08:45:05 EDT] Status packet: 10 20 01 00 01 B4 6D
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A03
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 9B 84 E4 2F D7 55 6C B5 8C 6B FA BA 86 56 3F 08 89 E4 20 86 11 37 75 4A
[Sun 2025-06-29 08:45:05 EDT] Requesting status.
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x1A07
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 49 48 10 AD FA A0 93 2B F7 28 97 2C B4 02 27 6F 88 3A AE 90 06 43 B1 8C
[Sun 2025-06-29 08:45:05 EDT] Status packet: 0F 20 01 01 01 31 91
[Sun 2025-06-29 08:45:05 EDT] Flushing packets...

[Sun 2025-06-29 08:45:05 EDT] Data receive request timed out!
[Sun 2025-06-29 08:45:05 EDT] Sending get_conf request.
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Intent: 0x17
[Sun 2025-06-29 08:45:05 EDT] *tir5v3* Packet: 4F 28 8C 05 9F CC 19 4E C7 B9 E2 F2 31 CA 89 5D 7F 16 84 EE 14 02 9C A5
[Sun 2025-06-29 08:45:06 EDT] Requesting data...
[Sun 2025-06-29 08:45:06 EDT] Requesting data...
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x1905012C
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: DF 5E 91 34 3F 96 D2 66 00 33 F0 EE F1 54 B6 AB 36 1C DC 01 32 39 80 BE
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x23350280
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 07 C0 0E 77 C8 B9 30 4D 1D 0C F4 F0 72 16 C8 78 05 5A CD BB 64 03 63 40
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x23350107
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: CF 8C 57 38 F2 26 F8 FC 9D C0 E6 BA C6 2F 77 12 F3 E4 28 BB 62 2D 75 2F
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 70 44 1E 37 E1 94 DA 56 94 AE 5B C4 AA F8 84 90 63 89 94 DA B4 87 C4 DD

[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x233B8F78
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: E5 B0 55 F3 90 DB F4 D1 BE 20 3F 7A 55 0B 28 B0 CF 4A A9 54 63 0C F6 F7
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x233B8E00
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 2C A3 8C C3 D0 80 DD 01 85 0B AD C4 B6 6C E5 F6 E6 C4 01 0E EB D1 E0 94
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 58 1B 3A 85 C6 B9 32 10 22 42 E1 F4 29 66 FF D7 2B FE 43 10 AB 29 4A AD
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 07 1F C9 A1 5A 6A BE BC BE 1E 4E 24 D3 C9 27 B4 BD 82 1B BC 28 46 71 6B
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 5C A8 CA CB D7 48 E2 9F EB 3D 46 1B EA FB 2E 21 1F 4D EA 46 B6 A7 97 D1

[Sun 2025-06-29 08:45:06 EDT] Going to start camera.
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 73 D5 2E 6F F3 BE C7 91 89 E4 04 6C 83 EF A9 8D 98 B3 88 C7 9E A7 C9 88
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x233502E0
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: ED 8D 51 93 44 68 68 6F D6 CE 16 E9 16 B7 C8 1A 23 E7 09 CC D8 A2 49 60
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x23350115
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: E7 32 6F F7 B1 9F EC 10 07 BE 6B 9B 2D 41 F6 5B 25 FE 12 ED 27 35 38 30
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 1C 4A 71 3B 31 79 6D A1 6E 1F 05 3A CD 73 A6 8C 0F 36 CD 05 2F F2 11 41
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x233B8F5E
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 69 18 69 75 28 3B C2 9F 52 16 13 E4 94 81 03 D5 69 15 48 10 B1 57 E4 7F

[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x233B8E01
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 86 6E 54 51 A6 CA 69 0F 41 92 4B 24 9D C0 F7 40 A6 C3 C1 AA 60 2B CF A9
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 32 1F FB 5D 32 71 CB 8D 90 96 50 2B 81 91 BD CC 95 31 8E 8C 29 34 17 EB
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 79 AD 21 53 2E 21 70 2D 7E 49 8E 11 28 60 BB 78 8B C2 09 48 08 9E 32 96
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19040000
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 94 4A 42 A9 BB 5C 56 DB 7F C0 B1 1D B2 7B BF 50 C4 C6 B1 7F 5F 3C BB 69
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 44 B7 5D 32 12 40 83 85 CD 2E B5 23 09 63 A8 05 80 61 80 40 AA 45 27 5B

[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Intent: 0x1A04
[Sun 2025-06-29 08:45:06 EDT] *tir5v3* Packet: 19 88 F0 1D 43 F7 D4 9D 29 F6 81 9D 3F 8E CB F5 B1 57 58 59 DA D8 B3 5A
[Sun 2025-06-29 08:45:07 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:45:07 EDT] *tir5v3* Packet: 7A B9 64 C7 50 E4 B7 6E 27 AE 42 C4 D8 5D 61 40 68 72 41 5D C5 15 B5 1E
[Sun 2025-06-29 08:45:07 EDT] *tir5v3* Intent: 0x19040022
[Sun 2025-06-29 08:45:07 EDT] *tir5v3* Packet: 0D 4A A2 30 E9 E6 E9 4D A3 19 37 64 A9 59 13 EC 1D 73 15 A3 60 7D B6 A1
[Sun 2025-06-29 08:45:07 EDT] Setting up Cap
[Sun 2025-06-29 08:45:07 EDT] Changing state to RUNNING!
[Sun 2025-06-29 08:45:07 EDT] *tir5v3* Intent: 0x19050118
[Sun 2025-06-29 08:45:07 EDT] *tir5v3* Packet: 18 98 9E A6 1E 6C F0 C5 9C DA AB 85 27 D1 B8 AA BD 16 B2 D0 56 CF BB 6B
[Sun 2025-06-29 08:45:07 EDT] Bad packet size! -1275068415 x 336
[Sun 2025-06-29 08:45:07 EDT] ERROR!!! ('68 DA')

[Sun 2025-06-29 08:45:14 EDT] Shutting down tracking!
[Sun 2025-06-29 08:45:14 EDT] Shutting down tracking...
[Sun 2025-06-29 08:45:14 EDT] Closing!
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x1A05
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 90 29 BD C1 A4 74 54 3E 1B 72 AA 0B 97 06 E5 E3 CC C5 3C 8C 66 F9 F6 18
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: EC B7 5E A3 A0 44 68 DF 27 D3 B7 66 EE 2A 10 FA 61 BF DF 44 23 EC 80 AF
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 56 6A 40 F0 52 C9 AA AC DB CA 2F CA AD 56 9D 65 BC 2F 8F CD 86 F0 24 65
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 69 F6 A4 2F 9C 0F 73 8F 61 3D 39 0D A8 83 3C 72 30 3E 0F 95 4F 9B 24 1C
[Sun 2025-06-29 08:45:14 EDT] Closing the TIR5 camera.
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x1905012C
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 72 48 83 4A 87 D9 FF 7D 7D 2F 64 E7 AB A9 ED DF E2 13 87 65 36 F9 95 C9
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x23350280
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 2E A4 4F 36 C7 64 1F 05 BB FB 26 AE 78 A3 DD 92 30 D4 3B 1D 5F 1D 18 E6
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x23350107
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 74 18 17 E9 42 2F 8D 92 63 53 F7 31 66 6D D2 C6 53 9D 6A 30 DD 9A B0 19
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x23350000
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 8B CF F5 B9 1F D5 D1 35 BE 14 65 4C 72 95 43 9D A0 95 43 72 06 96 BD 70
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x233B8F78
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: EA 7F B0 C2 87 EA 91 7B A3 AF 74 60 6A 0F 88 4A 5B 5F 12 FB CC B2 3B 0F
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x233B8E00
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: 1D EA B2 6D 7C BC E4 32 0F 46 1C 10 E5 BF BF 36 34 2C 46 BC EE A1 EB 01
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x19090001
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: D7 D7 C7 DA 19 6D D5 1F 44 38 87 BE 6A 06 28 86 16 4B 45 D5 44 79 7A 8A
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Intent: 0x19030005
[Sun 2025-06-29 08:45:14 EDT] *tir5v3* Packet: C5 29 6D C8 D8 1D A0 A1 F5 B9 0E D0 A4 14 E9 0B A7 42 11 D0 D9 27 DE 1E
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Intent: 0x19090000
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Packet: AC 7F AD CC DE D9 EF 49 A1 C9 66 F5 05 61 FA 78 0C C5 61 F5 8A 09 48 9B
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Intent: 0x1A06
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Packet: F5 BF 3B C0 D9 D7 37 86 A5 26 5F 95 5F 01 5E C5 42 4C 21 3D 40 2D BB A2
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Intent: 0x13
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Packet: 51 E1 84 8B A4 3B CA 64 14 A2 9B 9B 47 B1 FA DC 11 16 3A D6 3E 02 F7 7B
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Intent: 0x1A00
[Sun 2025-06-29 08:45:15 EDT] *tir5v3* Packet: 97 36 E5 46 7C 90 00 5D 14 3B 01 4F 06 65 63 A8 00 2A EF B1 F9 CC C2 F4
[Sun 2025-06-29 08:45:15 EDT] Closing TrackIR.
[Sun 2025-06-29 08:45:15 EDT] Releasing TrackIR interface.
[Sun 2025-06-29 08:45:15 EDT] TrackIR interface released.
[Sun 2025-06-29 08:45:15 EDT] Closing TrackIR handle.
[Sun 2025-06-29 08:45:15 EDT] Exiting libusb.
[Sun 2025-06-29 08:45:15 EDT] Libusb exited.
[Sun 2025-06-29 08:45:15 EDT] TIR5 camera closed.
[Sun 2025-06-29 08:45:15 EDT] Changing state to STOPPED!
[Sun 2025-06-29 08:45:15 EDT] Master closing socket 22
[Sun 2025-06-29 08:45:15 EDT] Closing semaphore 21 (pid 25173)!

Also tested in the GUI is the Firmware Install button
I downloaded the latest TRACKIR software manually from https://www.trackir.com/downloads/
Then in the Extractor dialog that popped up after selecting the Insatll Firmware button I selected the Extract from installer button
It allowed me to browse to the downlaoded exe and select it, and then a doialog popped up and stated

"NP's TrackIR installer might pop up now.
If it does, install it with all components to the default location, so the firmware and other necessary elements can be extracted.
The software will be installed to the wine sandbox, that will be deleted afterwards, so there are no leftovers."

It then started to process the file, I could see a wine dialog box in process but failed with 
Some of the files needed to fully utilize TrackIR were not found! Please see the log for more details.
Logfile
[Sun 2025-06-29 08:56:07 EDT]     WINEARCH='win32'
[Sun 2025-06-29 08:56:07 EDT] Launching wine command: 'wine --version'
[Sun 2025-06-29 08:56:07 EDT] Looking for existing spec.txt...[Sun 2025-06-29 08:56:07 EDT] Found '/opt/linuxtrack/bin/../share/linuxtrack/spec.txt'.[Sun 2025-06-29 08:56:07 EDT] spec.txt found and read.[Sun 2025-06-29 08:56:07 EDT] Looking for existing sources.txt...[Sun 2025-06-29 08:56:07 EDT] Found '/opt/linuxtrack/bin/../share/linuxtrack/sources.txt'.[Sun 2025-06-29 08:56:07 EDT] sources.txt found and read.
[Sun 2025-06-29 08:57:37 EDT] Found blob. Commencing extraction.
[Sun 2025-06-29 09:01:09 EDT] Initializing wine and running installer /home/mario/Downloads/TrackIR_5.5.3.exe[Sun 2025-06-29 09:01:09 EDT]     WINEARCH='win32'
[Sun 2025-06-29 09:01:09 EDT] Launching wine command: 'wine regedit" "/opt/linuxtrack/bin/../share/linuxtrack/win7.reg'

[Sun 2025-06-29 09:01:14 EDT] Wine finished with exitcode 53 (Normal exit).[Sun 2025-06-29 09:01:14 EDT] 004c:err:ole:StdMarshalImpl_MarshalInterface Failed to create ifstub, hr 0x80004002
004c:err:ole:CoMarshalInterface Failed to marshal the interface {6d5140c1-7436-11ce-8034-00aa006009fa}, hr 0x80004002
004c:err:ole:apartment_get_local_server_stream Failed: 0x80004002
004c:err:ole:start_rpcss Failed to open RpcSs service
0054:err:ole:StdMarshalImpl_MarshalInterface Failed to create ifstub, hr 0x80004002
0054:err:ole:CoMarshalInterface Failed to marshal the interface {6d5140c1-7436-11ce-8034-00aa006009fa}, hr 0x80004002
0054:err:ole:apartment_get_local_server_stream Failed: 0x80004002
wine: configuration in L"/tmp/wineH874lT" has been updated.
wine: failed to open "regedit\" \"/opt/linuxtrack/bin/../share/linuxtrack/win7.reg": c0000135


[Sun 2025-06-29 09:01:20 EDT] Commencing analysis of directory '/tmp/wineH874lT'...
[Sun 2025-06-29 09:01:20 EDT] ===============================[Sun 2025-06-29 09:01:20 EDT] Couldn't extract poem1.txt![Sun 2025-06-29 09:01:20 EDT] Couldn't extract tir4.fw![Sun 2025-06-29 09:01:20 EDT] Couldn't extract tir5.fw![Sun 2025-06-29 09:01:20 EDT] Couldn't extract poem2.txt![Sun 2025-06-29 09:01:20 EDT] Couldn't extract sn4.fw![Sun 2025-06-29 09:01:20 EDT] Couldn't extract tir5v2.fw![Sun 2025-06-29 09:01:20 EDT] Couldn't extract game data![Sun 2025-06-29 09:01:20 EDT] Couldn't extract TIRViews.dll!

Terminal showed this information while wrking through this process
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
kf.service.services: KServiceTypeTrader: serviceType "ThumbCreator" not found
Can't find a blob matching the installer.
"/tmp/wine8FSRgE"
"Analyzing " "/tmp/wine8FSRgE/drive_c/Program Files/Common Files/System/ADO/msado15.dll"
"Analyzing " "/tmp/wine8FSRgE/drive_c/Program Files/Common Files/System/OLE DB/msdaps.dll"
"Analyzing " "/tmp/wine8FSRgE/drive_c/Program Files/Common Files/System/OLE DB/msdasql.dll"
"Analyzing " "/tmp/wine8FSRgE/drive_c/Program Files/Common Files/System/OLE DB/oledb32.dll"
"Analyzing " "/tmp/wine8FSRgE/drive_c/Program Files/Internet Explorer/iexplore.exe"
"Analyzing " "/tmp/wine8FSRgE/drive_c/Program Files/Windows Media Player/wmplayer.exe"
"Analyzing " "/tmp/wine8FSRgE/drive_c/Program Files/Windows NT/Accessories/wordpad.exe"

Information from the help button
NP Firmware Extractor
Some NP devices (TrackIR4+ and SmartNav4) require a firmware that has to be loaded each time the device is started. Also the Linuxtrack-Wine bridge utilizes some informations contained in the package (the infamous poetry, list of supported games along with their IDs and keys for the "enhanced" interface). The firmware is extracted using Wine from the TrackIR windows package downloaded directly from NP. 
Firmware extraction ￼ 
When the Extraction dialog pops up, press the Download button to download the latest driver package. By default the newest package available is downloaded; if it is not accessible, select any other from the combo-box to the left of the button (all packages listed there contain the same firmware, only the game list might differ a bit) and try again. 
When the package is downloaded/browsed to, a dialog containing short instructions pops up. After pressing OK the package is installed to a temporary Wine prefix. 
On Linux, usual installation dialog will pop up - just install the whole thing. Newer packages contain some MS runtime and NP USB drivers install, both of them has to be installed in order to install the main package. Don't worry though, none of the installed things is going to last - when the data are extracted, everything is going to be removed (and the installation happens in temporary Wine prefix). 
On Macs, due to Wine configuration peculiarities, the installation should be performed silently. 
When the installer finishes, the extraction is started - you can follow its progress in the status part of the Extractor dialog. When the extraction is finished, the dialog closes by itself. 
If you don't have access to the Internet, just download any of the listed packages elsewhere and using the Extract from installer button find and open the one you downloaded. 
If you encounter any difficulties using the above methods, you can try to install the driver by hand (using Wine or install on windows machine and copy the result over). Then just press the Extract from unpacked, browse to the directory (the whole directory!) containing the NP software (e.g. ~/.wine/drive_c/Program\ Files/NaturalPoint/TrackIR5) and press Open button for the extraction to begin. 
Why so complicated?
Maybe you wander, why is the whole thing that complicated, or why to download ~20MB package instead of 100KB one? 
There are two main reasons that led to this decision: 
The first one is the neglect from the NP's side - I asked couple of relatively simple questions (e.g. what is their take on Linuxtrack in Wine, ...), and after a year of waiting without any real answer, I came to a conclusion that this is not a way to go. 
The second reason was their attempt to impose artificial limitations on the Linuxtrack itself in order to grant me a permission to use SmartNav 4 firmware; they asked for disabling SmartNav4 functionality on Mac OS, so people couldn't use Linuxtrack to emulate a mouse. Besides of being technically impossible to do (how a library can check what it is being used for), it would be completely against the Linuxtrack's spirit. 
For those reasons (and couple of others) I decided to cut all the bonds (they used to host Linuxtrack firmware package in the past - per their own request) and using Wine was the only logical choice. Given the fact, that this step is mostly one time only, I hope the inconvenience level is not too high. 
Technical informations
The extracted files are in the following path: ~/.config/linuxtrack/tir_firmware and it contains the following files: 
poem1.txt The first haiku verse - so called DLL signature
poem2.txt The second haiku verse - so called Application signature
gamedata.txt List of TrackIR enhanced games along with their IDs
sn4.fw.gz SmartNav4 firmware
tir4.fw.gz TrackIR4 firmware
tir5.fw.gz TrackIR5 firmware
tir5v2.fw.gz TrackIR5 rev 2 firmware
The haiku verses are used by most games to verify that there is a TrackIR software on the other side. It was used to prevent other programs from emulating the interface (most notably FreeTrack). The claim was based on the fact, that those strings contain NP's trademark and they are copyrighted. My belief is, that since most games refuse to work without it, it is a part of the interface and therefore not copyright-able in order to provide means of interoperability. Also Fair use should be applicable in this case. 
The gamedata.txt list is extracted from file sgl.dat; the file is encrypted using RC4 stream cipher, using first 5 bytes in MD5 hash of string "NaturalPoint" as a key. The payload is XML, containing data on supported games, of which the only relevant part is a game ID, its name and for games using the "enhanced" interface there are communication keys. The Linuxtrack-Wine bridge uses this info to determine game's name when passed its ID and the keys to emulate the enhanced interface when necessary. 

#### **Test 3: Help System**
```bash
echo "=== HELP SYSTEM TEST ==="
echo "Help files present:"
ls -la /opt/linuxtrack/share/linuxtrack/help.qch 2>/dev/null && echo "✅ help.qch found" || echo "❌ help.qch missing"
ls -la /opt/linuxtrack/share/linuxtrack/help.qhc 2>/dev/null && echo "✅ help.qhc found" || echo "❌ help.qhc missing"

echo ""
echo "Manual Test Required:"
echo "1. Open LinuxTrack application"
echo "2. Press F1 or go to Help menu"
echo "3. Qt5 help window should open"
echo "4. Navigate through help topics"
echo ""
echo "Result: [PASS/FAIL] - Help system works: ____"
```
RESULTS
=== HELP SYSTEM TEST ===
Help files present:
❌ help.qch missing
❌ help.qhc missing

However the help documents were in the GUI when I checked them
---

## 📋 **KUBUNTU 25.10 TEST REPORT**

Copy and fill out this report:

```
=== LinuxTrack Qt5 - Kubuntu 25.10 Test Report ===
Date: $(date)
Tester: [Your Name]
System: Kubuntu 25.10
Qt Version: $(qmake-qt5 -query QT_VERSION)
Kernel: $(uname -r)
s
- System Install: [SUCCESS/FAILED]
- Desktop Integration: [SUCCESS/FAILED]

FUNCTIONALITY PHASE:
- Menu Integration: [PASS/FAIL]
- Application Launch: [PASS/FAIL]
- Help System: [PASS/FAIL]
- Overall Functionality: [PASS/FAIL]

ISSUES FOUND:
1. [List any issues]
2. [Describe problems]

OVERALL ASSESSMENT:
Status: [PASS/FAIL/PARTIAL]
Ready for Kubuntu: [YES/NO]
Recommended: [YES/NO/WITH FIXES]

NEXT STEPS:
[What should be done next]
```

---

## 🚀 **After Testing**

### **If All Tests Pass:**
1. Document success in main progress tracker
2. Test on additional Ubuntu-based distributions  
3. Move to testing other distribution families

### **If Issues Found:**
1. Document specific problems
2. Check if issues are Kubuntu-specific
3. Report back for fixes before testing other distros

### **Quick Commands for Different Results:**

#### **Complete Success:**
```bash
echo "✅ Kubuntu 25.10: FULL SUCCESS - LinuxTrack Qt5 ready for production!"
```

#### **Partial Success:**
```bash
echo "⚠️  Kubuntu 25.10: PARTIAL SUCCESS - Core functionality works, minor issues found"
```

#### **Major Issues:**
```bash
echo "❌ Kubuntu 25.10: ISSUES FOUND - Needs fixes before proceeding"
```

---

**Start testing now! This should take about 30-45 minutes total.** 🚀

**Next distributions to test:** Ubuntu 24.04 LTS, Linux Mint, then Fedora/openSUSE 
ONe Note:  When using the Dark Theme in the OS the lines in the Detaled Axis setup is drawn black and is very hard to see in the Dark Theme