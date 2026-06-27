## Minor release — v2.1.0

> **Issues & support:** [gitlab.com/fwfa123/linuxtrackx-ir](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) (primary). This GitHub repo is a mirror—**do not use bare `#NN` issue numbers** here; they may link to the wrong upstream project. Use [GitLab issues](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) instead.

**Canonical release notes:** [GitLab v2.1.0](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.1.0)

### Highlights

- **Detailed Axes Setup** — live **Pose maps** (Look, Lean, Raise/Roll), resizable axis response graph, pan tuning sync.
- **Pose map visualization** — teal guide rings, live red dead-zone overlay (tracks per-axis sliders), green position dot; Measured or Filtered source.
- **Tracking Setup** — **Restore axis defaults** / **Save as axis defaults** (separate baseline from Default profile).
- **Build** — Debian/Ubuntu multilib docs for 32-bit `linuxtrack32`.

### Fixed

- Dead-zone overlay on pose maps uses normalized slider fraction (0–1).
- Wine bridge install paths for custom prefixes.

### Upgrade from v2.0.1

- Rebuild from tag `v2.1.0` for source installs.
- Replace AppImage when **`LinuxTrack-X-IR-2.1.0-x86_64.AppImage`** is uploaded to this release.

### AppImage

```bash
git checkout v2.1.0
./scripts/appimage/docker_build.sh
```

### Build from source

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
git checkout v2.1.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
```

See [`docs/readme/`](https://gitlab.com/fwfa123/linuxtrackx-ir/-/tree/v2.1.0/docs/readme) on GitLab.

### Links

- **GitLab release (canonical):** https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.1.0
- **CHANGELOG:** https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/v2.1.0/CHANGELOG.md
