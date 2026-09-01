# Native macOS TrackIR support

This directory contains the native `IOUSBHost` implementation for TIR5V2 and
TIR5V3. `TrackIRUSBBackend` provides device discovery and LinuxTrack transport
operations, while `TrackIRUSBTransport` owns the `IOUSBHostInterface` and its
pipes for the lifetime of the open device.

The production path is:

```text
ltr_server1 -> libtir -> libltusb1 -> TrackIRUSBBackend -> IOUSBHost
```

The server first requests ordinary IOUSBHost ownership. If the matched
interface cannot be opened, it retries with `DeviceCapture`. If capture cannot
be authorized for the current user, the server logs an instruction to rerun
`ltr_server1` with `sudo`; no capture is attempted when no TrackIR device is
present.

`DeviceCapture` is intended for controlled recovery when ordinary ownership
fails. It requires root privileges, or the
`com.apple.vm.device-access` entitlement together with device authorization.
Releasing the captured object resets the device and allows normal matching to
resume.

The probe matches VID `0x131D`, either TIR5V3 PID `0x0159` or TIR5V2 PID
`0x0158` (v3 preferred), configuration `1`, interface `0`, and alternate
setting `0`. Endpoint addresses are read from the descriptors rather than
hard-coded. The production server uses the shared LinuxTrack protocol
implementation for initialization, firmware, and frame acquisition; the USB
layer does not duplicate TrackIR protocol or image-decoding logic.

## Validation

The attached TIR5V2 was matched and opened through `IOUSBHost` using
`DeviceCapture`. Its interface has two bulk endpoints: OUT `0x01` and IN
`0x82`, both with 64-byte maximum packets. The IOUSBHost transport also
completed the control read and protocol transfers, receiving a 20-byte
configuration response and the 7-byte status response
`07 20 01 01 00 00 02`.

TIR5V3 matching and the existing LinuxTrack protocol path are implemented,
but TIR5V3 descriptor, initialization, and frame behavior remain unverified
until hardware is available.

## Install with Homebrew

The repository includes a Homebrew formula at
`Formula/linuxtrackx-ir.rb`. It builds the native macOS Qt GUI, the
IOUSBHost-backed `ltr_server1`, the LinuxTrack libraries, and the command-line
helpers directly into Homebrew's prefix. No `.app` bundle or code-signing
identity is required.

Because this source repository is not named `homebrew-*`, add it as a tap with
its explicit Git URL:

```sh
brew tap wilmai/linuxtrackx-ir https://github.com/wilmai/linuxtrackx-ir.git
brew install wilmai/linuxtrackx-ir/linuxtrackx-ir
```

The formula pins normal installs to the current `main` revision. To build the
latest commit from that branch instead, use:

```sh
brew install --HEAD wilmai/linuxtrackx-ir/linuxtrackx-ir
```

After installation, run `ltr_gui` for the GUI or `ltr_server1` for the
background tracking server. If IOUSBHost needs `DeviceCapture`, run the server
with `sudo` as described above.
