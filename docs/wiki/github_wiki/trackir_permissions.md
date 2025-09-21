# TrackIR Permission Setup

When using TrackIR devices with LinuxTrack X-IR on Linux, you need to install udev rules to grant your user account permission to access the TrackIR hardware. This guide explains the permission installation process and troubleshooting steps.

## Initial Permission Setup

The first time you start LinuxTrack X-IR with a TrackIR device connected, you'll see a permission setup dialog if the required udev rules are not installed.

![TrackIR Permission Setup dialog showing device detected but permissions missing](images/trackir_permissions-1.png)

This dialog appears when:

- TrackIR device is detected by LinuxTrack X-IR

- udev rules for TrackIR access are not installed

- Your user account is not in the required groups

To proceed with the installation:

1. Click **"Install Rules & Add to Groups"** (the highlighted button)
2. Enter your sudo password when prompted
3. Wait for the installation to complete

## Permission Setup After Refresh

If you start LinuxTrack X-IR without a TrackIR device connected and then plug it in later, you can trigger the permission setup by clicking the **"Refresh"** button in the Device Setup tab.

![TrackIR Permission Setup dialog triggered by refresh button](images/trackir_permissions-1a.png)

This will show the same permission setup dialog, allowing you to install the required udev rules and add your user to the necessary groups.

## Completing the Installation

After successfully installing the TrackIR permissions, you'll see a completion dialog with important next steps.

*Installation Complete dialog showing next steps*

The installation process will:

- Install udev rules for TrackIR and Mickey devices

- Add your user account to the required groups

- Configure system permissions for hardware access

## Required Actions After Installation

For the permission changes to take effect, you must:

1. **Log out and log back in** to refresh your user group memberships
2. **Unplug and replug your TrackIR device** to trigger the new udev rules

You can choose to log out immediately by clicking **"Yes"** in the completion dialog, or click **"No"** to log out manually later.

## Troubleshooting

If the permission setup dialog appears again after completing the installation and following the required steps, try these troubleshooting steps:

### Step 1: Verify Installation

Check if the udev rules were installed correctly:

```
ls -la /lib/udev/rules.d/99-TIR.rules
ls -la /lib/udev/rules.d/99-Mickey.rules
```

### Step 2: Check Group Membership

Verify your user is in the required groups:

```
groups $USER
```

You should see groups like `input`, `plugdev`, or similar hardware access groups.

### Step 3: Reboot System

If the permission dialog continues to appear after logging out and back in, try rebooting your system:

1. Save any open work
2. Reboot your computer
3. After reboot, start LinuxTrack X-IR
4. Connect your TrackIR device

### Step 4: Manual Installation

If automatic installation fails, you can install the udev rules manually:

```
sudo cp 99-TIR.rules /lib/udev/rules.d/
sudo cp 99-Mickey.rules /lib/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## What the Installation Does

The TrackIR permission installation process:

- **Installs udev rules** that allow your user account to access TrackIR hardware

- **Adds your user to hardware groups** required for USB device access

- **Configures system permissions** for head tracking devices

- **Enables automatic device detection** when TrackIR is connected

Once properly installed, you won't see the permission setup dialog again, and your TrackIR device will work seamlessly with LinuxTrack X-IR.
