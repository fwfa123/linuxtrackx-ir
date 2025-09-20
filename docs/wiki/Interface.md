# LinuxTrack X-IR Interface Documentation

## Overview

**LinuxTrack X-IR v0.99.25** provides a comprehensive interface allowing developers to incorporate head-tracking into their applications. This interface is compatible with the original LinuxTrack API and has been maintained for backward compatibility while adding modern features.

The interface consists of two files: `linuxtrack.h` and `linuxtrack.c`; just add them to your project and you get access to the LinuxTrack X-IR's functionality.

## Modern Development Features

LinuxTrack X-IR v0.99.25 includes several modern development features:

- **Qt5 Modernization**: Complete migration from Qt4 to Qt5 for modern Linux compatibility
- **Enhanced Security**: Position Independent Executables (PIE), stack protector, and fortify source enabled by default
- **Cross-Distribution Support**: Optimized builds for Ubuntu, Fedora, Arch Linux, Debian, and MX Linux
- **Gaming Platform Integration**: Steam Proton and Lutris support with Wine Bridge integration
- **Modern Build System**: Updated autotools and CMake support for flexible build configurations

## API Compatibility

The LinuxTrack X-IR API maintains full backward compatibility with the original LinuxTrack interface. Existing applications will work without modification, while benefiting from:

- Improved performance and stability
- Better error handling and debugging information
- Enhanced security features
- Modern Linux distribution support

## Interface Overview

The basic LinuxTrack interface is relatively simple and provides essential functionality for head-tracking applications.

### Basic Types

```c
typedef enum {LINUXTRACK_OK, INITIALIZING, RUNNING, PAUSED, STOPPED,
  err_NOT_INITIALIZED, err_SYMBOL_LOOKUP, err_NO_CONFIG, err_NOT_FOUND, err_PROCESSING_FRAME
} linuxtrack_state_type;
```

### Basic Functions

```c
linuxtrack_state_type linuxtrack_init(const char *cust_section);
linuxtrack_state_type linuxtrack_shutdown(void);
linuxtrack_state_type linuxtrack_suspend(void);
linuxtrack_state_type linuxtrack_wakeup(void);
linuxtrack_state_type linuxtrack_recenter(void);
const char *linuxtrack_explain(linuxtrack_state_type err);
linuxtrack_state_type linuxtrack_get_tracking_state(void);
int linuxtrack_get_pose(float *heading, float *pitch, float *roll,
                        float *tx, float *ty, float *tz, uint32_t *counter);
```

## Basic Interface Description

### linuxtrack_init

```c
linuxtrack_state_type linuxtrack_init(const char *cust_section);
```

Requests initialization of the tracker. Looks for LinuxTrack X-IR installation and if it finds one, it tries to start the tracking. It is required that LinuxTrack X-IR is not only installed, but also configured correctly by the user.

**Parameters:**
- `cust_section`: indicates a profile name to be used; if it is NULL, default profile ("Default") is used.

**Return value:** 0 means success. Note however, that it doesn't mean that the tracker started successfully - it means the initialization request was sent.

To check if the tracker actually started, check the return value of `linuxtrack_get_tracking_state` function. Just remember that it can take several seconds for the tracker to start. Note however, that you can start calling `linuxtrack_get_pose` immediately - if the LinuxTrack X-IR was not initialized yet, it will just return "zero" pose (all angles and distances set to zero).

### linuxtrack_shutdown

```c
linuxtrack_state_type linuxtrack_shutdown(void);
```

Requests tracker shutdown. Due to the fact that there might be several applications using LinuxTrack X-IR running in parallel, the tracker actually closes down only when the last client disconnects.

**Return value:** 0 means success.

### linuxtrack_suspend / linuxtrack_wakeup

```c
linuxtrack_state_type linuxtrack_suspend(void);
linuxtrack_state_type linuxtrack_wakeup(void);
```

Requests the tracking be paused or resumed. Pausing the tracking is especially useful for TrackIR/SmartNav class of devices, because it turns off their IR LEDs, lowering the device's power dissipation.

**Return value:** 0 means success.

### linuxtrack_recenter

```c
linuxtrack_state_type linuxtrack_recenter(void);
```

Requests the current pose should be used as a new center pose.

**Return value:** 0 means success.

### linuxtrack_get_tracking_state / linuxtrack_explain

```c
linuxtrack_state_type linuxtrack_get_tracking_state(void);
const char *linuxtrack_explain(linuxtrack_state_type err);
```

Requests information on the current tracker state.

**Return value:** indicates the tracker state according to the `linuxtrack_state_type` enum.

### linuxtrack_get_pose

```c
int linuxtrack_get_pose(float *heading, float *pitch, float *roll,
                        float *tx, float *ty, float *tz, uint32_t *counter);
```

