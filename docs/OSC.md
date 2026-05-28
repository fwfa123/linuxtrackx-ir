# Open Sound Control (OSC)

This document describes how LinuxTrack X-IR exposes head tracking data over **OSC** (UDP, via **liblo**). It is suitable to copy into the project wiki or to send to users who want to integrate with Pure Data, SuperCollider, Max, custom scripts, etc.

**Upstream context:** The original feature request for OSC output is [uglyDwarf/linuxtrack#80](https://github.com/uglyDwarf/linuxtrack/issues/80). The message layout suggested there (e.g. per-dot paths like `dots/3/...`) is **not** what the current C implementation sends; the authoritative format is below.

---

## How OSC is supported

1. **Library**  
   OSC uses **liblo**. When `pkg-config` finds liblo, the build sets **`OSC_SUPPORT`** in `config.h` (see root `CMakeLists.txt`: `LIBLO`, `OSC_SUPPORT`).

2. **What it is *not***  
   OSC is **not** one of the **Linuxtrack Server / ltr_pipe** output formats in the GUI **Advanced** tab (uinput-abs, FlightGear, IL-2, Silent Wings, etc.). Those are separate.

3. **What it *is***  
   A small helper program **`osc_server`** ([`src/osc_server.c`](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/main/src/osc_server.c)): it uses the **liblinuxtrack** client API, reads pose (and blob points), and **sends OSC** to a UDP port on **this machine** using liblo. A **macOS** variant lives under `src/osc_server_mac/`.

4. **Build and install**  
   With liblo installed, configure with `-DENABLE_OSC=ON` (see README “Level 5”). CMake builds **`osc_server`** and installs it to **`bin`** next to `ltr_pipe` (e.g. `/opt/bin/osc_server` if prefix is `/opt`). When installing to **`/opt`**, the same post-install step that symlinks **`ltr_gui`** to **`/usr/local/bin`** also symlinks **`osc_server`**, so **`osc_server`** is on a typical **`PATH`** without editing `PATH`.

5. **GUI**  
   The Qt GUI only **reports** whether the build included OSC (`OSC_SUPPORT`) in the system info dialog. There is no separate “OSC server” panel in the GUI.

Further build notes: [`docs/technical/ADVANCED_INSTALLATION_AND_DEPENDENCIES.md`](technical/ADVANCED_INSTALLATION_AND_DEPENDENCIES.md).

---

## How to use it (typical workflow)

1. **Install/build** with liblo present and OSC enabled in CMake (distro packages: e.g. `liblo-dev`, `liblo-devel`).

2. **Ensure tracking works** in the normal way (device configured, profile OK). `osc_server` calls `linuxtrack_init(NULL)` like other clients; the tracking pipeline must be usable on the machine.

3. **Start an OSC receiver** listening on a **UDP port greater than 1024** (Pure Data, SuperCollider, `python-osc`, etc.).

4. **Run the sender**

   After **`sudo cmake --install .`** with prefix **`/opt`**, you should have **`/usr/local/bin/osc_server`** → **`/opt/bin/osc_server`** (same pattern as **`ltr_gui`**). If you installed to another prefix or skipped install, use **`/opt/bin/osc_server`** or run from **`build/src/osc_server`**.

   If you have built but not installed, run from the build directory:

   ```bash
   cd build
   ./src/osc_server 9000
   ```

   Same usage:

   ```text
   osc_server <port>
   ```

   The first argument is the **destination UDP port** on **localhost** (`lo_address_new(NULL, port)` in the source). Ports **must be &gt; 1024** (enforced in `main`).

5. **Optional:** `osc_server <port> -q` sends a **`/quit`** message to that address (for a listener that implements shutdown).

6. **Update rate:** The main loop calls `linuxtrack_wait(3333)` (microseconds) between iterations when reading pose, so the OSC side is on the order of **hundreds of Hz**, independent of the GUI “Tracking rate (FPS)” slider (that throttles the Qt GUI pose/preview path, not this standalone program unless changed).

---

## OSC message format

Messages are sent as an **OSC bundle** from `sendPose()` in `osc_server.c`:

| Path | Arguments (liblo typetag) | Meaning |
|------|---------------------------|--------|
| `/linuxtrack/pose` | `ffffff` — six floats | `pitch`, `yaw`, `roll`, `tx`, `ty`, `tz` |
| `/linuxtrack/point` | `ifff` — int + three floats (one message per blob) | `index`, `x`, `y`, `weight` |

Older docs that show paths like `/linuxtrack/head/...` do **not** match this code.

---

## One-line summary

**LinuxTrack exposes OSC through the `osc_server` program (liblo): start your OSC app on a UDP port, then run `osc_server <port>` with tracking working; you receive bundles on `/linuxtrack/pose` and `/linuxtrack/point`. The Advanced tab server formats are not OSC.**
