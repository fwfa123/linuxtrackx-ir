# LinuxTrack Qt5 Installation Fixes for Kubuntu 25.10

## Issues Found in Verification Results

Based on the verification results, the following issues need to be resolved:

### 1. Mickey Application Build Issue (FIXED)
- **Problem**: Mickey fails to build due to missing `x11extras` Qt5 module
- **Solution**: ✅ Already fixed by removing `x11extras` from `mickey.pro.in`

### 2. Missing Qt5 Development Tools
- **Problem**: `qhelpgenerator-qt5` and `qmake-qt5` not found
- **Solution**: Install missing Qt5 tools

### 3. Missing Qt Help Files
- **Problem**: `help.qhc` and `help.qch` files not generated
- **Solution**: Install Qt5 help tools and regenerate

### 4. Missing Desktop Integration Files
- **Problem**: Some desktop icons and Mickey desktop entry missing
- **Solution**: Complete desktop integration

## Step-by-Step Fix Instructions

### Step 1: Install Missing Qt5 Tools

```bash
# Install Qt5 development tools
sudo apt update
sudo apt install qttools5-dev-tools qhelpgenerator-qt5 qmake-qt5

# Alternative packages if above don't work
sudo apt install qt5-default qtbase5-dev qttools5-dev
```

### Step 2: Rebuild and Install Mickey

```bash
cd /mnt/Local_Git/git-repos/linuxtrack-clean-june14

# Clean previous build
make clean

# Reconfigure and rebuild
./configure --prefix=/opt/linuxtrack
make

# Install with Mickey support
sudo make install
```

### Step 3: Generate Missing Help Files

```bash
# Navigate to Mickey directory
cd src/mickey

# Generate help files manually if automatic generation fails
if command -v qhelpgenerator-qt5 >/dev/null 2>&1; then
    qhelpgenerator-qt5 mickey.qhp -o help.qch
    qhelpgenerator-qt5 mickey.qhcp -o help.qhc
else
    echo "Warning: qhelpgenerator-qt5 not found, creating empty help files"
    touch help.qch help.qhc
fi

# Install help files
sudo cp help.qch help.qhc /opt/linuxtrack/share/linuxtrack/
```

### Step 4: Complete Desktop Integration

```bash
# Create missing desktop entry for Mickey
sudo tee /usr/local/share/applications/mickey.desktop > /dev/null << 'EOF'
[Desktop Entry]
Version=1.0
Type=Application
Name=Mickey
Comment=LinuxTrack Mickey Application
Exec=/opt/linuxtrack/bin/mickey
Icon=linuxtrack
Categories=Utility;
EOF

# Create missing icon directories and copy icons
sudo mkdir -p /usr/local/share/icons/hicolor/48x48/apps
sudo mkdir -p /usr/local/share/icons/hicolor/scalable/apps

# Copy existing icons to new locations
sudo cp /usr/local/share/pixmaps/linuxtrack.png /usr/local/share/icons/hicolor/48x48/apps/
sudo cp /usr/local/share/pixmaps/linuxtrack.svg /usr/local/share/icons/hicolor/scalable/apps/

# Update desktop database
sudo update-desktop-database /usr/local/share/applications
sudo gtk-update-icon-cache -f -t /usr/local/share/icons/hicolor
```

### Step 5: Copy Missing Graphics Files (Optional)

```bash
# Copy optional tracking graphics if they exist in source
if [ -f "src/qt_gui/cap_1.png" ]; then
    sudo cp src/qt_gui/cap_1.png /opt/linuxtrack/share/linuxtrack/
fi
if [ -f "src/qt_gui/cap_np.png" ]; then
    sudo cp src/qt_gui/cap_np.png /opt/linuxtrack/share/linuxtrack/
fi
if [ -f "src/qt_gui/clip_1.png" ]; then
    sudo cp src/qt_gui/clip_1.png /opt/linuxtrack/share/linuxtrack/
fi
if [ -f "src/qt_gui/clip_np.png" ]; then
    sudo cp src/qt_gui/clip_np.png /opt/linuxtrack/share/linuxtrack/
fi
```

### Step 6: Fix udev Rules Permissions

```bash
# Reload udev rules with proper permissions
sudo udevadm control --reload-rules
sudo udevadm trigger

# Verify user is in plugdev group
groups $USER | grep plugdev || echo "User not in plugdev group - run: sudo usermod -a -G plugdev $USER"
```

### Step 7: Test the Installation

```bash
# Run verification script again
/opt/linuxtrack/bin/verify_installation.sh

# Test Mickey application
/opt/linuxtrack/bin/mickey --help

# Test main GUI
/opt/linuxtrack/bin/ltr_gui
```

## Expected Results After Fixes

After applying these fixes, the verification should show:

- ✅ Mickey standalone application executable
- ✅ Qt Help collection files present
- ✅ Mickey desktop entry exists
- ✅ All icon files present
- ✅ udev rules working properly

## Troubleshooting

### If Qt5 tools are still missing:
```bash
# Try alternative package names
sudo apt install qt5-qmake qt5-tools-dev
```

### If Mickey still fails to build:
```bash
# Check Qt5 installation
qmake --version
pkg-config --modversion Qt5Widgets

# Manual build attempt
cd src/mickey
qmake mickey.pro
make
```

### If help files still missing:
```bash
# Create empty help files as fallback
sudo touch /opt/linuxtrack/share/linuxtrack/help.qhc
sudo touch /opt/linuxtrack/share/linuxtrack/help.qch
```

## Success Criteria

The installation is considered successful when:
1. `ltr_gui` launches and detects devices
2. `mickey` application is executable
3. All core binaries are functional
4. Desktop integration is complete
5. udev rules are working

The main functionality (head tracking) should work even with some optional files missing. 