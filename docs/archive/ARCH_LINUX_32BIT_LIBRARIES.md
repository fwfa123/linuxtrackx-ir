# Arch Linux 32-bit Libraries Guide

## Status: `build_32bit_libs.sh` removed

The script `./scripts/build_32bit_libs.sh` has been **removed** (replaced by the modern CMake build). Do not reference it in docs or runbooks.

## Default CMake build: lib32-mxml and lib32-liblo not required

The **current** CMake 32-bit `liblinuxtrack` and wine bridge **do not** require `lib32-mxml` or `lib32-liblo` for the default build. You only need:

- **32-bit headers** for building: `lib32-glibc` and `lib32-gcc-libs` (multilib).

If some optional or legacy component ever needs lib32-mxml or lib32-liblo, that will be documented separately.

## Manual build (advanced / legacy)

If you see "skipping incompatible" or similar for mxml/liblo in a **non-default** build and you need 32-bit versions, you can build them manually. The `build_32bit_libs.sh` script is no longer available.

### Build lib32-mxml

```bash
sudo pacman -S --needed base-devel
export CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" PKG_CONFIG_PATH='/usr/lib32/pkgconfig'
cd /tmp
wget https://github.com/michaelrsweet/mxml/releases/download/v3.3.1/mxml-3.3.1.tar.gz
tar -xf mxml-3.3.1.tar.gz && cd mxml-3.3.1
./configure --libdir=/usr/lib32 --includedir=/usr/include/mxml32 --build=i686-pc-linux-gnu --prefix=/usr
make -j$(nproc) && sudo make install
sudo ln -sf /usr/lib32/libmxml.so.1 /usr/lib32/libmxml.so
```

### Build lib32-liblo

```bash
cd /tmp
wget https://github.com/radarsat1/liblo/releases/download/0.32/liblo-0.32.tar.gz
tar -xf liblo-0.32.tar.gz && cd liblo-0.32
./configure --libdir=/usr/lib32 --includedir=/usr/include/liblo32 --build=i686-pc-linux-gnu --prefix=/usr
make -j$(nproc) && sudo make install
sudo ln -sf /usr/lib32/liblo.so.7 /usr/lib32/liblo.so
```

### PKGBUILD

The old `scripts/lib32-mxml.PKGBUILD` was archived to `docs/archive/scripts/root/lib32-mxml.PKGBUILD`; it is historical and not required by the current MinGW bridge build.

## Building LinuxTrack (CMake)

Use CMake, not autotools:

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

## References

- [Arch Linux 32-bit package guidelines](https://wiki.archlinux.org/title/32-bit_package_guidelines)
- [Arch Linux readme](readme/arch-linux.md)
