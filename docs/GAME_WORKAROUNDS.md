# Game-specific hints and workarounds

Short, community-tested notes for individual titles under Wine/Proton/Lutris.
These are not general install steps — see the [README](../README.md) and
[troubleshooting](troubleshooting/README.md) guides for that.

**Contributing:** If you confirm a workaround on a current build, add a section
below (game name, platform, prefix type, what to avoid, what works).

---

## Flatpak (Steam or Lutris) — Flatseal / CLI

If **Steam** or **Lutris** is installed from **Flatpak**, games run in a sandbox.
The same Wine prefix on `/media` may track with native Steam/Lutris but fail under
Flatpak until the sandbox can see LinuxTrack paths on the host.

**Flatseal (GUI):** install from Flathub (`com.github.tchx84.Flatseal`), select
**Steam** (`com.valvesoftware.Steam`) or **Lutris** (`net.lutris.Lutris`), then
under **Filesystem → Other files** add:

| Flatpak app | Path to allow (read-only is enough) |
|-------------|-------------------------------------|
| **Steam** | `/home/<username>/.config/linuxtrack` |
| **Steam** (library on external drive) | `/media`, `/run/media`, or your library mount |
| **Lutris** | `/opt` or `/usr` if LinuxTrack is installed there |

Optional **Environment** in Flatseal for Steam: `LINUXTRACK_UNIX_HOME=/home/<username>`.

**CLI (same permissions):**

```bash
# Steam — head tracking (Wine bridge / Proton)
flatpak override --user --filesystem=/home/<username>/.config/linuxtrack:ro com.valvesoftware.Steam

# Lutris — host install tree
flatpak override --user --filesystem=/opt:ro net.lutris.Lutris
```

Fully quit and restart Steam or Lutris after changing permissions.

More detail: [Steam (Flatpak) tracking and logs](troubleshooting/STEAM_FLATPAK_DEBUG_LOGS.md),
[Lutris Flatpak](readme/flatpak.md).

---

## DCS World — Proton / Wine

**Symptom:** Side or rear cockpit panels are hard to reach after turning your
head; lateral lean does not follow head orientation (GitLab #60).

**Cause (fixed in v2.0+):** The MinGW Wine bridge (`NPClient64.dll`) applied
translation alignment with a transposed rotation matrix (`Rᵀ * t`) instead of
matching the Linuxtrack server path (`R * t`). DCS on Linux always uses the Wine
bridge (there is no native Linux DCS client).

**Working setup:**

1. Install the Wine bridge into the DCS Proton/Lutris prefix (`ltr_gui` Gaming
   tab or `scripts/install/install_wine_bridge.sh`).
2. Enable head tracking in DCS (Controls → Head Tracking).
3. Use a DCS-specific profile in `ltr_gui` (auto-created on first launch).
4. Set translation axis limits to **±500 mm** in Detailed Axes Setup (TrackIR
   convention).

**If translation still feels wrong:**

- Toggle **System → Disable translation/rotation alignment** and compare
  side-panel reach with the head turned ~45°.
- Reinstall `NPClient64.dll` after updating Linuxtrack (WOW64/Proton prefixes).

---

## Arma 2 (`arma2.exe`) — Wine / Lutris

**Symptom:** Game crashes during startup when the Linuxtrack Wine bridge is
installed (often before the main menu). Emptying the whole `Linuxtrack` folder
lets the game run; full install crashes again.

**Cause (confirmed):** `FreeTrackClient.dll` in the bridge install directory.
Arma loads it via the Freetrack registry path
(`HKCU\Software\Freetrack\FreetrackClient`) and faults. `NPClient.dll` alone does
not reproduce the startup crash.

**Working setup (32-bit and WOW64 prefixes):**

1. Install the bridge as usual (`ltr_gui` Gaming tab → Lutris/Steam/custom prefix, or `scripts/install/install_wine_bridge.sh`) into the
   prefix).
2. **Remove** from the prefix `Linuxtrack` folder (do not deploy):
   - `FreeTrackClient.dll`
   - `FreeTrackTester.exe` / `ftc.exe` (optional; not required for Arma 2)
3. **Keep:** `NPClient.dll`, firmware symlinks (`TIRViews.dll`, `mfc42u.dll`),
   and other NPClient/support files as needed.
4. Registry may still list both NaturalPoint and Freetrack paths pointing at
   `C:\Program Files (x86)\Linuxtrack\` or `C:\Program Files\Linuxtrack\` — that
   is fine as long as `FreeTrackClient.dll` is **not** on disk there.
5. In `Documents\Arma 2\*.ArmA2Profile` (example):
   ```text
   trackIR=1;
   freeTrack=0;
   ```
6. Start tracking in `ltr_gui` (ArmA 2 profile) before launching the game.
   Optional env: `LINUXTRACK_UNIX_HOME`, `LINUXTRACK_DBG=w`.

**Do not use:** “FreeTrack only” in the in-game controller menu with
`FreeTrackClient.dll` present — that was a red herring for this Wine stack.

**Diagnostics:** `/tmp/linuxtrack_npclient.log` (NPClient path only).

**Verified:** Lutris `arma-2` (32-bit) and `arma-2-wow64` with GE-Proton after
removing `FreeTrackClient.dll`; head tracking in-game works with NPClient.

---

## Template (copy for new games)

### Game title — platform

**Symptom:** …

**Workaround:** …

**Verified:** …
