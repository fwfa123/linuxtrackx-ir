# Using LinuxTrack with Lutris (Flatpak) and other Flatpak games

When Lutris is installed as a Flatpak, games launched from it run **inside the Flatpak sandbox**. The sandbox does not expose the host's `/opt` or `/usr` by default, so the game cannot see your Linuxtrack install (e.g. `/opt/lib/linuxtrack/`). You may see "All library search attempts failed" or "linuxtrack_find_library failed" in the game's output.

## Fix: Grant Lutris access to your Linuxtrack path with Flatseal

Use **Flatseal** (Flatpak permission manager) to give the Lutris Flatpak access to the host path where Linuxtrack is installed.

1. **Install Flatseal** if needed:
   - From Flathub: `flatpak install flathub com.github.tchx84.Flatseal`
   - Or install from your distro's software center (search for "Flatseal").

2. **Open Flatseal** and select **Lutris** (net.lutris.Lutris) in the list.

3. Under **Filesystem** → **Other files**, add the path where Linuxtrack is installed:
   - If you installed to `/opt`: add **/opt** (read-only is enough).
   - If you installed system-wide to `/usr`: add **/usr** (or a more specific path if your distro supports it).

4. **Restart Lutris** and launch the game again.

Ensure **ltr_server** is running on the host so the game can connect to the tracker.

## Alternative: Command line

You can apply the same permission from the terminal:

- If Linuxtrack is under `/opt`:
  ```bash
  flatpak override --user --filesystem=/opt net.lutris.Lutris
  ```
- If Linuxtrack is under `/usr`:
  ```bash
  flatpak override --user --filesystem=/usr net.lutris.Lutris
  ```

Then restart Lutris and launch the game again.

## Optional: LINUXTRACK_LIBS

You can set the game's **Environment variables** in Lutris (Configure → Game options) to point to the library paths, e.g.:

`LINUXTRACK_LIBS=/opt/lib/linuxtrack/liblinuxtrack32.so.0:/opt/lib/linuxtrack/liblinuxtrack.so.0`

Adjust paths to your install. The same filesystem access (via Flatseal or override) is still required so that path is visible inside the sandbox.
