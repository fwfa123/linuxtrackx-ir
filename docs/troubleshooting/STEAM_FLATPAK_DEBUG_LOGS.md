# Steam (Flatpak) — head tracking and debug logs

If Steam was installed from **Flathub (Flatpak)**, Proton games run in a sandbox. Native Steam on the same machine may work while Flatpak Steam does not, even when both use the same game library on `/media` or another drive.

## Head tracking not working (Wine bridge / Proton)

### Symptom

- `ltr_gui` tracks on the host (camera / TrackIR OK).
- Host socket is up: `/tmp/ltr_m_sock` and `127.0.0.1:42371` (from `ltr_gui`).
- In-game: no head tracking; same prefix worked with **native** Steam before.

`/tmp/linuxtrack_npclient.log` may show only:

```text
Attach request
GetSignature request
Signature result: fallback used ...
```

and never `StartDataTransmission`, `GetData`, or `MinGW TCP pose connect: ok`.

### Fix (Flatseal) — required for most users

Grant Flatpak Steam read access to your LinuxTrack config (firmware, `gamedata.txt`, poem signatures):

1. Install **Flatseal** if needed: `flatpak install flathub com.github.tchx84.Flatseal`
2. Open **Flatseal** → **Steam** (`com.valvesoftware.Steam`)
3. **Filesystem** → **Other files** → add (read-only is enough):

   ```text
   /home/<username>/.config/linuxtrack
   ```

   Example: `/home/mario/.config/linuxtrack`

4. **Environment** (recommended if launch options do not apply):

   | Variable | Value |
   |----------|--------|
   | `LINUXTRACK_UNIX_HOME` | `/home/<username>` |
   | `LINUXTRACK_DBG` | `w` (optional; more NPClient logging) |

5. Fully quit and restart Steam, then test **in-game** (cockpit / mission), not only the Arma launcher.

Command-line equivalent:

```bash
flatpak override --user --filesystem=/home/<username>/.config/linuxtrack:ro com.valvesoftware.Steam
```

### Is `/opt` required in Flatseal?

**Usually no** for Flatpak Steam + Wine bridge + Proton:

- The bridge installs **NPClient** into the game’s Wine prefix (`compatdata/<APPID>/pfx/...`).
- Runtime data comes from **`~/.config/linuxtrack/tir_firmware/`** (poems, `gamedata.txt`) — that path is what Flatpak must see.

**Optional `/opt` (read-only):** only if something must read the system install tree (e.g. `LINUXTRACK_LIBS` pointing at `/opt/lib/linuxtrack/`, or “library not found” errors referencing `/opt`). Proton’s pressure-vessel often already exposes `/opt`; many users never need a Flatseal `/opt` override for Steam.

**Lutris (Flatpak)** is different: games may load `liblinuxtrack` directly from `/opt` — see [Flatpak (Lutris)](../readme/flatpak.md).

### Other Flatseal notes

| Permission | Role |
|------------|------|
| **`~/.config/linuxtrack`** | **Required** for TrackIR firmware / gamedata inside Proton |
| **`/media`, `/run/media`, `/mnt`** | Steam library on external drives |
| **`/tmp`** | Host `/tmp/ltr_m_sock`; optional if TCP `127.0.0.1:42371` works |
| **`/opt`** | Optional; not the usual fix for Wine-bridge tracking |
| **Sockets (D-Bus, etc.)** | Not used by LinuxTrack; no `ltr_m_sock` checkbox |
| **Devices → all** | USB TrackIR for **host** `ltr_gui`, not the in-game DLL |

### Wrong prefix?

Flatpak Steam may use a library only under `/media/...` with **no** `compatdata` under `~/.var/app/...`. Confirm the active prefix while the game runs:

```bash
ls -ld /path/to/steamapps/compatdata/<APPID>
pgrep -af 'arma3|Arma3' | grep -v launcher   # example: real game, not launcher only
```

Install the Wine bridge from `ltr_gui` → **Steam (Proton)** for that game after Flatpak Steam is set up.

### Success looks like

In `/tmp/linuxtrack_npclient.log` during gameplay:

```text
Signature result: OK
RegisterProgramProfileID request: ...
StartDataTransmission request
MinGW TCP pose connect: ok ...
GetData request
```

---

## Collecting debug logs

Logs do **not** go to the system `/tmp` folder unless you add extra permissions. Use the steps below instead.

### 1. Launch options (recommended)

In Steam: **right-click the game → Properties → Launch Options**, paste:

```text
PROTON_LOG=1 PROTON_LOG_DIR=$HOME/proton-logs %command%
```

Launch the game once, reproduce the issue, then quit.

**On your PC, open this folder:**

```text
~/.var/app/com.valvesoftware.Steam/data/Steam/proton-logs/
```

You should see a file named like `steam-<appid>.log`. Attach that file to your issue.

*For Windows/Proton games only. Native Linux games may not create this log.*

### 2. Steam client logs

If launch options are not enough:

```text
~/.var/app/com.valvesoftware.Steam/data/Steam/logs/
```

Zip the newest files from that folder and attach them.

### 3. NPClient logs (Proton)

Always-on (if NPClient loads):

```text
/tmp/linuxtrack_npclient.log
```

With `LINUXTRACK_DBG=w` (Flatseal Environment or launch options), also check under the prefix or game directory:

```text
.../compatdata/<APPID>/pfx/NPClient.log
.../compatdata/<APPID>/pfx/drive_c/.../Arma 3/NPClient.log   # game CWD may vary
```

Replace `<APPID>` with the game’s Steam app ID.

### Why not host `/tmp` for Proton logs?

Flatpak Steam runs in a sandbox. `/tmp` inside Steam is **not** the same as `/tmp` on your desktop unless you grant it.

To use the real system `/tmp` (optional):

```bash
flatpak override --user --filesystem=/tmp com.valvesoftware.Steam
```

Then restart Steam. The `$HOME/proton-logs` method above is simpler for Proton debug output.

### Debug checklist

1. Flatseal: allow `/home/<user>/.config/linuxtrack` (tracking)
2. Set launch options: `PROTON_LOG=1 PROTON_LOG_DIR=$HOME/proton-logs %command%` (if asked for logs)
3. Run the game and reproduce the problem
4. Attach `steam-*.log`, `/tmp/linuxtrack_npclient.log`, and Steam `logs/` if requested