Requests the current pose. Heading, pitch and roll values are in degrees, translation values tx, ty, tz are in millimeters. Counter value denotes tracker frame number, from which the current pose was computed; if the number doesn't change in between two calls, the pose didn't change either.

In case of a problem (LinuxTrack X-IR not installed, not configured, miss-configured, not initialized yet...), the reported pose consists of all zeros.

**Return value:**
- 1: new pose is available
- 0: no new pose, but everything else OK
- -1: signals a problem

## Advanced Interface

If, for some reason the normal interface is not sufficient for your needs (for normal head-tracking applications it should be), there is an advanced interface available. This interface provides additional low level information on current pose, like sensor resolution, raw pose values (before processing) and blob coordinates. This interface is meant mainly for computer vision research and similar applications.

**Note:** Using raw values completely bypasses LinuxTrack X-IR's profile system, application of sensitivity curves and filtration.

### Advanced Interface Overview

```c
typedef struct{
  float pitch;
  float yaw;
  float roll;
  float tx;
  float ty;
  float tz;
  uint32_t counter;
  uint32_t resolution_x;
  uint32_t resolution_y;
  float raw_pitch;
  float raw_yaw;
  float raw_roll;
  float raw_tx;
  float raw_ty;
  float raw_tz;
  uint8_t status;
} linuxtrack_pose_t;

int linuxtrack_get_pose_full(linuxtrack_pose_t *pose, float blobs[], int num_blobs, int *blobs_read);
int linuxtrack_get_abs_pose(float *heading, float *pitch, float *roll,
                        float *tx, float *ty, float *tz, uint32_t *counter);
linuxtrack_state_type linuxtrack_request_frames(void);
int linuxtrack_get_frame(int *req_width, int *req_height, size_t buf_size, uint8_t *buffer);
linuxtrack_state_type linuxtrack_notification_on(void);
int linuxtrack_get_notify_pipe(void);
int linuxtrack_wait(int timeout);
```

On top of the functions available in the basic interface, there is the `linuxtrack_get_pose_full` function, that provides all the low level informations on the current pose.

## Advanced Interface Description

### linuxtrack_get_pose_full

```c
int linuxtrack_get_pose_full(linuxtrack_pose_t *pose, float blobs[], int num_blobs, int *blobs_read);
```

Requests all the low level informations on the current pose. These include raw pose values (no sensitivity curves or filtration applied), sensor resolution and blob coordinates.

**Parameters:**
- `pose`: pointer to linuxtrack_pose_t structure
- `blobs`: array should contain 3x the num_blobs elements (each blob has the x,y coordinates and a number of pixels value)
- `num_blobs`: maximum number of blobs to read
- `blobs_read`: indicates how many blobs were actually read

**Return value:** 0 in case of a problem, nonzero means success.

At the moment LinuxTrack X-IR reports up to 10 blobs; this limit can be increased only by its recompilation (ltlib.h, value of MAX_BLOBS macro). Note however that Wiimote is capable of reporting only 4 blobs (hardware limitation)!

### linuxtrack_get_abs_pose

```c
int linuxtrack_get_abs_pose(float *heading, float *pitch, float *roll,
                        float *tx, float *ty, float *tz, uint32_t *counter);
```

Same as `linuxtrack_get_pose`, with the exception that the absolute pose in camera coordinates is provided.

**Return value:** Same as `linuxtrack_get_pose`.

### linuxtrack_request_frames

```c
linuxtrack_state_type linuxtrack_request_frames(void);
```

By calling this function, you notify the tracker that you wish to receive "images" from the tracking device.

**Return value:** linuxtrack_state_type indicating success or failure.

### linuxtrack_get_frame

```c
int linuxtrack_get_frame(int *req_width, int *req_height, size_t buf_size, uint8_t *buffer);
```

Copies an image (if available) to the client supplied buffer. Buffer size is passed through buf_size parameter (to avoid overrun). Parameters req_width and req_height return the width and height of the frame.

**Return value:** Zero in case of a problem - check the req_width/req_height values to see if the buffer passed-in is big enough and enlarge it, if necessary. Nonzero value means success.

### linuxtrack_notification_on

```c
linuxtrack_state_type linuxtrack_notification_on(void);
```

Requests notifications on incoming frames be sent via a pipe.

**Return value:** linuxtrack_state_type indicating success or failure.

### linuxtrack_get_notify_pipe

```c
int linuxtrack_get_notify_pipe(void);
```

This function is to be used if you need to implement the polling yourself (you poll on several files, ...). In any other case, `linuxtrack_wait` is easier way to receive notifications.

**Return value:** File descriptor of the notifications pipe. The pipe is set for nonblocking access. Make sure you read all that is in the pipe in order to receive notification when new frame arrives.

### linuxtrack_wait

```c
int linuxtrack_wait(int timeout);
```

When called, the function block until either a new frame arrives, or until the timeout (in milliseconds) is up.

**Return value:** One in case of the new frame arrival; zero means timeout and negative value means problem (other side of the pipe hanged-up, ...).

