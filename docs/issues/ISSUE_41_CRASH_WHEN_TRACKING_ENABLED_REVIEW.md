# Issue #41 – Crash to desktop when tracking enabled (review)

**GitLab:** https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/41  
**Summary:** AppImage segfaults when user clicks Start (trackir/smartnav). Fedora 43, Wayland, Plasma.

## What the logs show

1. **Terminal:** Last line before crash: `Will try to connect to socket '/tmp/ltr_m_sock'` then `Segmentation fault (core dumped)`.
2. **Device log (logfile viewer):** TrackIR 131D:0158 found → "Opening handle to the device found." → **"Error opening device!"** → Close/exit.

So two things are going on:

- **Segfault:** In the process that prints "Will try to connect to socket" (the GUI, via `ltr_init()` → `ltr_int_connect_to_socket()` in `src/ipc_utils.c`). The crash happens in that same process right after that message (either inside `ltr_int_connect_to_socket` or immediately after it returns).
- **Device open failure:** A separate code path (TrackIR backend in `src/libusb_ifc.c`) reports "Error opening device!" — usually udev/permissions (user not in the right group, or rules not applied). That does not by itself cause the segfault but explains why the device never opens.

## Code path when you click Start

1. **GUI:** `on_StartLtrPipeButton_pressed()` (e.g. `ltr_gui.cpp`) → starts `ltr_pipe` detached → calls `TRACKER.start(sec)`.
2. **TRACKER.start()** ends up in the C API and calls **`ltr_init()`** in `src/ltlib.c`.
3. **`ltr_init()`** checks if the master server is up by calling **`ltr_int_connect_to_socket("/tmp/ltr_m_sock")** in `src/ipc_utils.c`.
4. That function does:
   - `printf("Will try to connect to socket '%s'\n", name);`
   - `socket(PF_UNIX, SOCK_STREAM, 0)`
   - `ioctl(..., FIONBIO, ...)`
   - `memset` / `strncpy` of `struct sockaddr_un`
   - `connect(socket_fd, ...)`

So the segfault is either inside that sequence (e.g. `socket`, `ioctl`, `memset`, `strncpy`, `connect`) or in the caller right after return. Without a backtrace we can’t pin it to a single line.

## Resolution

The reporter confirmed the issue was due to **missing udev rules** for the TrackIR device. Installing the udev rules (and ensuring the user is in the correct group, and optionally rebooting) resolved the problem. The repo rule in `src/99-TIR.rules` (vendor 131d, MODE 0666) correctly covers 131D:0158.

## Fixes applied in tree

1. **Socket fd leak:** When `connect()` failed, the code returned `-1` without closing `socket_fd`. That leaks a file descriptor on every failed connect. Fixed by closing `socket_fd` before returning on connect failure in `ltr_int_connect_to_socket()`.
2. **Debug printf:** The `printf("Will try to connect to socket ...")` was replaced with `ltr_int_log_message(...)` so:
   - Log goes to the normal LinuxTrack log (not raw stdout), and
   - The crash location is not misread as “right after a random printf”.

3. **Udev rules detection:** The permission dialog and install logic now check both `/lib/udev/rules.d/99-TIR.rules` and `/usr/lib/udev/rules.d/99-TIR.rules`, so Fedora and other FHS distros correctly report "rules installed" when the rule is in either location. When installing, the code prefers the directory that exists (e.g. `/usr/lib/udev/rules.d` on Fedora).
4. **Graceful handling when Start fails:** If the user clicks Start and tracking fails to initialize (e.g. `linuxtrack_get_tracking_state()` returns an error such as `err_NOT_INITIALIZED`), the GUI now shows the TrackIR permission dialog after a short delay (if the user has not chosen "don't show again"), guiding them to install udev rules instead of crashing or failing silently.

## What to ask the reporter for

1. **Backtrace** (exact crash location):
   - Run the AppImage under GDB, or
   - After crash run e.g. `coredumpctl list` then `coredumpctl info` / `coredumpctl debug` and get a backtrace. That will show whether the crash is in `socket`, `ioctl`, `connect`, or in the caller (e.g. `ltlib.c` after `ltr_int_connect_to_socket` returns).
2. **Device permissions:** The device log shows "Error opening device!" (TrackIR 131D:0158). They should:
   - Confirm udev rules for TrackIR are installed and that the user is in the group that can access the device (e.g. `plugdev` or the group used by those rules).
   - Even if the segfault is fixed, tracking won’t work until the device can be opened.

## Possible follow-ups (after backtrace)

- If the crash is in **`connect()`** or **`socket()`**: consider environment (e.g. Wayland/sandbox, seccomp) and whether to add a short retry/backoff when the master isn’t ready yet (GUI starts `ltr_pipe` then immediately calls `TRACKER.start()`; the master might not have created `/tmp/ltr_m_sock` yet).
- If the crash is **after** `ltr_int_connect_to_socket` returns (e.g. in `ltlib.c`): inspect use of the return value and any shared state (e.g. `mmm`, `com`) for NULL or use-after-free when running in client vs standalone mode.
- **Portability:** `connect()` currently uses `sizeof(struct sockaddr_un)`. For portability, the length can be set with `offsetof(struct sockaddr_un, sun_path) + strlen(address.sun_path) + 1`; worth doing if we see odd behavior on other OSes or with different libc/kernel versions.

## Files touched

- `src/ipc_utils.c`: in `ltr_int_connect_to_socket()` — close socket on connect failure; replace `printf` with `ltr_int_log_message`.
- `src/qt_gui/trackir_permission_dialog.cpp`: udev rules considered installed if either `/lib/udev/rules.d/99-TIR.rules` or `/usr/lib/udev/rules.d/99-TIR.rules` exists; install targets the directory that exists (prefer `/usr/lib/udev/rules.d` when present).
- `src/qt_gui/ltr_gui.cpp`: after Start, 1s delayed check also reads tracking state and shows TrackIR permission dialog when state is an error and the user has not dismissed the dialog.
