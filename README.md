# LinuxTrack - Head Tracking for Linux

**LinuxTrack** is a head tracking solution for Linux that provides 6DOF (6 degrees of freedom) head tracking for flight simulators, games, and other applications. This repository contains the modernized Qt5 version with improved compatibility and stability.

## Features

- **TrackIR 5 Support**: Full support for TrackIR 5 devices with proper device communication
- **Multiple Input Methods**: Support for TrackIR, Wiimote, webcam, and face tracking
- **Qt5 GUI**: Modern Qt5-based configuration interface
- **Wine Integration**: Seamless integration with Windows games via Wine
- **Profile Management**: Multiple tracking profiles for different applications
- **Real-time Tracking**: Low-latency head tracking with configurable sensitivity

## Hardware Support

- **TrackIR 5**: Full native support (USB ID: 131d:0159)
- **TrackIR 4**: Compatible with legacy devices
- **Wiimote**: Nintendo Wii Remote head tracking
- **Webcam**: Face tracking via computer vision
- **PS3 Eye**: PlayStation 3 Eye camera support

## Quick Start

### Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential autotools-dev automake libtool
sudo apt-get install qtbase5-dev qttools5-dev-tools
sudo apt-get install libusb-1.0-0-dev libmxml-dev
sudo apt-get install libv4l-dev libcwiid-dev liblo-dev
sudo apt-get install bison flex pkg-config
```

**Fedora/RHEL:**
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install qt5-qtbase-devel qt5-qttools-devel
sudo dnf install libusb1-devel mxml-devel
sudo dnf install libv4l-devel cwiid-devel liblo-devel
sudo dnf install bison flex pkgconfig
```

### Build Instructions

1. **Clone the repository:**
   ```bash
   git clone https://github.com/YOUR_USERNAME/linuxtrack.git
   cd linuxtrack
   ```

2. **Generate build system:**
   ```bash
   autoreconf -fiv
   ```

3. **Configure:**
   ```bash
   ./configure --prefix=/usr/local
   ```

4. **Build:**
   ```bash
   make -j$(nproc)
   ```

5. **Install:**
   ```bash
   sudo make install
   ```

### TrackIR Device Setup

1. **Install udev rules:**
   ```bash
   sudo cp src/99-TIR.rules /lib/udev/rules.d/
   sudo udevadm control --reload-rules
   sudo udevadm trigger
   ```

2. **Add user to required groups:**
   ```bash
   sudo usermod -a -G plugdev,linuxtrack $USER
   ```

3. **Log out and log back in** for group changes to take effect.

### Running LinuxTrack

1. **Launch the GUI:**
   ```bash
   /usr/local/bin/ltr_gui
   ```

2. **Or use the convenience script:**
   ```bash
   ./run_qt5_gui.sh
   ```

**Note:** If running on Wayland, you may need to use X11 session for optimal compatibility:
```bash
# Switch to X11 session or use:
./run_qt5_gui_x11.sh
```

## Configuration

### Initial Setup

1. **Connect your TrackIR device** and ensure it's detected
2. **Launch LinuxTrack GUI**
3. **Select your input device** (TrackIR, Wiimote, etc.)
4. **Choose your model** (TrackClip, TrackClip Pro, etc.)
5. **Calibrate tracking** using the built-in tools
6. **Test tracking** with the real-time preview

### Profile Management

LinuxTrack supports multiple profiles for different applications:

- **Default Profile**: General-purpose tracking settings
- **Flight Sim Profile**: Optimized for flight simulators
- **Racing Profile**: Configured for racing games
- **Custom Profiles**: Create your own for specific needs

### Wine Integration

For Windows games running under Wine:

1. **Enable Wine bridge** in LinuxTrack settings
2. **Configure game profiles** for specific Windows applications
3. **Launch games** - LinuxTrack will automatically provide tracking data

## Troubleshooting

### Common Issues

**TrackIR Not Detected:**
- Check USB connection
- Verify udev rules are installed: `ls -la /lib/udev/rules.d/99-TIR.rules`
- Check user groups: `groups $USER`
- Restart udev: `sudo systemctl restart systemd-udevd`

**Permission Errors:**
- Ensure user is in `plugdev` and `linuxtrack` groups
- Check device permissions: `ls -la /dev/bus/usb/`
- Verify udev rules are active: `udevadm test $(udevadm info -q path -n /dev/bus/usb/001/002)`

**GUI Won't Start:**
- Check Qt5 installation: `qmake --version`
- Verify library paths: `ldd /usr/local/bin/ltr_gui`
- Try X11 mode if using Wayland

**Build Errors:**
- Install missing dependencies (see Prerequisites)
- Clean build: `make clean && make distclean`
- Regenerate build system: `autoreconf -fiv`

### Debug Mode

Enable debug logging:
```bash
export LTR_DEBUG=1
./run_qt5_gui.sh
```

Check logs:
```bash
tail -f ~/.config/linuxtrack/linuxtrack.log
```

## Development

### Build System

LinuxTrack uses GNU Autotools as the primary build system:

- `configure.ac` - Main configuration
- `Makefile.am` - Build rules
- `src/` - Source code organized by component

### Qt5 Modernization

This version has been modernized from Qt4 to Qt5:

- **Qt5 compatibility** - Updated all Qt API calls
- **Modern build system** - Enhanced qmake integration
- **Improved stability** - Fixed library loading issues
- **Better error handling** - Enhanced diagnostic messages

### Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## System Requirements

### Minimum Requirements

- **OS**: Linux kernel 3.0+ (Ubuntu 16.04+, Fedora 25+, etc.)
- **CPU**: x86_64 or i386 architecture
- **RAM**: 256 MB available
- **Display**: X11 or Wayland (X11 preferred for hardware devices)

### Recommended Requirements

- **OS**: Modern Linux distribution (Ubuntu 20.04+, Fedora 35+)
- **CPU**: Multi-core processor for optimal performance
- **RAM**: 1 GB available
- **USB**: USB 2.0+ port for TrackIR devices

## License

LinuxTrack is released under the GNU General Public License v3.0. See `COPYING` for details.

## Credits

- **Original LinuxTrack**: uglyDwarf and contributors
- **Qt5 Modernization**: Community modernization effort
- **TrackIR Protocol**: Reverse engineering community
- **Testing**: LinuxTrack user community

## Support

- **Issues**: Report bugs and feature requests via GitHub Issues
- **Community**: Join discussions in the LinuxTrack community
- **Documentation**: Additional documentation in the `doc/` directory

## Version History

- **0.99.20**: Qt5 modernization, improved TrackIR 5 support, enhanced stability
- **Previous versions**: See `ChangeLog` for historical releases

---

**Happy Head Tracking!** 🎮✈️🏎️ 