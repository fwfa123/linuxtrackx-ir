## Build integrity and CI plan

Goal: prevent regressions where critical install rules or runtime assets (driver libraries and data files) are omitted, and automatically validate that a fresh install can run the GUI and detect devices.

### 1) CI: staged install and artifact verification (GitLab)

- Build and stage install into an isolated prefix:
  - `autoreconf -fiv`
  - `./configure --prefix=/usr/local`
  - `make -j$(nproc)`
  - `make install DESTDIR=$CI_PROJECT_DIR/stage`

- Verify required artifacts exist under `stage`:
  - Libraries in `stage/usr/local/lib/linuxtrack/`:
    - `libwc.so.0`
    - `libtir.so.0`
    - `libjoy.so.0`
    - `libltusb1.so.0`
  - Data in `stage/usr/local/share/linuxtrack/`:
    - `linuxtrack1.conf`
    - `99-TIR.rules`, `99-Mickey.rules`
    - `haarcascade_frontalface_alt2.xml`

- Headless smoke test to ensure the GUI seeds the user config:
  - `HOME=$CI_PROJECT_DIR/home QT_QPA_PLATFORM=offscreen timeout 3 $CI_PROJECT_DIR/stage/usr/local/bin/ltr_gui || true`
  - Assert file exists: `$CI_PROJECT_DIR/home/.config/linuxtrack/linuxtrack1.conf`

Example GitLab CI job (outline):

```yaml
staged-install-verify:
  image: debian:stable
  stage: test
  script:
    - apt-get update && apt-get install -y build-essential autoconf automake libtool \
        qtbase5-dev qttools5-dev-tools libmxml-dev libusb-1.0-0-dev libx11-dev libxrandr-dev \
        bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev
    - autoreconf -fiv
    - ./configure --prefix=/usr/local
    - make -j"$(nproc)"
    - make install DESTDIR="$CI_PROJECT_DIR/stage"
    - ls -l "$CI_PROJECT_DIR/stage/usr/local/lib/linuxtrack/" | sed -n '1,200p'
    - test -f "$CI_PROJECT_DIR/stage/usr/local/lib/linuxtrack/libwc.so.0"
    - test -f "$CI_PROJECT_DIR/stage/usr/local/lib/linuxtrack/libtir.so.0"
    - test -f "$CI_PROJECT_DIR/stage/usr/local/lib/linuxtrack/libjoy.so.0"
    - test -f "$CI_PROJECT_DIR/stage/usr/local/lib/linuxtrack/libltusb1.so.0"
    - test -f "$CI_PROJECT_DIR/stage/usr/local/share/linuxtrack/linuxtrack1.conf"
    - test -f "$CI_PROJECT_DIR/stage/usr/local/share/linuxtrack/99-TIR.rules"
    - test -f "$CI_PROJECT_DIR/stage/usr/local/share/linuxtrack/99-Mickey.rules"
    - mkdir -p "$CI_PROJECT_DIR/home"
    - HOME="$CI_PROJECT_DIR/home" QT_QPA_PLATFORM=offscreen timeout 3 "$CI_PROJECT_DIR/stage/usr/local/bin/ltr_gui" || true
    - test -f "$CI_PROJECT_DIR/home/.config/linuxtrack/linuxtrack1.conf"
```

### 2) Protect main with CI + CODEOWNERS

- Protect `main` branch; require green CI and approval for merges.
- Add `CODEOWNERS` entries so build/install files need review:
  - `src/Makefile.am`
  - `configure.ac`
  - `scripts/install/**`

### 3) Centralize install rules

- Move install definitions for driver libraries and data into a single include:
  - Create `src/build/install.mk` with all `pkglib_LTLIBRARIES` and `dist_pkgdata_DATA`.
  - In `src/Makefile.am`, `include build/install.mk`.
- Reduces chances a merge drops critical targets.

### 4) Configure-time guards

- Enforce consistency via `configure.ac`:
  - If TrackIR support is enabled, but `libltusb1` would not be built/installed, fail configure with `AC_MSG_ERROR`.
  - Print a clear capability summary at the end of configure; CI fails on unexpected "no" for TrackIR/Webcam support.

### 5) Runtime dlopen smoke test (CI optional)

- Build a tiny C helper to `dlopen()` `libwc.so.0`, `libtir.so.0`, `libjoy.so.0`, `libltusb1.so.0` from the staged libdir, fail if any missing.

### 6) Lightweight static checks

- Simple CI grep checks:
  - Verify `pkglib_LTLIBRARIES` includes `libwc.la`, `libtir.la`, `libjoy.la`, `libltusb1.la`.
  - Verify `dist_pkgdata_DATA` includes `linuxtrack1.conf` and udev/cascade files.

### 7) Branch hygiene

- Rebase feature branches on latest `main` before merging; run full CI.
- Avoid direct pushes/force-pushes to `main`; use merge requests with required approvals.

### 8) GUI robustness (already implemented)

- On startup, if prefs load succeeds but `~/.config/linuxtrack/linuxtrack1.conf` is missing, create the config directory and copy defaults; then reload. Ensures first-run works even if rules/groups were pre-installed.


