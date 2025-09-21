# Linuxtrack Interface

**LinuxTrack X-IR v0.99.25** provides a comprehensive interface allowing developers to incorporate head-tracking into their applications. This interface is compatible with the original LinuxTrack API and has been maintained for backward compatibility while adding modern features.

The interface consists of two files: linuxtrack.h and linuxtrack.c; just add them to your project and you get access to the LinuxTrack X-IR's functionality.

The reason why the interface is not provided in the usual form of a library is the following: if you'd ship an application directly linked against LinuxTrack X-IR's interface library, it wouldn't work on a system where LinuxTrack X-IR is not installed or configured properly. Technically the LinuxTrack X-IR might be provided in a form of a static library, but there might be problems when you'd use it to build a dynamic library (say a plug-in).

The interface in its current form looks for the LinuxTrack X-IR, and when found it is used; when the LinuxTrack X-IR is not found (or not configured properly), it just indicates failure and the main application can work on.

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

## Interface overview

```
typedef enum {LINUXTRACK_OK, INITIALIZING, RUNNING, PAUSED, STOPPED,
  err_NOT_INITIALIZED, err_SYMBOL_LOOKUP, err_NO_CONFIG, err_NOT_FOUND, err_PROCESSING_FRAME
} linuxtrack_state_type;

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

Basic Linuxtrack interface is relatively simple: **linuxtrack\_init** initializes the interface provided a string identifying your application (specifies a profile to be used), **linuxtrack\_shutdown** shuts the interface down.

Pair **linuxtrack\_suspend** and **linuxtrack\_wakeup** allow you to pause and resume the tracking temporarily; technically it might seem redundant, but pausing and resuming via these two functions is usually much quicker than shutting the whole tracker down and initializing it again.

Function **linuxtrack\_recenter** allows user to recenter the tracker when needed.

Function **linuxtrack\_get\_tracking\_state** returns current state of the Linuxtrack's interface; the tracker itself runs in a different process and commands for it are delivered asynchronously. For that reason you might want to query tracker's state to see if your command was performed yet or not. To convert the state id to a meaningfull string use function **linuxtrack\_explain**.

Function **linuxtrack\_get\_pose** returns you the current pose and state info. Normally you should use it once per (your applications) frame to obtain the current pose. The pose data are processed already (filtered, scaled, response curves applied according to the profile you specified upon initialization), so it doesn't make much sense to call it any faster.

## Interface description

```
linuxtrack_state_type linuxtrack_init(const char *cust_section);
```

Requests initialization of the tracker. Looks for Linuxtrack installation and if it finds one, it tries to start the tracking. It is required that Linuxtrack is not only installed, but also configured correctly by the user.

Parameter cust\_section indicates a profile name to be used; if it is NULL, default profile ("Default") is used.

Return value of 0 means success. Note however, that it doesn't mean that the tracker started successfully - it means the initialization request was sent.

To check, if the tracker actually started, check the return value of linuxtrack\_get\_tracking\_state function. Just remember, that it can take several seconds for the tracker to start. Note however, that you can start calling linuxtrack\_get\_pose immediately - if the Linuxtrack was not initialized yet, it will just return "zero" pose (all angles and distances set to zero).

```
linuxtrack_state_type linuxtrack_shutdown(void);
```

Requests tracker shutdown. Due to the fact, that there might be several applications using Linuxtrack running in parallel, the tracker actually closes down only when the last client disconnects.

Return value of 0 means success.

```
linuxtrack_state_type linuxtrack_suspend(void);
linuxtrack_state_type linuxtrack_wakeup(void);
```

Requests the tracking be paused or resumed. Pausing the tracking is especially useful for TrackIR/SmartNav class of devices, because it turns off their IR LEDs, lowering the device's power dissipation.

Return value of 0 means success.

```
linuxtrack_state_type linuxtrack_recenter(void);
```

Requests the current pose should be used as a new center pose.

Return value of 0 means success.

```
typedef enum {LINUXTRACK_OK, INITIALIZING, RUNNING, PAUSED, STOPPED,
  err_NOT_INITIALIZED, err_SYMBOL_LOOKUP, err_NO_CONFIG, err_NOT_FOUND, err_PROCESSING_FRAME
} linuxtrack_state_type;
linuxtrack_state_type linuxtrack_get_tracking_state(void);
const char *linuxtrack_explain(linuxtrack_state_type err);
```

Requests information on the current tracker state.

Return value indicates the tracker state according to the linuxtrack\_state\_type enum.

```
const char *linuxtrack_explain(linuxtrack_state_type err);
```

Returns a string describing the err state.

```
int linuxtrack_get_pose(float *heading, float *pitch, float *roll,
                        float *tx, float *ty, float *tz, uint32_t *counter);
```

Requests the current pose. Heading, pitch and roll values are in degrees, translation values tx, ty, tz are in millimeters. Counter value denotes tracker frame number, from which the current pose was computed; if the number doesn't change in between two calls, the pose didn't change either.

In case of a problem (Linuxtrack not installed, not configured, miss-configured, not initialized yet...), the reported pose consists of all zeros.

Return value of 1 means the new pose is available, 0 means no new pose, but everything else OK, -1 signals a problem.

## Advanced interface

If, for some reason the normal interface is not sufficient for your needs (for normal head-tracking applications it should be), there is an advanced interface available. This interface provides additional low level information on current pose, like sensor resolution, raw pose values (before processing) and blob coordinates. This interface is meant mainly for computer vision research and similar applications.
Please note, that using raw values completely bypasses Linuxtrack's profile system, application of sensitivity curves and filtration.

## Advanced Interface overview

```
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

On top of the functions available in the basic interface, there is the **linuxtrack\_get\_pose\_full** function, that provides all the low level informations on the current pose.

