# Build environment for LinuxTrack X-IR AppImage
#
# Uses Ubuntu 22.04 LTS for maximum glibc compatibility (2.35) and
# deterministic library soname versions (ICU 70, Qt 6.2, etc.).
# Building on the oldest supported LTS ensures the AppImage runs on
# all distros from 2022 onward (Fedora 36+, Ubuntu 22.04+, Debian 12+,
# Arch, openSUSE Tumbleweed, RHEL 9+).
#
# Usage:
#   docker build -t linuxtrackx-appimage -f scripts/appimage/Dockerfile .
#   docker run --rm -v "$PWD:/src" -w /src linuxtrackx-appimage \
#       scripts/appimage/v2/ci_build.sh

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC
ENV APPIMAGE_EXTRACT_AND_RUN=1

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake bison flex pkg-config \
    autoconf automake libtool \
    git wget curl ca-certificates file \
    patchelf sqlite3 squashfs-tools desktop-file-utils \
    qt6-base-dev qt6-base-dev-tools \
    qt6-tools-dev qt6-tools-dev-tools \
    libqt6sql6-sqlite libqt6opengl6-dev \
    libgl-dev libglu1-mesa-dev \
    libx11-dev libx11-xcb-dev libxrender-dev \
    libxcb1-dev libxcb-glx0-dev \
    libxcb-icccm4-dev libxcb-image0-dev \
    libxcb-keysyms1-dev libxcb-randr0-dev \
    libxcb-render-util0-dev libxcb-shape0-dev \
    libxcb-shm0-dev libxcb-sync-dev \
    libxcb-xfixes0-dev libxcb-xinerama0-dev \
    libxcb-xkb-dev libxcb-xinput-dev \
    libxkbcommon-dev libxkbcommon-x11-dev \
    libfontconfig1-dev libfreetype-dev \
    libusb-1.0-0-dev libmxml-dev libv4l-dev liblo-dev zlib1g-dev \
    libopencv-dev libcwiid-dev \
    libudev-dev libdbus-1-dev \
    gcc-multilib g++-multilib libc6-dev-i386 \
    gcc-mingw-w64-i686 g++-mingw-w64-i686 mingw-w64-tools \
    gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 \
    && rm -rf /var/lib/apt/lists/*

# Modern appimagetool + static type2-runtime (FUSE3-linked; no host libfuse2 on target systems).
# continuous tags move; audit with: appimagetool --version && runtime-x86_64 --appimage-version
RUN cd /tmp && \
    wget -q "https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage" \
         -O appimagetool.AppImage && \
    chmod +x appimagetool.AppImage && \
    ./appimagetool.AppImage --appimage-extract && \
    mv squashfs-root /opt/appimagetool && \
    ln -s /opt/appimagetool/AppRun /usr/local/bin/appimagetool && \
    rm appimagetool.AppImage && \
    mkdir -p /opt/appimage-runtime && \
    wget -q "https://github.com/AppImage/type2-runtime/releases/download/continuous/runtime-x86_64" \
         -O /opt/appimage-runtime/runtime-x86_64 && \
    chmod +x /opt/appimage-runtime/runtime-x86_64

ENV APPIMAGE_RUNTIME=/opt/appimage-runtime/runtime-x86_64

# X-Plane SDK (headers only). Official archives: https://developer.x-plane.com/sdk/
# Default URL is a public mirror with the same layout as the official SDK (CHeaders/XPLM/...).
ARG WITH_XPLANE_SDK=1
ARG XPLANE_SDK_GIT_URL=https://github.com/Coalition-of-Freeware-Developers/X-Plane12-SDK.git
RUN if [ "$WITH_XPLANE_SDK" = "1" ]; then \
        git clone --depth 1 "$XPLANE_SDK_GIT_URL" /opt/xplane-sdk && \
        test -f /opt/xplane-sdk/CHeaders/XPLM/XPLMPlugin.h; \
    fi

# CMake and prepare.sh expect this path (directory that contains XPLM/ and Widgets/).
ENV XPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

WORKDIR /src
