# LinuxTrack Wine Bridge Setup Guide

## Overview
The LinuxTrack Wine Bridge provides head tracking support for Windows applications running under Wine. It includes TrackIR and FreeTrack compatible interfaces that allow Windows games and applications to receive head tracking data from LinuxTrack.

## Prerequisites

### Required Software
- **Wine**: Windows compatibility layer
- **LinuxTrack**: Main head tracking system (this package)
- **TrackIR Firmware**: Required for TrackIR compatibility

### Installation Commands by Distribution

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install wine wine32 wine64
```

#### Fedora
```bash
sudo dnf install wine wine-core wine-desktop
```

#### Arch Linux
```bash
sudo pacman -S wine wine-mono wine-gecko
```

#### OpenSUSE
```bash
sudo zypper install wine wine-32bit
```

## Installation

### Automatic Installation
1. Run the LinuxTrack Wine Bridge launcher:
   ```bash
   linuxtrack-wine-launcher.sh
   ```
2. Follow the prompts to select your Wine prefix
3. The installer will automatically set up all required components

### Manual Installation
1. Navigate to the wine bridge directory:
   ```bash
   cd /usr/share/linuxtrack/wine/
   ```
2. Run the installer in your desired Wine prefix:
   ```bash
   WINEPREFIX=~/.wine wine linuxtrack-wine.exe
   ```

## Components Installed

### TrackIR Compatibility
- **NPClient.dll**: TrackIR client library (32-bit)
- **NPClient64.dll**: TrackIR client library (64-bit)
- **TrackIR.exe**: TrackIR views component for specific games

### FreeTrack Compatibility
- **FreeTrackClient.dll**: FreeTrack client library
- **ftc.exe**: FreeTrack testing application

### Utilities
- **Controller.exe**: Hotkey controller for pause/resume/recenter
- **Tester.exe**: TrackIR protocol testing (32-bit)
- **Tester64.exe**: TrackIR protocol testing (64-bit)
- **check_data.exe**: Data verification utility

## Usage

### Testing Installation
1. Run the tester applications to verify installation:
   ```bash
   # Test TrackIR compatibility
   wine Tester.exe
   
   # Test FreeTrack compatibility
   wine ftc.exe
   ```

### Using the Controller
1. Launch the controller:
   ```bash
   wine Controller.exe
   ```
2. Configure hotkeys for:
   - **Pause/Resume**: Temporarily stop tracking
   - **Recenter**: Reset tracking center position

### Game Setup
1. Ensure LinuxTrack is running and configured
2. Start your Windows game/application under Wine
3. The game should automatically detect TrackIR/FreeTrack support
4. Configure tracking settings in the game as needed

## Supported Games

### Tested and Working
- **Flight Simulators**: MSFS 2004, FSX, Prepar3D
- **Racing Games**: Richard Burns Rally, F1 Challenge, ToCA Race Driver 2
- **Combat Sims**: IL-2 Sturmovik, Falcon AF
- **Other**: Battlefield 2, Wings of War, Condor Soaring

### General Compatibility
Any game that supports TrackIR or FreeTrack should work. The wine bridge provides the same interfaces that these games expect.

## Troubleshooting

### Common Issues

#### "TrackIR not detected"
- Ensure TrackIR firmware is installed in the Wine prefix
- Verify LinuxTrack is running and tracking
- Check that the correct Wine prefix is being used

#### "No tracking data"
- Confirm LinuxTrack is actively tracking
- Check that the correct profile is selected in LinuxTrack
- Verify the game is using the correct tracking interface

#### "Controller not responding"
- Ensure the controller is running in the same Wine prefix as the game
- Check that hotkeys are properly configured
- Verify the controller window is active when pressing hotkeys

### Debug Steps
1. Run the tester applications to verify basic functionality
2. Check LinuxTrack logs for tracking data
3. Verify Wine prefix configuration
4. Test with a known working game

## Advanced Configuration

### Multiple Wine Prefixes
Install the wine bridge in each prefix where you need head tracking:
```bash
WINEPREFIX=~/.wine-gaming wine linuxtrack-wine.exe
WINEPREFIX=~/.wine-work wine linuxtrack-wine.exe
```

### Profile Management
- Each Wine prefix uses its own tracking profile
- Profiles are automatically created on first use
- Configure tracking settings in LinuxTrack for each profile

### Performance Optimization
- Use 32-bit Wine for better compatibility
- Ensure adequate USB bandwidth for tracking devices
- Close unnecessary applications to reduce system load

## Support

For additional help:
- Check the main LinuxTrack documentation
- Visit the LinuxTrack website: http://linuxtrack.eu
- Review the wine bridge README.wine file
- Test with the provided tester applications

## Version Information
This wine bridge is compatible with LinuxTrack 0.99.11 and later versions. 