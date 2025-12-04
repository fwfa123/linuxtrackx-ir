# Migration Guide: Upgrading from /usr/local to /opt

This guide helps you migrate your LinuxTrack installation from `/usr/local` to `/opt` prefix.

## Why Migrate?

The `/opt` prefix is now the default and recommended installation location because:

- **Steam Proton Compatibility**: `/opt` is accessible in Steam Proton's pressure-vessel containers, while `/usr/local` is not
- **Better Isolation**: `/opt` is designed for third-party software, reducing conflicts with system packages
- **Container Support**: Works better with containerized gaming environments (Steam, Lutris)

## Prerequisites

- Existing LinuxTrack installation in `/usr/local`
- Root/sudo access
- Backup of your configuration (optional but recommended)

## Migration Steps

### Step 1: Backup Your Configuration (Optional)

```bash
# Backup your configuration file
cp ~/.config/linuxtrack/linuxtrack1.conf ~/.config/linuxtrack/linuxtrack1.conf.backup
```

### Step 2: Stop LinuxTrack

```bash
# Stop the daemon if running
pkill ltr_server1
pkill ltr_gui
```

### Step 3: Clean Up Old Installation

You have two options:

#### Option A: Use the Cleanup Script (Recommended)

```bash
cd /path/to/linuxtrackx-ir
sudo ./scripts/install/cleanup_usr_local.sh
```

The script will:
- List all files that will be removed
- Ask for confirmation
- Remove LinuxTrack files from `/usr/local`
- Clean up desktop entries and icon cache

#### Option B: Manual Cleanup

```bash
# Remove binaries
sudo rm -f /usr/local/bin/ltr_gui
sudo rm -f /usr/local/bin/ltr_server1
sudo rm -f /usr/local/bin/ltr_pipe
sudo rm -f /usr/local/bin/ltr_extractor
sudo rm -f /usr/local/bin/ltr_recenter
sudo rm -f /usr/local/bin/mickey

# Remove libraries
sudo rm -rf /usr/local/lib/linuxtrack/

# Remove data files
sudo rm -rf /usr/local/share/linuxtrack/

# Remove desktop entries
sudo rm -f /usr/local/share/applications/linuxtrack*.desktop

# Remove icons
sudo rm -f /usr/local/share/icons/hicolor/*/apps/linuxtrack*

# Remove ld.so.conf entry if it points to /usr/local
if [ -f /etc/ld.so.conf.d/linuxtrack.conf ]; then
    if grep -q "/usr/local/lib/linuxtrack" /etc/ld.so.conf.d/linuxtrack.conf; then
        sudo rm -f /etc/ld.so.conf.d/linuxtrack.conf
        sudo ldconfig
    fi
fi

# Update desktop database
sudo update-desktop-database /usr/local/share/applications 2>/dev/null || true
sudo gtk-update-icon-cache -f -t /usr/local/share/icons/hicolor 2>/dev/null || true
```

### Step 4: Delete Old Configuration File

**Important**: The configuration file stores the old prefix. You must delete it so LinuxTrack can detect the new installation location.

```bash
# Delete the old configuration file
rm ~/.config/linuxtrack/linuxtrack1.conf
```

> **Note**: This will reset your LinuxTrack settings. If you want to preserve settings, you can edit the file and change the `PREFIX` value from `/usr/local` to `/opt` instead of deleting it.

### Step 5: Rebuild and Install to /opt

```bash
cd /path/to/linuxtrackx-ir

# Clean previous build (if any)
rm -rf build/
mkdir build && cd build

# Configure with /opt prefix (this is now the default)
cmake .. -DCMAKE_INSTALL_PREFIX=/opt [your other options]

# Build
cmake --build . -j$(nproc)

# Install
sudo cmake --install .
```

### Step 6: Verify Installation

```bash
# Check that binaries are installed
ls -la /opt/bin/ltr_*

# Check that libraries are installed
ls -la /opt/lib/linuxtrack/

# Check that symlinks were created (for PATH compatibility)
ls -la /usr/local/bin/ltr_*

# Verify installation
./scripts/install/verify_installation.sh
```

### Step 7: Reinstall Wine Bridge

If you use Wine/Proton, you'll need to reinstall the Wine bridge in all your Wine prefixes:

```bash
# For Steam games, use the GUI to reinstall Wine bridge
ltr_gui

# Or manually for each prefix:
# Find your Wine prefixes
find ~/.steam/steam/steamapps/compatdata -name "pfx" -type d

# For each prefix, run the installer
# (Use the GUI method for easier management)
```

### Step 8: Test

```bash
# Start LinuxTrack GUI
ltr_gui

# Verify daemon is running
pgrep -f ltr_server1

# Test with a game
```

## What Changed?

### Installation Location

- **Old**: `/usr/local/bin`, `/usr/local/lib/linuxtrack/`, `/usr/local/share/linuxtrack/`
- **New**: `/opt/bin`, `/opt/lib/linuxtrack/`, `/opt/share/linuxtrack/`

### PATH Compatibility

Symlinks are automatically created in `/usr/local/bin` pointing to `/opt/bin`, so commands like `ltr_gui` will still work even if `/opt/bin` is not in your PATH.

### Configuration File

The configuration file at `~/.config/linuxtrack/linuxtrack1.conf` stores the install prefix. After migration, it will be recreated with the new prefix.

## Troubleshooting

### Binaries Not Found

If `ltr_gui` or other commands are not found:

1. Check if symlinks exist:
   ```bash
   ls -la /usr/local/bin/ltr_*
   ```

2. If symlinks don't exist, they should have been created during installation. Check installation logs.

3. As a workaround, you can use full paths:
   ```bash
   /opt/bin/ltr_gui
   ```

### Library Not Found

If libraries are not found:

1. Check ldconfig:
   ```bash
   sudo ldconfig
   ldconfig -p | grep linuxtrack
   ```

2. Verify `/etc/ld.so.conf.d/linuxtrack.conf` points to `/opt/lib/linuxtrack`:
   ```bash
   cat /etc/ld.so.conf.d/linuxtrack.conf
   ```

### Wine Bridge Not Working

1. Make sure you deleted the old configuration file (Step 4)
2. Reinstall the Wine bridge using the GUI
3. Check that libraries are accessible:
   ```bash
   ls /opt/lib/linuxtrack/liblinuxtrack*.so.0
   ```

### Desktop Entry Not Found

1. Update desktop database:
   ```bash
   sudo update-desktop-database /opt/share/applications
   ```

2. Some desktop environments may not search `/opt/share/applications` by default. You may need to:
   - Use the application menu search
   - Create a launcher manually
   - Or use the command line: `ltr_gui`

## Rollback

If you need to rollback to `/usr/local`:

1. Uninstall from `/opt`:
   ```bash
   cd build
   sudo cmake --build . --target uninstall
   ```

2. Remove symlinks:
   ```bash
   sudo rm -f /usr/local/bin/ltr_*
   ```

3. Rebuild with `/usr/local` prefix:
   ```bash
   cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local [your options]
   cmake --build . -j$(nproc)
   sudo cmake --install .
   ```

## Additional Notes

- The cleanup script (`cleanup_usr_local.sh`) is safe to run multiple times
- Your Wine prefixes and game configurations are not affected by this migration
- The migration only affects the LinuxTrack installation location, not your tracking profiles or game settings

## Getting Help

If you encounter issues during migration:

1. Check the [troubleshooting guide](troubleshooting.md)
2. Review installation logs
3. Verify file permissions
4. Check system logs for errors

For additional support, see the main [README](../README.md) or open an issue on GitLab.
