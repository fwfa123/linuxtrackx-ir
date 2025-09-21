# Wine Games Support

To allow Windows programs running under Wine to use headtracking, Linuxtrack X-IR provides Linuxtrack-Wine installer.
This installer provides necessary interfaces to channel headtracking data to both TrackIR enabled applications (both native like Condor Soaring, or via TIRViews.dll like MSFS series) and FreeTrack compatible applications.
Be aware, that Linuxtrack-Wine requires TrackIR firmware with companion files to be available (even if you aren't using the TrackIR for tracking).

Please note, that it is necessary to install TrackIR firmware (along with mfc42u.dll needed by TIRViews.dll) to utilize all of the above options. The easiest way to do that is to use the **Install Linuxtrack-Wine support...** button found on the Misc pane of the ltr\_gui. That way, all the necessary components will be installed/extracted in one go.

On Linux, a dialogue asking for the target "Wine prefix" (or "bottle") pops up after you install the prerequisites - the Linuxtrack-Wine will be installed to the prefix/bottle of your choice.

On Mac OS X, the linuxtrack-wine.exe installer (located in the Linuxtrack .dmg package) must be
installed manually - just run the installer in the desired "bottle".

Please be aware that linuxtrack-wine.exe must be installed into each and every wine bottle/prefix in which you require the headtracking.

If you installed Linuxtrack-Wine prior to version 0.99.11, to gain access to the new features you need to uninstall it first, reinstall TrackIR firmware (to get TIRViews.dll and mfc42u.dll) and then install the new linuxtrack-wine.exe.

## Modern Installation Methods

LinuxTrack X-IR now uses modern installation methods for better compatibility with current distributions:

### Method 1: Winetricks Installation (Recommended for Debian/Ubuntu/MX)

The most reliable approach for Debian-based systems is to use winetricks:
`WINEPREFIX=~/.wine winetricks mfc42`

### Method 2: Package Manager Installation (Other Distributions)

Some distributions have mfc42 packages available:

- **Fedora/RHEL/CentOS:** `sudo dnf install mfc42`- **Arch Linux/Manjaro:** `sudo pacman -S mfc42`- **OpenSUSE:** `sudo zypper install mfc42`

**Note:** Ubuntu/Debian/MX Linux do not have MFC42 packages in their repositories and should use winetricks instead.

### Method 3: Manual Installation

As a last resort, manually copy mfc42u.dll from a Windows system to the LinuxTrack firmware directory.

The installation process will automatically try these methods in order, providing clear guidance if manual intervention is required.

## Winegcc Build System (Updated)

LinuxTrack X-IR uses winegcc for generating Wine-compatible binaries. This approach provides several advantages:

### Benefits of Winegcc Approach

- **Perfect Wine Integration:** .exe.so and .dll.so files work seamlessly with Wine

- **Symlink Support:** Native Unix symlinks work properly

- **Standard Dependencies:** Uses standard wine-devel packages

- **Future-Proof:** Wine-native approach for long-term compatibility

- **Simplified CI/CD:** Standard Wine environment for automated builds

### Built Components

The build system generates the following Wine-compatible components:

- **NPClient.dll.so** - TrackIR API compatibility (32-bit)

- **NPClient64.dll.so** - TrackIR API compatibility (64-bit)

- **FreeTrackClient.dll.so** - FreeTrack API compatibility

- **Controller.exe.so** - Hotkey control utility

- **Tester.exe.so** - TrackIR testing utility

- **Tester64.exe.so** - 64-bit TrackIR testing utility

- **ftc.exe.so** - FreeTrack testing utility

- **TrackIR.exe.so** - Fake TrackIR executable for games

## Linuxtrack-wine usage

When you installed the Linuxtrack-Wine to the bottle/prefix of your choice, there are several programs at your disposal.

**Controller** allows you to bind hotkeys to pause/resume and recenter the tracking.
It can be minimized to the tray so it doesn't clutter the desktop.
When a **Controller** is started, window pops up and you should specify the pause key and recenter key -
just press the appropriate button followed by key/keys of your choice (max 2 keys).
One thing to be aware of is, that the **Controller** works (meaning it sees the hotkey pressed) only inside the bottle/prefix where you installed it!

**TrackIR** is a fake TrackIR executable, that is required by certain games in order to accept the tracking data. It also loads TIRViews library, that channels the headtracking data into following games:

- Battlefield 2- Wings of War- Nascar Racing Season 2003- Collin McRae Rally 04- ToCA Race Driver 2- Richard Burns Rally- F1 Challenge- MSFS 2004, FSX- CFS3- Prepar3D

**FreeTrackTester**, **Tester** (and on 64bit Wine also **Tester64**) allow you to troubleshoot the Linuxtrack-Wine installation. They are FreeTrack and TrackIR client applications and if they are working, the installation is OK.

Linuxtrack-Wine can also be uninstalled if necessary using the **Uninstall**.

## Supported games

Technically speaking, any FreeTrack and/or TrackIR enabled game/application should work. Directly tested (meaning I tested them) were the following:

- Microsoft Flight Simulator (2020, X)- DCS World- IL-2 Sturmovik series- War Thunder- Elite Dangerous- Euro Truck Simulator 2- American Truck Simulator- Falcon AF- Condor Soaring simulator- MSFS 2004

If you know of others (working or not; assuming the game itself work under wine), please let me know, so I can either assist you in troubleshooting or update the list.

## Note on profiles

When the game/application is started for the first time, a new profile is created automatically by copying the default one.
When adjusting the tracking in the ltr\_gui, be sure to select the correct profile first, otherwise you won't see any change (because you are modifying the wrong profile).

The TrackIR software uses the plus/minus 500mm as a displacement limit, so to use the full displacement range, set the limits for translations accordingly (go to the **Tracking Setup** pane, select the appropriate profile, then press the **Detailed Axes Setup** button and for translations set limit to 500).

## Troubleshooting

If you encounter issues with the Wine installation:

### Common Issues

- **Build Errors:** Ensure wine-devel packages are installed

- **NSIS Errors:** The build system now properly handles .exe.so/.dll.so file extensions

- **C++ Linking Errors:** Fixed with -lstdc++ flags in winegcc commands

- **Missing Files:** Verify all dependencies are installed (see README for details)

### Verification Steps

1. Check that winegcc and wineg++ are available: `which winegcc wineg++`
2. Verify Wine development tools: `winegcc --version`
3. Test the build process: `make distclean && ./configure --prefix=/opt && make`
4. Check generated files: Look for .exe.so and .dll.so files in build directories

