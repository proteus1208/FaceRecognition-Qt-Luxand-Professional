# Face Recognition System - Professional Qt + Luxand FaceSDK

## Overview
A comprehensive professional-grade face recognition application built with Qt 6, Luxand FaceSDK, and advanced camera integration. Supports real-time face detection, registration, identification, and video recording from multiple sources including PC webcams and IP cameras.

## Features

### Core Functionality
- **Real-time Face Detection** - Detect faces in live camera feeds
- **Face Registration/Enrollment** - Register new faces with multiple angles and lighting conditions
- **Face Recognition** - Identify faces against database with confidence scores
- **Multi-Camera Support** - PC Webcam + IP Camera (RTSP) simultaneous streaming
- **Video Recording** - Record camera feeds with face detection overlays
- **Face Database** - SQLite database for face templates and metadata

### Technical Features
- Qt 6 Modern UI with QML support
- Luxand FaceSDK integration (v6.1.0+)
- Multi-threaded recognition engine
- RTSP/HTTP IP camera streaming
- Comprehensive logging and error handling
- Configuration file management
- Cross-platform (Windows, Linux, macOS)

## Project Structure

```
FaceRecognition-Qt-Luxand-Professional/
├── src/
│   ├── main.cpp                          # Application entry point
│   ├── mainwindow.h/cpp                  # Main window logic
│   ├── core/
│   │   ├── application.h/cpp             # Core application
│   │   └── constants.h                   # Global constants
│   ├── ui/
│   │   ├── mainwindow.ui                 # Main window UI (Designer)
│   │   ├── mainwindow.h/cpp              # Main window implementation
│   │   ├── enrollmentdialog.ui/h/cpp     # Face enrollment dialog
│   │   └── settingsdialog.ui/h/cpp       # Application settings
│   ├── camera/
│   │   ├── camerahandler.h/cpp           # PC Webcam handler
│   │   ├── ipcamerahandler.h/cpp         # IP Camera handler (RTSP)
│   │   └── cameramanager.h/cpp           # Camera management
│   ├── database/
│   │   ├── facedatabase.h/cpp            # Database operations
│   │   ├── databasemanager.h/cpp         # Database management
│   │   └── facerecord.h                  # Face record structure
│   ├── luxand/
│   │   ├── luxandfaceengine.h/cpp        # Luxand SDK wrapper
│   │   ├── facedetector.h/cpp            # Face detection
│   │   └── facerecognizer.h/cpp          # Face recognition
│   ├── recognition/
│   │   ├── recognitionworker.h/cpp       # Worker thread for recognition
│   │   ├── recognitionengine.h/cpp       # Recognition engine
│   │   └── recognitionresult.h           # Recognition result structure
│   └── utils/
│       ├── logger.h/cpp                  # Logging system
│       ├── configmanager.h/cpp           # Configuration management
│       └── imageprocessor.h/cpp          # Image processing utilities
├── resources/
│   ├── resources.qrc                     # Qt resource file
│   ├── images/                           # UI images and icons
│   ├── config/
│   │   └── app_config.ini                # Application configuration
│   └── database/
│       └── schema.sql                    # Database schema
├── tests/
│   ├── cameratest.cpp                    # Camera tests
│   └── luxandtest.cpp                    # Luxand SDK tests
├── FaceRecognition.pro                   # Qt project file
├── README.md                             # This file
└── SETUP.md                              # Setup and installation guide
```

## Requirements

### System Requirements
- **OS**: Windows 10+, Linux (Ubuntu 20.04+), macOS 10.15+
- **CPU**: Intel i5/Ryzen 5 or better
- **RAM**: 4GB minimum, 8GB recommended
- **Storage**: 500MB for application + database

### Software Requirements
- Qt 6.2 or later
- Visual Studio 2019+ (Windows) or GCC 9+ (Linux)
- Luxand FaceSDK 6.1.0 or later
- SQLite 3
- OpenCV 4.5+ (optional, for fallback face detection)

## Installation & Setup

### Step 1: Install Luxand FaceSDK

