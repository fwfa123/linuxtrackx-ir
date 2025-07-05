# Technical Implementation Guide - TrackIR Plugin for OpenTrack
**Based on LinuxTrack TrackIR Implementation Analysis**

## 🎯 **Core Implementation Details**

### **1. USB Device Detection and Communication**

#### **Device IDs (from `libusb_ifc.c`)**
```cpp
// TrackIR Device IDs
#define TIR_VENDOR_ID    0x131D  // NaturalPoint
#define TIR5V3_PRODUCT_ID 0x0159 // TrackIR 5 v3
#define TIR5V2_PRODUCT_ID 0x0158 // TrackIR 5 v2  
#define TIR5_PRODUCT_ID   0x0157 // TrackIR 5
#define TIR4_PRODUCT_ID   0x0156 // TrackIR 4
#define TIR3_PRODUCT_ID   0x0155 // TrackIR 3
#define TIR2_PRODUCT_ID   0x0150 // TrackIR 2

// USB Endpoints
#define TIR_OUT_EP  0x01  // Control endpoint
#define TIR_IN_EP   0x82  // Data endpoint
```

#### **Device Detection Implementation**
```cpp
// Based on ltr_int_find_tir() from libusb_ifc.c
bool TrackIRDevice::find_device() {
    libusb_device **list;
    libusb_device *found = NULL;
    
    ssize_t cnt = libusb_get_device_list(ctx, &list);
    if (cnt < 0) {
        return false;
    }
    
    // Search for TrackIR devices in order of preference
    const std::vector<std::pair<uint16_t, std::string>> devices = {
        {TIR5V3_PRODUCT_ID, "TrackIR 5 v3"},
        {TIR5V2_PRODUCT_ID, "TrackIR 5 v2"},
        {TIR5_PRODUCT_ID, "TrackIR 5"},
        {TIR4_PRODUCT_ID, "TrackIR 4"},
        {TIR3_PRODUCT_ID, "TrackIR 3"},
        {TIR2_PRODUCT_ID, "TrackIR 2"}
    };
    
    for (const auto& device : devices) {
        for (ssize_t i = 0; i < cnt; i++) {
            libusb_device *dev = list[i];
            struct libusb_device_descriptor desc;
            
            if (libusb_get_device_descriptor(dev, &desc) == 0) {
                if (desc.idVendor == TIR_VENDOR_ID && 
                    desc.idProduct == device.first) {
                    found = dev;
                    device_type = device.second;
                    break;
                }
            }
        }
        if (found) break;
    }
    
    if (found) {
        int err = libusb_open(found, &handle);
        if (err == 0) {
            libusb_free_device_list(list, 1);
            return true;
        }
    }
    
    libusb_free_device_list(list, 1);
    return false;
}
```

### **2. TrackIR Hardware Communication**

#### **Control Packets (from `tir_hw.c`)**
```cpp
// TrackIR Control Commands
static const uint8_t VIDEO_OFF[] = {0x14, 0x01};
static const uint8_t VIDEO_ON[] = {0x14, 0x00};
static const uint8_t FIFO_FLUSH[] = {0x12};
static const uint8_t CAMERA_STOP[] = {0x13};
static const uint8_t FPGA_INIT[] = {0x1b};
static const uint8_t CFG_RELOAD[] = {0x20};
static const uint8_t GET_STATUS[] = {0x1d};
static const uint8_t GET_CONF[] = {0x17};
static const uint8_t PRECISION_MODE[] = {0x19, 0x03, 0x10, 0x00, 0x05};
static const uint8_t SET_IR_BRIGHTNESS[] = {0x10, 0x00, 0x02, 0x00, 0xA0};
```

#### **Device Initialization**
```cpp
// Based on init_camera_tir5v3() from tir_hw.c
bool TrackIRDevice::init_camera(bool force_fw_load, bool ir_on) {
    if (!handle) return false;
    
    // Configure device
    if (!ltr_int_prepare_device(TIR_CONFIGURATION, TIR_INTERFACE)) {
        return false;
    }
    
    // Initialize FPGA
    if (!ltr_int_send_data(TIR_OUT_EP, FPGA_INIT, sizeof(FPGA_INIT))) {
        return false;
    }
    
    // Load firmware if needed
    if (force_fw_load || !firmware_loaded) {
        if (!load_firmware()) {
            return false;
        }
    }
    
    // Configure camera
    if (!configure_camera()) {
        return false;
    }
    
    // Set IR LED state
    if (!set_ir_led(ir_on)) {
        return false;
    }
    
    // Start camera
    if (!start_camera()) {
        return false;
    }
    
    return true;
}
```

### **3. IR Frame Processing**

