# OpenTrack TrackIR Plugin Implementation Plan
**Based on LinuxTrack TrackIR Implementation Analysis**

## 🎯 **Project Overview**

Create a TrackIR input plugin for OpenTrack that leverages LinuxTrack's proven TrackIR implementation while respecting OpenTrack's architecture and licensing.

## 📋 **Technical Architecture**

### **1. Plugin Structure (OpenTrack Integration)**

```
opentrack/tracker/
├── tracker-trackir/                    # New TrackIR plugin
│   ├── tracker-trackir.cpp            # Main plugin implementation
│   ├── tracker-trackir.hpp            # Header file
│   ├── trackir-device.cpp             # USB device handling
│   ├── trackir-device.hpp             # Device interface
│   ├── trackir-processor.cpp          # IR blob processing
│   ├── trackir-processor.hpp          # Processing interface
│   └── CMakeLists.txt                 # Build configuration
```

### **2. Core Components to Port**

#### **A. USB Device Management (from `ltr_usb_trackir.c`)**
```cpp
// trackir-device.hpp
class TrackIRDevice {
private:
    libusb_context* ctx;
    libusb_device_handle* handle;
    int vendor_id;      // 0x131d (NaturalPoint)
    int product_id;     // Varies by model (TrackIR 2-5)
    
public:
    bool init();
    bool get_frame(uint8_t* buffer, size_t* size);
    void close();
    bool is_connected() const;
    
    // TrackIR-specific methods
    bool download_firmware();
    bool set_resolution(int width, int height);
    bool set_framerate(int fps);
};
```

#### **B. IR Blob Processing (from `ltr_tracker.c`)**
```cpp
// trackir-processor.hpp
class TrackIRProcessor {
private:
    cv::Mat frame;
    std::vector<cv::Point2f> blobs;
    TrackIRModel model_type;  // CAP, CLIP, FACE
    
public:
    bool process_frame(const uint8_t* data, size_t size);
    Pose get_pose() const;  // 6DOF output
    bool calibrate();
    
    // Configuration
    void set_threshold(int threshold);
    void set_min_blob_size(int min_size);
    void set_max_blob_size(int max_size);
    void set_model_type(TrackIRModel type);
};
```

#### **C. Main Plugin Interface (OpenTrack Integration)**
```cpp
// tracker-trackir.hpp
class TrackerTrackIR : public ITracker {
private:
    std::unique_ptr<TrackIRDevice> device;
    std::unique_ptr<TrackIRProcessor> processor;
    
public:
    TrackerTrackIR();
    ~TrackerTrackIR();
    
    // ITracker interface implementation
    module_status start_tracker(QFrame*) override;
    void data(double *data) override;
    bool center() override;
    
    // Configuration
    void set_fps(int fps);
    void set_resolution(int width, int height);
    void set_threshold(int threshold);
};
```

## 🔄 **Porting Strategy**

### **Phase 1: Core USB Communication**
1. **Port `trackir_init()`** → `TrackIRDevice::init()`
   - libusb device detection
   - Vendor/Product ID matching
   - USB interface configuration
   - Firmware download (if needed)

2. **Port `trackir_get_frame()`** → `TrackIRDevice::get_frame()`
   - Raw IR frame capture
   - YUYV pixel format handling
   - Frame rate management

### **Phase 2: IR Processing Pipeline**
1. **Port blob detection logic** from `ltr_tracker.c`
   - OpenCV integration for blob detection
   - Size filtering (min/max blob size)
   - Threshold adjustment
   - Noise reduction

2. **Port 6DOF conversion** from `ltr_tracker_process_frame()`
   - Blob position to pose conversion
   - Calibration matrix application
   - Scale factor application (-16384 to 16384 range)

### **Phase 3: OpenTrack Integration**
1. **Implement ITracker interface**
   - `start_tracker()` - Initialize device and processor
   - `data()` - Return current pose data
   - `center()` - Reset tracking center

2. **Add GUI configuration**
   - TrackIR device selection
   - Blob size/threshold controls
   - Model type selection (Cap/Clip/Face)

## 🛠 **Implementation Details**

### **Key Technical Challenges**

#### **1. C to C++ Conversion**
```cpp
// LinuxTrack C code:
int trackir_init() {
    libusb_init(&ctx);
    // ... device detection
}

// OpenTrack C++ equivalent:
bool TrackIRDevice::init() {
    int result = libusb_init(&ctx);
    if (result != LIBUSB_SUCCESS) {
        return false;
    }
    // ... device detection with RAII
    return true;
}
```