**Windows:**
```bash
# Download from https://www.luxand.com/download/
# Install to C:\Program Files\Luxand\FaceSDK
# Or set LUXAND_SDK_PATH in FaceRecognition.pro
```

**Linux:**
```bash
sudo apt-get install libfacesdk-dev
# Update include/lib paths in .pro file
```

### Step 2: Install Qt 6

**Windows:**
- Download Qt 6 from https://www.qt.io/download
- Install with MSVC compiler
- Set Qt path in environment variables

**Linux:**
```bash
sudo apt-get install qt6-base-dev qt6-multimedia-dev
```

### Step 3: Clone & Build

```bash
git clone https://github.com/proteus1208/FaceRecognition-Qt-Luxand-Professional.git
cd FaceRecognition-Qt-Luxand-Professional

# Open in Qt Creator
qt-creator FaceRecognition.pro

# Or build from command line
qmake FaceRecognition.pro
make
```

### Step 4: Configure Application

1. Edit `resources/config/app_config.ini`:
```ini
[Luxand]
license_key=YOUR_LICENSE_KEY_HERE

[Camera]
default_webcam_index=0
enable_ip_camera=true
ip_camera_url=rtsp://192.168.1.100:554/stream

[Database]
path=./database/faces.db

[Recognition]
confidence_threshold=0.85
min_face_size=20
```

2. Initialize database:
```bash
sqlite3 database/faces.db < resources/database/schema.sql
```

## Usage

### Main Window
- **Live Feed**: Real-time camera preview
- **Face Detection**: Detected faces shown with rectangles
- **Recognition**: Identified persons displayed with confidence score

### Face Enrollment
1. Click "Enroll New Face"
2. Enter person's name
3. Capture 5-10 photos from different angles
4. Click "Complete Enrollment"

### Settings
1. Configure camera source (Webcam/IP Camera)
2. Set recognition confidence threshold
3. Manage database
4. Configure logging

## API Documentation

### LuxandFaceEngine
```cpp
class LuxandFaceEngine {
public:
    bool initialize(const QString &licenseKey);
    bool detectFace(const QImage &image, Face &face);
    bool extractFaceTemplate(const QImage &image, QByteArray &template);
    bool matchFace(const QByteArray &t1, const QByteArray &t2, double &similarity);
};
```

### CameraManager
```cpp
class CameraManager {
public:
    bool startWebcam(int index);
    bool startIPCamera(const QString &rtspUrl);
    void stop();
    QImage getFrame();
    void recordVideo(const QString &filepath);
};
```

### FaceDatabase
```cpp
class FaceDatabase {
public:
    bool addFace(const QString &name, const QByteArray &template, const QImage &photo);
    bool findFace(const QByteArray &template, QString &name, double &confidence);
    QList<FaceRecord> getAllFaces();
    bool removeFace(const QString &name);
};
```

## Performance Optimization

- Multi-threaded face recognition on dedicated worker thread
- GPU acceleration support (CUDA if available)
- Optimized template matching
- Efficient image preprocessing
- Memory pooling for large video processing

## Troubleshooting

### Luxand License Error
```
Error: Invalid or expired license key
Solution: Update license_key in app_config.ini
```

### IP Camera Connection Failed
```
Error: Cannot connect to RTSP stream
Solution:
1. Verify IP camera is online
2. Check RTSP URL format: rtsp://IP:PORT/stream
3. Verify network connectivity
```

### Low Recognition Accuracy
```
Solution:
1. Re-enroll faces with better lighting
2. Use 10+ photos from different angles
3. Increase confidence_threshold in config
```

## License

This project is provided as-is for educational and commercial use.
Luxand FaceSDK requires its own commercial license.

## Support

For issues and feature requests, please visit:
https://github.com/proteus1208/FaceRecognition-Qt-Luxand-Professional/issues

## References

- Luxand FaceSDK Documentation: https://www.luxand.com/doc.php
- Qt Documentation: https://doc.qt.io/
- RTSP Streaming: https://tools.ietf.org/html/rfc2326