#### **Frame Capture and Processing**
```cpp
// Based on TrackIR frame processing from tir_hw.c
bool TrackIRDevice::get_frame(uint8_t* buffer, size_t* size) {
    if (!handle) return false;
    
    // Read frame data from USB endpoint
    size_t transferred = 0;
    int result = libusb_bulk_transfer(
        handle, 
        TIR_IN_EP, 
        buffer, 
        FRAME_SIZE, 
        &transferred, 
        FRAME_TIMEOUT_MS
    );
    
    if (result == LIBUSB_SUCCESS && transferred > 0) {
        *size = transferred;
        return true;
    }
    
    return false;
}
```

#### **IR Blob Detection (OpenCV Integration)**
```cpp
// Enhanced blob detection using OpenCV
bool TrackIRProcessor::process_frame(const uint8_t* data, size_t size) {
    // Convert YUYV to grayscale
    cv::Mat frame(height, width, CV_8UC2, (void*)data);
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_YUV2GRAY_YUYV);
    
    // Apply threshold for IR detection
    cv::Mat binary;
    cv::threshold(gray, binary, threshold, 255, cv::THRESH_BINARY);
    
    // Find contours (blobs)
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // Filter blobs by size
    std::vector<cv::Point2f> valid_blobs;
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area >= min_blob_size && area <= max_blob_size) {
            cv::Moments moments = cv::moments(contour);
            if (moments.m00 != 0) {
                cv::Point2f center(moments.m10 / moments.m00, 
                                 moments.m01 / moments.m00);
                valid_blobs.push_back(center);
            }
        }
    }
    
    // Process valid blobs for 6DOF tracking
    return process_blobs_for_tracking(valid_blobs);
}
```

### **4. 6DOF Pose Calculation**

#### **Blob to Pose Conversion**
```cpp
// Based on LinuxTrack's tracking algorithms
bool TrackIRProcessor::process_blobs_for_tracking(const std::vector<cv::Point2f>& blobs) {
    if (blobs.size() < 3) {
        // Need at least 3 points for 6DOF tracking
        return false;
    }
    
    // Sort blobs by position (for consistent tracking)
    std::vector<cv::Point2f> sorted_blobs = blobs;
    std::sort(sorted_blobs.begin(), sorted_blobs.end(), 
              [](const cv::Point2f& a, const cv::Point2f& b) {
                  return a.x < b.x;
              });
    
    // Calculate pose based on model type
    switch (model_type) {
        case TrackIRModel::CAP:
            return calculate_cap_pose(sorted_blobs);
        case TrackIRModel::CLIP:
            return calculate_clip_pose(sorted_blobs);
        case TrackIRModel::FACE:
            return calculate_face_pose(sorted_blobs);
        default:
            return false;
    }
}
```

#### **Cap Model Pose Calculation**
```cpp
bool TrackIRProcessor::calculate_cap_pose(const std::vector<cv::Point2f>& blobs) {
    // Cap model uses 3 reflective dots in a triangle pattern
    if (blobs.size() < 3) return false;
    
    // Calculate triangle properties
    cv::Point2f center = (blobs[0] + blobs[1] + blobs[2]) / 3.0f;
    
    // Calculate orientation from triangle
    float angle = calculate_triangle_orientation(blobs);
    
    // Calculate distance from blob size
    float distance = calculate_distance_from_blob_size(blobs);
    
    // Apply calibration matrix
    Pose pose;
    pose.x = center.x - image_center.x;
    pose.y = center.y - image_center.y;
    pose.z = distance;
    pose.yaw = angle;
    pose.pitch = calculate_pitch_from_blobs(blobs);
    pose.roll = calculate_roll_from_blobs(blobs);
    
    current_pose = pose;
    return true;
}
```

### **5. OpenTrack Integration**

#### **Main Plugin Interface**
```cpp
// tracker-trackir.hpp
class TrackerTrackIR : public ITracker {
private:
    std::unique_ptr<TrackIRDevice> device;
    std::unique_ptr<TrackIRProcessor> processor;
    Pose current_pose;
    bool is_running;
    
public:
    TrackerTrackIR();
    ~TrackerTrackIR();
    
    // ITracker interface
    module_status start_tracker(QFrame*) override;
    void data(double *data) override;
    bool center() override;
    
    // Configuration
    void set_threshold(int threshold);
    void set_min_blob_size(int min_size);
    void set_max_blob_size(int max_size);
    void set_model_type(TrackIRModel type);
    void set_ir_brightness(int brightness);
    
private:
    bool initialize_device();
    void process_frame();
};
```

