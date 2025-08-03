# Arch Linux 32-bit Libraries Guide

This guide explains how to build missing 32-bit libraries for LinuxTrack on Arch Linux.

## Problem

LinuxTrack requires 32-bit versions of some libraries for Wine bridge compatibility, but these are not available in the official Arch repositories or AUR:

- `lib32-mxml` (Mini-XML library)
- `lib32-liblo` (Open Sound Control library)

## Solutions

### Option 1: Automated Build Script (Recommended)

Use the provided script to automatically build and install the missing 32-bit libraries:

```bash
./scripts/build_32bit_libs.sh
```

This script will:
- Download source code for both libraries
- Build 32-bit versions with proper flags
- Install to `/usr/lib32/`
- Create necessary symlinks
- Clean up build files

### Option 2: Manual Build

If you prefer to build manually or the script fails:

#### Build lib32-mxml:

```bash
# Install dependencies
sudo pacman -S --needed base-devel multilib-devel

# Set 32-bit compiler flags
export CFLAGS="-m32"
export CXXFLAGS="-m32"
export LDFLAGS="-m32"
export PKG_CONFIG_PATH='/usr/lib32/pkgconfig'

# Download and build mxml
cd /tmp
wget https://github.com/michaelrsweet/mxml/releases/download/v3.3.1/mxml-3.3.1.tar.gz
tar -xf mxml-3.3.1.tar.gz
cd mxml-3.3.1

./configure \
  --program-suffix="-32" \
  --libdir=/usr/lib32 \
  --libexecdir=/usr/lib32 \
  --includedir=/usr/include/mxml32 \
  --build=i686-pc-linux-gnu \
  --prefix=/usr

make -j$(nproc)
sudo make install
sudo ln -sf /usr/lib32/libmxml.so.1 /usr/lib32/libmxml.so
```

#### Build lib32-liblo:

```bash
# Download and build liblo
cd /tmp
wget https://github.com/radarsat1/liblo/releases/download/0.32/liblo-0.32.tar.gz
tar -xf liblo-0.32.tar.gz
cd liblo-0.32

./configure \
  --program-suffix="-32" \
  --libdir=/usr/lib32 \
  --libexecdir=/usr/lib32 \
  --includedir=/usr/include/liblo32 \
  --build=i686-pc-linux-gnu \
  --prefix=/usr

make -j$(nproc)
sudo make install
sudo ln -sf /usr/lib32/liblo.so.7 /usr/lib32/liblo.so
```

### Option 3: Create AUR Packages

For long-term community benefit, you can create proper AUR packages:

1. Use the PKGBUILD templates in `scripts/`
2. Test the packages locally
3. Submit to AUR for others to use

## Verification

After building, verify the libraries are installed:

```bash
ls -la /usr/lib32/libmxml*
ls -la /usr/lib32/liblo*
```

## Building LinuxTrack

Once the 32-bit libraries are installed, build LinuxTrack:

```bash
./configure --prefix=/usr/local --enable-ltr-32lib-on-x64
make -j$(nproc)
sudo make install
```

## Troubleshooting

### "skipping incompatible" errors
- Ensure 32-bit libraries are in `/usr/lib32/`
- Check that symlinks are correct
- Verify compiler flags include `-m32`

### Build failures
- Install `multilib-devel` package
- Ensure `gcc-multilib` is installed
- Check that all dependencies are available

### Wine bridge issues
- Ensure wine-stable is installed (not wine-staging)
- Verify wine32 is properly installed
- Check that 32-bit libraries are accessible to winegcc

## References

- [Arch Linux 32-bit package guidelines](https://wiki.archlinux.org/title/32-bit_package_guidelines)
- [LinuxTrack GitHub Issue #206](https://github.com/uglyDwarf/linuxtrack/issues/206) 