# Advanced interface description

```
int linuxtrack_get_pose_full(linuxtrack_pose_t *pose, float blobs[], int num_blobs, int *blobs_read);
```

Requests all the low level informations on the current pose. These include raw pose values (no sensitivity curves or filtration applied), sensor resolution and blob coordinates.

The blobs array should contain 3x the num\_blobs elements (each blob has the x,y coordinates and a number of pixels value). Value of blobs\_read indicates how many blobs were actually read.

At the moment Linuxtrack reports up to 10 blobs; this limit can be increased only by its recompilation (ltlib.h, value of MAX\_BLOBS macro). Note however that Wiimote is capable of reporting only 4 blobs (hardware limitation)!

```
int linuxtrack_get_abs_pose(float *heading, float *pitch, float *roll,
                        float *tx, float *ty, float *tz, uint32_t *counter);
```

Same as **linuxtrack\_get\_pose**, with the exception that the absolute pose in camera coordinates is provided.

```
linuxtrack_state_type linuxtrack_request_frames(void);
```

By calling this function, you notify the tracker that you wish to receive "images" from the tracking device.

```
int linuxtrack_get_frame(int *req_width, int *req_height, size_t buf_size, uint8_t *buffer);
```

Copies an image (if available)to the client supplied buffer. Buffer size is passed through buf\_size
parameter (to avoid overrun). Parameters req\_widht and req\_height return the width and height of the frame.

Returns zero in case of a problem - check the req\_width/req\_height values to see if the buffer passed-in is big enough and enlarge it, if necessary.

Nonzero value means success.

```
linuxtrack_state_type linuxtrack_notification_on(void);
```

Requests notifications on incomming frames be sent via a pipe.

```
int linuxtrack_get_notify_pipe(void);
```

This function is to be used if you need to implement the polling yourself (you poll on several files, ...). In any other case, **linuxtrack\_wait** is easier way to receive notifications.

Returns file descriptor of the notifications pipe. The pipe is set for nonblocking access. Make sure you read all that is in the pipe in order to receive notification when new frame arrives.

```
int linuxtrack_wait(int timeout);
```

When called, the function block until either a new frame arrives, or until the timeout (in miliseconds) is up.

Returns one in case of the new frame arrival; zero means timeout and negative value means problem (other side of the pipe hanged-up, ...).

# X-Plane plugin interface

In order to allow other X-Plane plugins to utilize Linuxtrack, Linuxtrack's X-Plane plugin (xlinuxtrack) exports the following datarefs:

```
linuxtrack/pilots_head_x
linuxtrack/pilots_head_y
linuxtrack/pilots_head_z
linuxtrack/pilots_head_psi
linuxtrack/pilots_head_the
linuxtrack/pilots_head_roll
```

All are floats, distances are in meters, angles in degrees; these values are directly compatible with X-Planes sim/graphics/view/pilots\_head\* datarefs.
There is one more dataref:

```
linuxtrack/enable_head_control
```

With this integer dataref, you can enable or disable head controll by xlinuxtrack. Zero means xlinuxtrack doesn't controll view, nonzero value means it does controll the view.

# Hello World examples

Linuxtrack provides two "Hello World" examples of the Linuxtrack's interface in external program.
The first one called linuxtrack\_hello\_world.c shows how to use the Linuxtrack's interface, while linuxtrack\_hello\_world\_adv.c provides example of advanced interface usage.

Both programs should be fairly straightforward; all they do is tracking initialization, they track for a bit, pause the tracking, resume it, after a few moments they close the tracking and close down.

There are a few things worth noting. First of all, Linuxtrack's interface is asynchronous - the fact, that a function returned only means that the request was sent, not that it was carried out. For example, running **linuxtrack\_suspend** to pause tracking, does'n stop the tracking immediately; it just sent the tracker a command to pause the tracking, that will be carried out as soon as possible. If you want to be absolutely sure that the command was performed, query the tracker status. Note however, that blocking the application waiting for the status change is not recommended, mainly because it hinders user experience (unresponsive application). It can also cause race condition problems due to the fact, that several applications might issue different commands simultaneously.

This might seem to be a problem, but in fact it is not. Thanks to the fact, that **linuxtrack\_get\_pose** and **linuxtrack\_get\_pose\_full** return value higher than zero only when a now pose is available, you will not receive invalid pose no matter when you call it. This means, that your application can safely start calling those functions right after the initialization is requested (by means of calling  **linuxtrack\_init**), provided that you process the pose informations only when higher than zero value is returned.

By default, tracking is automatically re-centered when the tracker is started. If user looks away in that instant, he is going to need to recenter the tracking. To do that, simply call **linuxtrack\_recenter**.

The **initialize\_tracking** function used in the examples contains a loop waiting for the tracker initialization. As discussed above, normally there is no need to do that - it would unnecessarily delay the start of your application. In the example it is used to make things simple, there aren't any clean means to close the application in case the tracker doesn't work (except for Ctrl-C, but that is not very nice way).

# Building

The only thing that Linuxtrack interface needs is that your application is linked against libel. The reason is, that the interface dynamically loads the Linuxtrack library (if available) and for that it needs libdl.
Here is the command-line used to build the example code:

```
gcc -o linuxtrack_hello_world -g -I. linuxtrack.c linuxtrack_hello_world.c -ldl
gcc -o linuxtrack_hello_world_adv -g -I. linuxtrack.c linuxtrack_hello_world_adv.c -ldl
```

The "-ldl" is used to link against libdl; if you use some IDE, just add it to linker flags and you should be good to go. The "-I." flag instructs the compiler to look for linuxtrack.h in the current directory (you might need it to point somewhere else, for example if you store all .h files in a single location). It is also expected that you have the linuxtrack.c file in your project directory.