#### **Plugin Implementation**
```cpp
// tracker-trackir.cpp
module_status TrackerTrackIR::start_tracker(QFrame* frame) {
    if (is_running) {
        return status_ok;
    }
    
    // Initialize device
    if (!initialize_device()) {
        return status_error;
    }
    
    // Initialize processor
    if (!processor->initialize()) {
        return status_error;
    }
    
    is_running = true;
    return status_ok;
}

void TrackerTrackIR::data(double *data) {
    if (!is_running) {
        // Return neutral pose
        for (int i = 0; i < 6; i++) {
            data[i] = 0.0;
        }
        return;
    }
    
    // Process current frame
    process_frame();
    
    // Convert to TrackIR format (-16384 to 16384)
    const float TRACKIR_SCALE = 16384.0f;
    data[0] = current_pose.yaw * TRACKIR_SCALE;    // Yaw
    data[1] = current_pose.pitch * TRACKIR_SCALE;  // Pitch
    data[2] = current_pose.roll * TRACKIR_SCALE;   // Roll
    data[3] = current_pose.x * TRACKIR_SCALE;      // X translation
    data[4] = current_pose.y * TRACKIR_SCALE;      // Y translation
    data[5] = current_pose.z * TRACKIR_SCALE;      // Z translation
}

bool TrackerTrackIR::center() {
    if (processor) {
        return processor->calibrate();
    }
    return false;
}
```

### **6. Configuration and GUI**

#### **Qt Configuration Interface**
```cpp
// tracker-trackir.ui (Qt Designer)
class TrackerTrackIRConfig : public QWidget {
    Q_OBJECT
    
private:
    QComboBox* device_combo;
    QSpinBox* threshold_spin;
    QSpinBox* min_blob_spin;
    QSpinBox* max_blob_spin;
    QComboBox* model_combo;
    QSpinBox* ir_brightness_spin;
    QPushButton* test_button;
    QPushButton* calibrate_button;
    
public slots:
    void on_device_changed(int index);
    void on_threshold_changed(int value);
    void on_min_blob_changed(int value);
    void on_max_blob_changed(int value);
    void on_model_changed(int index);
    void on_ir_brightness_changed(int value);
    void on_test_clicked();
    void on_calibrate_clicked();
};
```

#### **Configuration Storage**
```cpp
// tracker-trackir.ini
[tracker-trackir]
device=auto
threshold=140
min-blob-size=4
max-blob-size=2500
model-type=clip
ir-brightness=7
status-led-brightness=0
```

### **7. Build System Integration**

#### **CMakeLists.txt**
```cmake
# tracker-trackir/CMakeLists.txt
set(TRACKER_TRACKIR_SOURCES
    tracker-trackir.cpp
    trackir-device.cpp
    trackir-processor.cpp
)

set(TRACKER_TRACKIR_HEADERS
    tracker-trackir.hpp
    trackir-device.hpp
    trackir-processor.hpp
    trackir-types.hpp
)

set(TRACKER_TRACKIR_UI
    tracker-trackir.ui
)

set(TRACKER_TRACKIR_CONFIG
    tracker-trackir.ini
)

qt5_wrap_cpp(TRACKER_TRACKIR_MOC ${TRACKER_TRACKIR_HEADERS})
qt5_wrap_ui(TRACKER_TRACKIR_UI_H ${TRACKER_TRACKIR_UI})

add_library(tracker-trackir SHARED
    ${TRACKER_TRACKIR_SOURCES}
    ${TRACKER_TRACKIR_MOC}
    ${TRACKER_TRACKIR_UI_H}
)

target_link_libraries(tracker-trackir
    opentrack-api
    ${OpenCV_LIBS}
    ${LIBUSB_LIBRARIES}
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
)

target_include_directories(tracker-trackir PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${LIBUSB_INCLUDE_DIRS}
)
```

## 🚀 **Implementation Steps**

### **Phase 1: Core USB Communication (Week 1-2)**
1. **Set up OpenTrack development environment**
2. **Create basic plugin skeleton**
3. **Implement device detection and USB communication**
4. **Test basic device connectivity**

### **Phase 2: IR Processing (Week 3-4)**
1. **Implement frame capture from TrackIR**
2. **Add OpenCV blob detection**
3. **Implement basic pose calculation**
4. **Test with TrackIR hardware**

### **Phase 3: OpenTrack Integration (Week 5-6)**
1. **Implement ITracker interface**
2. **Add configuration GUI**
3. **Integrate with OpenTrack's output protocols**
4. **Test with games**

### **Phase 4: Polish and Testing (Week 7-8)**
1. **Performance optimization**
2. **Comprehensive testing**
3. **Documentation and release preparation**

## 🎯 **Key Technical Insights from LinuxTrack**

### **1. Device Communication Patterns**
- **Firmware Loading**: TrackIR devices require firmware to be loaded
- **Configuration Sequence**: Specific command sequence for device initialization
- **Frame Format**: YUYV pixel format at specific resolutions

### **2. IR Processing Techniques**
- **Threshold-based Detection**: IR LEDs appear as bright spots
- **Size Filtering**: Filter blobs by area to reduce noise
- **Multi-point Tracking**: Use multiple IR points for 6DOF

### **3. Calibration Requirements**
- **Model-specific Calibration**: Different calibration for Cap vs Clip models
- **Distance Calculation**: Use blob size to estimate distance
- **Orientation Calculation**: Use blob positions to determine orientation

---

**This technical guide provides the foundation for implementing a professional-grade TrackIR plugin for OpenTrack, leveraging the proven techniques from LinuxTrack's implementation.** 