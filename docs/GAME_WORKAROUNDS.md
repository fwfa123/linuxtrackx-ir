# Game-specific hints and workarounds

Short, community-tested notes for individual titles under Wine/Proton/Lutris.
These are not general install steps — see the [README](../README.md) and
[troubleshooting](troubleshooting/README.md) guides for that.

**Contributing:** If you confirm a workaround on a current build, add a section
below (game name, platform, prefix type, what to avoid, what works).

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
