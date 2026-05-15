# X-Plane SDK (build dependency)

LinuxTrack’s X-Plane plugin needs the **X-Plane SDK headers** at configure time. End users of a prebuilt AppImage do not need the SDK.

## Download

1. Open [X-Plane plugin SDK downloads](https://developer.x-plane.com/sdk/plugin-sdk-downloads/).
2. Download the current **ZIP** archive (for example `XPSDK430.zip`). The version number in the filename changes with SDK releases.

Older docs referred to `.tar.gz` archives; current official downloads are **ZIP only**.

## Install layout

The zip contains a top-level `SDK/` directory:

```
SDK/
  CHeaders/
    XPLM/
    Widgets/
  ...
```

CMake expects headers at **`/opt/xplane-sdk/CHeaders`** (or set `-DXPLANE_SDK_PATH=` to the directory that contains `XPLM/` and `Widgets/`).

## Extract (all distributions)

Install `unzip` if needed, then:

```bash
sudo mkdir -p /opt/xplane-sdk
tmpdir=$(mktemp -d)
unzip -q /path/to/XPSDK*.zip -d "$tmpdir"
sudo cp -a "$tmpdir"/SDK/. /opt/xplane-sdk/
rm -rf "$tmpdir"

test -f /opt/xplane-sdk/CHeaders/XPLM/XPLMPlugin.h && echo "X-Plane SDK OK"
```

Do **not** unzip directly into `/opt/xplane-sdk` without copying `SDK/` contents — you would get `/opt/xplane-sdk/SDK/CHeaders` and CMake would not find the headers.

## Build with LinuxTrack

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt \
  -DENABLE_XPLANE=ON \
  -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
```

Distribution guides: [Arch](arch-linux.md), [Debian/Ubuntu](debian-ubuntu.md), [Fedora/RHEL](fedora-rhel.md).

## Plugins shipped

- **xlinuxtrack.so** — legacy X-Plane 8.x / 9.x
- **xlinuxtrack9.so** — X-Plane 9.x and newer

After install, enable the plugin in X-Plane’s plugin manager for head tracking.
