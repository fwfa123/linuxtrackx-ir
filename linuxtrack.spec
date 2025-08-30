Name:           linuxtrack
Version:        0.99.24
Release:        1%{?dist}
Summary:        Head tracking software for Linux

License:        GPLv3+
URL:            https://github.com/uglyDwarf/linuxtrack
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qttools-devel
BuildRequires:  qt5-qthelp-devel
BuildRequires:  libusb1-devel
BuildRequires:  mxml-devel
BuildRequires:  opencv-devel
BuildRequires:  libv4l-devel
BuildRequires:  cwiid-devel
BuildRequires:  pkgconfig
BuildRequires:  autoconf
BuildRequires:  automake
BuildRequires:  libtool
BuildRequires:  bison
BuildRequires:  flex
BuildRequires:  gcc-c++

Requires:       qt5-qtbase
Requires:       libusb1
Requires:       mxml
Requires:       opencv
Requires:       systemd-udev
Recommends:     cwiid
Recommends:     libv4l

%description
LinuxTrack is a head tracking application for Linux that enables users to
control games and applications using natural head movements. It supports
various input devices including TrackIR, webcams with face tracking, and
Nintendo Wiimote controllers.

This package provides the main LinuxTrack application with Qt5 GUI, device
drivers, and configuration tools. It includes support for multiple tracking
technologies and offers extensive customization options for head tracking
profiles and axis mappings.

Features:
* TrackIR device support with full 6DOF tracking
* Webcam-based face tracking using OpenCV
* Nintendo Wiimote support via Bluetooth
* Comprehensive GUI for configuration and calibration
* Multiple output protocols for game integration
* Profile management and axis customization
* Real-time tracking visualization and debugging

%package devel
Summary:        Development files for LinuxTrack
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       libusb1-devel

%description devel
This package contains the header files and static libraries needed for
developing applications that use LinuxTrack head tracking functionality.

LinuxTrack provides a C API for integrating head tracking into games
and applications, supporting multiple input devices and tracking methods.

%package doc
Summary:        Documentation for LinuxTrack
BuildArch:      noarch

%description doc
This package contains comprehensive documentation for LinuxTrack head
tracking software, including user manual, setup guides, device configuration
instructions, troubleshooting documentation, and API reference.

%prep
%autosetup

%build
# Generate configure script if needed
if [ ! -f configure ]; then
    autoreconf -fiv
fi

%configure --prefix=/opt/linuxtrack \
           --enable-static=no \
           --enable-shared=yes

%make_build

%install
%make_install

# Create necessary directories
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_datadir}/doc/%{name}
mkdir -p %{buildroot}%{_sysconfdir}/ld.so.conf.d
mkdir -p %{buildroot}%{_udevrulesdir}

# Install documentation
cp -f README* %{buildroot}%{_datadir}/doc/%{name}/ || true
cp -f CHANGELOG* %{buildroot}%{_datadir}/doc/%{name}/ || true
cp -f TROUBLESHOOTING.md %{buildroot}%{_datadir}/doc/%{name}/ || true

# Create symbolic links in /usr/bin for convenience
ln -sf /opt/linuxtrack/bin/ltr_gui %{buildroot}%{_bindir}/ltr_gui
ln -sf /opt/linuxtrack/bin/mickey %{buildroot}%{_bindir}/mickey
ln -sf /opt/linuxtrack/bin/ltr_server1 %{buildroot}%{_bindir}/ltr_server1
ln -sf /opt/linuxtrack/bin/verify_installation.sh %{buildroot}%{_bindir}/linuxtrack-verify

# Set up library configuration
echo "/opt/linuxtrack/lib/linuxtrack" > %{buildroot}%{_sysconfdir}/ld.so.conf.d/linuxtrack.conf

# Install udev rules manually (since our Makefile puts them in the wrong place for RPM)
install -m 644 src/99-TIR.rules %{buildroot}%{_udevrulesdir}/
install -m 644 src/99-Mickey.rules %{buildroot}%{_udevrulesdir}/

%files
%license COPYING*
%doc %{_datadir}/doc/%{name}
/opt/linuxtrack/bin/*
/opt/linuxtrack/lib/linuxtrack/*.so*
/opt/linuxtrack/share/linuxtrack/*
%{_bindir}/ltr_gui
%{_bindir}/mickey
%{_bindir}/ltr_server1
%{_bindir}/linuxtrack-verify
%{_datadir}/applications/*.desktop
%{_datadir}/icons/hicolor/48x48/apps/*
%{_datadir}/icons/hicolor/scalable/apps/*
%{_datadir}/pixmaps/*
%{_udevrulesdir}/99-TIR.rules
%{_udevrulesdir}/99-Mickey.rules
%config(noreplace) %{_sysconfdir}/ld.so.conf.d/linuxtrack.conf

%files devel
/opt/linuxtrack/include/*
/opt/linuxtrack/lib/linuxtrack/*.a
/opt/linuxtrack/lib/pkgconfig/*

%files doc
%doc %{_datadir}/doc/%{name}/*

%post
# Reload udev rules
%udev_rules_update

# Update desktop database
if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database %{_datadir}/applications || true
fi

# Update icon cache
if command -v gtk-update-icon-cache >/dev/null 2>&1; then
    gtk-update-icon-cache -f -t %{_datadir}/icons/hicolor || true
fi

# Update library cache
/sbin/ldconfig

echo ""
echo "LinuxTrack installation completed!"
echo ""
echo "To use LinuxTrack:"
echo "  1. Add your user to the plugdev group:"
echo "     sudo usermod -a -G plugdev \$USER"
echo "  2. Log out and log back in"  
echo "  3. Run: ltr_gui"
echo ""
echo "For troubleshooting: linuxtrack-verify"
echo "Documentation: %{_datadir}/doc/%{name}/"
echo ""

%postun
if [ $1 -eq 0 ] ; then
    # Package removal, not upgrade
    %udev_rules_update
    
    # Update desktop database
    if command -v update-desktop-database >/dev/null 2>&1; then
        update-desktop-database %{_datadir}/applications || true
    fi
    
    # Update icon cache
    if command -v gtk-update-icon-cache >/dev/null 2>&1; then
        gtk-update-icon-cache -f -t %{_datadir}/icons/hicolor || true
    fi
    
    # Update library cache
    /sbin/ldconfig
fi

%changelog
* Sun Jun 29 2025 LinuxTrack X-IR Project <fwfa123@users.noreply.github.com> - 0.99.23-1
- Qt5 modernization release
- Complete installation system implementation
- Fixed installation paths to use /opt/linuxtrack
- Added comprehensive desktop integration
- Implemented automatic udev rules installation
- Added installation verification system
- Enhanced troubleshooting documentation
- Improved resource files and help system deployment
- Added Mickey standalone application support
- Fixed GUI resources and 3D model installation
- Comprehensive RPM packaging support

* Wed Jan 01 2025 LinuxTrack Project <linuxtrack@users.noreply.github.com> - 0.99.19-1
- Previous Qt4 release (reference)
- Legacy installation system
- Basic functionality 