# Testing Build Script with Revert Capability

This guide explains how to test the `build_arch_linux.sh` script while being able to revert all changes.

## Method 1: Automated Script (Recommended)

Use the provided `test_build_with_revert.sh` script:

```bash
cd /media/mario/Local_Git/git-repos/linuxtrackx-ir
./scripts/test_build_with_revert.sh
```

This will:
1. Create a snapshot of your system state
2. Run the build script
3. Track all changes
4. Provide a revert script

To revert changes:
```bash
./scripts/test_build_with_revert.sh --revert
```

## Method 2: Manual Tracking

If you prefer to track changes manually:

### Before Running Build Script

1. **Record installed packages:**
   ```bash
   pacman -Q > ~/packages_before.txt
   yay -Q > ~/aur_packages_before.txt 2>/dev/null || true
   ```

2. **Record user groups:**
   ```bash
   groups > ~/groups_before.txt
   ```

3. **Note current state:**
   - Check if `/opt/bin/ltr_gui` exists
   - Check if `/opt/xplane-sdk` exists
   - Check if `/etc/profile.d/xplane-sdk.sh` exists

### After Running Build Script

1. **Record new packages:**
   ```bash
   pacman -Q > ~/packages_after.txt
   yay -Q > ~/aur_packages_after.txt 2>/dev/null || true
   ```

2. **Find newly installed packages:**
   ```bash
   comm -13 <(sort ~/packages_before.txt) <(sort ~/packages_after.txt) | awk '{print $1}'
   ```

3. **Find newly installed AUR packages:**
   ```bash
   comm -13 <(sort ~/aur_packages_before.txt 2>/dev/null || echo "") <(sort ~/aur_packages_after.txt) | awk '{print $1}'
   ```

### To Revert Changes

1. **Remove newly installed packages:**
   ```bash
   # Review the list first, then remove
   sudo pacman -Rns <package-list>
   ```

2. **Remove AUR packages:**
   ```bash
   yay -Rns <aur-package-list>
   ```

3. **Remove installed files:**
   ```bash
   sudo rm -rf /opt/bin/ltr_gui
   sudo rm -rf /opt/lib/linuxtrack
   sudo rm -rf /opt/share/linuxtrack
   sudo rm -rf /opt/xplane-sdk
   sudo rm -f /etc/profile.d/xplane-sdk.sh
   ```

4. **Remove user from groups (if needed):**
   ```bash
   sudo gpasswd -d $USER plugdev
   sudo gpasswd -d $USER input
   ```

## Method 3: Using Build Script Options

You can also use the build script's built-in options to limit what gets installed:

```bash
# Only configure (no package installation)
./scripts/build_arch_linux.sh --configure-only

# Only build (assumes dependencies already installed)
./scripts/build_arch_linux.sh --build-only

# Only install (assumes build completed)
./scripts/build_arch_linux.sh --install-only
```

This way you can test individual steps without installing everything.

## What Gets Installed/Modified

The build script may:

1. **Install packages via pacman:**
   - base-devel, autoconf, automake, libtool
   - qt5-base, qt5-tools, qt5-x11extras
   - opencv, libusb, mxml, libx11, libxrandr
   - bison, flex
   - lib32-glibc, lib32-gcc-libs
   - v4l-utils

2. **Install AUR packages (via yay/paru):**
   - wine32
   - cwiid (optional)
   - liblo-ipv6 (optional)
   - nsis (optional)

3. **Create directories:**
   - `/opt/xplane-sdk` (if X-Plane SDK is installed)
   - `build/` (in source directory)

4. **Install to:**
   - `/opt/bin/ltr_gui`
   - `/opt/lib/linuxtrack/`
   - `/opt/share/linuxtrack/`

5. **Modify system:**
   - Add user to `plugdev` and `input` groups
   - Create `/etc/profile.d/xplane-sdk.sh` (if X-Plane SDK installed)

## Notes

- The `--needed` flag in pacman means it won't reinstall already-installed packages
- AUR packages installed via yay/paru can be removed with `yay -Rns <package>`
- User group changes require manual removal if you want to revert them
- The build directory (`build/`) can be safely deleted: `rm -rf build/`