#### **2. Memory Management**
```cpp
// Use RAII and smart pointers
class TrackIRDevice {
private:
    std::unique_ptr<libusb_context, void(*)(libusb_context*)> ctx;
    std::unique_ptr<libusb_device_handle, void(*)(libusb_device_handle*)> handle;
    
public:
    TrackIRDevice() : ctx(nullptr, libusb_exit), handle(nullptr, libusb_close) {}
};
```

#### **3. OpenCV Integration**
```cpp
// Leverage OpenTrack's existing OpenCV usage
bool TrackIRProcessor::process_frame(const uint8_t* data, size_t size) {
    cv::Mat frame(height, width, CV_8UC2, (void*)data);  // YUYV format
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_YUV2GRAY_YUYV);
    
    // Apply threshold
    cv::threshold(gray, gray, threshold, 255, cv::THRESH_BINARY);
    
    // Find contours (blobs)
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // Filter by size and convert to tracking data
    return process_blobs(contours);
}
```

## 🎮 **Game Compatibility**

### **Leverage OpenTrack's Existing Output**
- **proto-freetrack** - Already supports TrackIR-compatible output
- **proto-wine** - Wine integration for Linux gaming
- **proto-udp** - Network output for remote tracking

### **Data Format Compatibility**
```cpp
// Ensure output matches TrackIR's expected format
void TrackerTrackIR::data(double *data) {
    Pose pose = processor->get_pose();
    
    // Convert to TrackIR range (-16384 to 16384)
    data[0] = pose.yaw * TRACKIR_SCALE_FACTOR;    // Yaw
    data[1] = pose.pitch * TRACKIR_SCALE_FACTOR;  // Pitch  
    data[2] = pose.roll * TRACKIR_SCALE_FACTOR;   // Roll
    data[3] = pose.x * TRACKIR_SCALE_FACTOR;      // X translation
    data[4] = pose.y * TRACKIR_SCALE_FACTOR;      // Y translation
    data[5] = pose.z * TRACKIR_SCALE_FACTOR;      // Z translation
}
```

## 📁 **File Organization**

### **Source Files to Create**
1. **`tracker-trackir.cpp/hpp`** - Main plugin interface
2. **`trackir-device.cpp/hpp`** - USB device management
3. **`trackir-processor.cpp/hpp`** - IR processing pipeline
4. **`trackir-types.hpp`** - Common types and constants

### **Configuration Files**
1. **`CMakeLists.txt`** - Build configuration
2. **`tracker-trackir.ini`** - Default configuration
3. **`tracker-trackir.ui`** - Qt Designer UI file

## 🔒 **Legal and Licensing Considerations**

### **Fork Naming and Attribution**
- **Fork Name**: "TrackLinux" or "OpenTrack-TrackIR"
- **Disclaimer**: "Based on OpenTrack and inspired by LinuxTrack's TrackIR support"
- **License Compliance**: Respect both OpenTrack (ISC) and LinuxTrack (GPL) licenses

### **Code Attribution**
```cpp
/*
 * TrackIR plugin for OpenTrack
 * Based on OpenTrack (ISC License) and inspired by LinuxTrack's TrackIR support
 * 
 * Copyright (c) 2025 [Your Name]
 * Based on LinuxTrack TrackIR implementation
 * 
 * This code is provided for educational and compatibility purposes only.
 * Not affiliated with OpenTrack or NaturalPoint.
 */
```

## 🚀 **Development Roadmap**

### **Week 1-2: Foundation**
- Set up OpenTrack development environment
- Study OpenTrack's plugin architecture
- Create basic plugin skeleton

### **Week 3-4: USB Communication**
- Port LinuxTrack's USB device handling
- Implement device detection and initialization
- Test basic frame capture

### **Week 5-6: IR Processing**
- Port blob detection and processing
- Implement 6DOF conversion
- Add calibration logic

### **Week 7-8: OpenTrack Integration**
- Implement ITracker interface
- Add GUI configuration
- Test with OpenTrack's output protocols

### **Week 9-10: Testing and Polish**
- Comprehensive testing with TrackIR hardware
- Performance optimization
- Documentation and release preparation

## 🎯 **Success Criteria**

1. **Hardware Compatibility**: Works with TrackIR 2-5 cameras
2. **Software Integration**: Seamless OpenTrack integration
3. **Game Compatibility**: Works with TrackIR-compatible games
4. **Performance**: 120 FPS tracking capability
5. **User Experience**: Intuitive configuration interface

---

**This plan provides a solid foundation for creating a professional-grade TrackIR plugin for OpenTrack while respecting the existing codebase and licensing requirements.** 