## X-Plane Plugin Interface

In order to allow other X-Plane plugins to utilize LinuxTrack X-IR, LinuxTrack X-IR's X-Plane plugin (xlinuxtrack) exports the following datarefs:

```c
linuxtrack/pilots_head_x
linuxtrack/pilots_head_y
linuxtrack/pilots_head_z
linuxtrack/pilots_head_psi
linuxtrack/pilots_head_the
linuxtrack/pilots_head_roll
```

All are floats, distances are in meters, angles in degrees; these values are directly compatible with X-Planes sim/graphics/view/pilots_head* datarefs.

There is one more dataref:

```c
linuxtrack/enable_head_control
```

With this integer dataref, you can enable or disable head control by xlinuxtrack. Zero means xlinuxtrack doesn't control view, nonzero value means it does control the view.

## Hello World Examples

LinuxTrack X-IR provides two "Hello World" examples of the LinuxTrack X-IR's interface in external program. The first one called `linuxtrack_hello_world.c` shows how to use the LinuxTrack X-IR's interface, while `linuxtrack_hello_world_adv.c` provides example of advanced interface usage.

Both programs should be fairly straightforward; all they do is tracking initialization, they track for a bit, pause the tracking, resume it, after a few moments they close the tracking and close down.

There are a few things worth noting. First of all, LinuxTrack X-IR's interface is asynchronous - the fact that a function returned only means that the request was sent, not that it was carried out. For example, running `linuxtrack_suspend` to pause tracking, doesn't stop the tracking immediately; it just sent the tracker a command to pause the tracking, that will be carried out as soon as possible. If you want to be absolutely sure that the command was performed, query the tracker status. Note however, that blocking the application waiting for the status change is not recommended, mainly because it hinders user experience (unresponsive application). It can also cause race condition problems due to the fact that several applications might issue different commands simultaneously.

This might seem to be a problem, but in fact it is not. Thanks to the fact that `linuxtrack_get_pose` and `linuxtrack_get_pose_full` return value higher than zero only when a new pose is available, you will not receive invalid pose no matter when you call it. This means that your application can safely start calling those functions right after the initialization is requested (by means of calling `linuxtrack_init`), provided that you process the pose informations only when higher than zero value is returned.

By default, tracking is automatically re-centered when the tracker is started. If user looks away in that instant, he is going to need to recenter the tracking. To do that, simply call `linuxtrack_recenter`.

The `initialize_tracking` function used in the examples contains a loop waiting for the tracker initialization. As discussed above, normally there is no need to do that - it would unnecessarily delay the start of your application. In the example it is used to make things simple, there aren't any clean means to close the application in case the tracker doesn't work (except for Ctrl-C, but that is not very nice way).

## Building Applications

The only thing that LinuxTrack X-IR interface needs is that your application is linked against libdl. The reason is that the interface dynamically loads the LinuxTrack X-IR library (if available) and for that it needs libdl.

Here is the command-line used to build the example code:

```bash
gcc -o linuxtrack_hello_world -g -I. linuxtrack.c linuxtrack_hello_world.c -ldl
gcc -o linuxtrack_hello_world_adv -g -I. linuxtrack.c linuxtrack_hello_world_adv.c -ldl
```

The "-ldl" is used to link against libdl; if you use some IDE, just add it to linker flags and you should be good to go. The "-I." flag instructs the compiler to look for linuxtrack.h in the current directory (you might need it to point somewhere else, for example if you store all .h files in a single location). It is also expected that you have the linuxtrack.c file in your project directory.

## Migration from Original LinuxTrack

### For Existing Applications

**Existing Users**: The fork maintains compatibility with original configurations. Existing applications will continue to work without modification.

**New Users**: Follow the updated installation guides for modern distributions.

**TrackIR Users**: Full compatibility maintained and enhanced with modern Wine Bridge support.

### For Developers

**Build System**: Updated to modern autotools and CMake with enhanced cross-distribution support.

**Dependencies**: Qt5 instead of Qt4, winegcc for Wine compatibility with improved security features.

**Documentation**: Comprehensive guides and troubleshooting with enhanced error handling.

**Security**: Modern security features enabled by default including PIE, stack protector, and fortify source.

---

For more information about using LinuxTrack X-IR in your applications:

- [Welcome](Welcome.md) - Getting started with LinuxTrack X-IR
- [Device Setup](Device-Setup.md) - Hardware configuration guide
- [Gaming Integration](Gaming-Integration.md) - Wine Bridge and gaming platform support
- [System Information](System.md) - System information and troubleshooting
- [News](News.md) - Latest features and updates

For additional support:

- [GitLab Issues](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) - Report bugs and request features
- [Community Forums](https://forums.x-plane.org/index.php?act=idx) - Get help from the community
- [Technical Documentation](../technical/) - Detailed technical guides
