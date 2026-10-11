# Installation and Setup Guide

## Prerequisites

### Windows
- Visual Studio 2019 or 2022 Community Edition (free)
- Qt 6.2+ (download from qt.io)
- Luxand FaceSDK (download from luxand.com)
- CMake 3.16+ (optional)

### Linux (Ubuntu 20.04+)
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    qt6-base-dev \
    qt6-multimedia-dev \
    libsqlite3-dev \
    libopencv-dev
```

### macOS
```bash
brew install qt cmake
```

## Step-by-Step Installation

### 1. Download Luxand FaceSDK

**Windows:**
```
1. Visit https://www.luxand.com/download/
2. Download "FaceSDK for Windows"
3. Extract to: C:\Program Files\Luxand\FaceSDK
   (or custom location)
```

**Linux:**
```bash
# Download and extract
wget https://www.luxand.com/downloads/facesdk-linux.tar.gz
tar -xzf facesdk-linux.tar.gz
sudo mv facesdk /opt/luxand/facesdk
```

### 2. Update FaceRecognition.pro

Modify paths if not using default installation:

```pro
# For Windows
LUXAND_SDK_PATH = C:/Program Files/Luxand/FaceSDK  # Change this

# For Linux
# LUXAND_SDK_PATH = /opt/luxand/facesdk
```

### 3. Install Qt 6

**Download Qt 6 Online Installer:**
- Windows: https://download.qt.io/official_releases/online_installers/
- Select MSVC 2019/2022 compiler when installing

**Or use Package Manager:**
```bash
# Ubuntu
sudo apt-get install qt6-base-dev qt6-multimedia-dev

# macOS
brew install qt6
```

### 4. Clone Repository

```bash
git clone https://github.com/proteus1208/FaceRecognition-Qt-Luxand-Professional.git
cd FaceRecognition-Qt-Luxand-Professional
```

### 5. Build Project

**Option A: Using Qt Creator (Recommended)**
```
1. Open Qt Creator
2. File → Open File or Project
3. Select: FaceRecognition.pro
4. Configure with Qt 6.2+
5. Build → Build Project
6. Run (Green Play button)
```

**Option B: Command Line**

```bash
# Windows (MSVC)
qmake FaceRecognition.pro -spec win32-msvc
jom  # or: nmake

# Linux/macOS
qmake FaceRecognition.pro
make
```

### 6. First Run Configuration

**Create/Edit config file:**
```bash
cp resources/config/app_config.ini.example resources/config/app_config.ini
```

**Edit app_config.ini:**
```ini
[Application]
app_name=Face Recognition System
version=1.0.0
log_level=INFO

[Luxand]
license_key=YOUR_LICENSE_KEY  # Get from luxand.com

[Camera]
default_webcam_index=0
enable_webcam=true
enable_ip_camera=true
ip_camera_url=rtsp://192.168.1.100:554/stream
ip_camera_username=admin
ip_camera_password=password

[Database]
path=./database/faces.db
auto_backup=true

[Recognition]
confidence_threshold=0.85
min_face_size=20
max_face_size=500
detection_timeout_ms=5000
```

### 7. Initialize Database

```bash
# Create database directory
mkdir -p database

# Initialize database schema
sqlite3 database/faces.db < resources/database/schema.sql
```

**Or run from application:**
- Launch application
- Go to Settings → Database → Initialize

## Verification

### Check Luxand Installation
```cpp
// In main.cpp before app.exec()
LuxandFaceEngine engine;
if (!engine.initialize("YOUR_LICENSE_KEY")) {
    qCritical() << "Luxand initialization failed!";
    return 1;
}
qDebug() << "Luxand initialized successfully";
```

### Test Webcam
```
1. Run application
2. Camera tab should show live feed from default webcam
3. If blank, check camera permissions and permissions
```

### Test IP Camera
```
1. Settings → Cameras
2. Enable IP Camera
3. Enter RTSP URL: rtsp://CAMERA_IP:554/stream
4. Click "Connect"
5. Should show live feed
```

## Troubleshooting

### Build Errors

**Error: "LuxandFaceSDK.h not found"**
```
Solution:
1. Verify Luxand SDK installation path
2. Update INCLUDEPATH in .pro file
3. Check path separators (/ not \\)
```

**Error: "facesdk.lib not found" (Windows)**
```
Solution:
1. Download correct Windows SDK version
2. Verify lib/ directory exists in SDK
3. Update LIBS path in .pro file
```

**Error: Qt version mismatch**
```
Solution:
1. Use same Qt version in .pro as installed
2. Run: qmake --version
3. Verify Qt Creator uses correct kit
```

### Runtime Errors

**"Cannot load facesdk.dll" (Windows)**
```
Solution:
1. Copy facesdk.dll to executable directory
2. Or add SDK\bin to PATH environment variable
3. Check DLL dependencies: Dependencies.exe or dumpbin /imports
```

**"License invalid or expired"**
```
Solution:
1. Get free or commercial license from luxand.com
2. Update license_key in app_config.ini
3. Restart application
```

**Camera not working**
```
Solution:
1. Check camera permissions:
   - Windows: Settings → Privacy → Camera
   - Linux: Check /dev/video* permissions
   - macOS: System Preferences → Security & Privacy
2. Test camera with system tool first
3. Try different camera index (0, 1, 2...)
```

### Performance Issues

**High CPU usage**
```
Solution:
1. Lower camera resolution in settings
2. Increase detection timeout
3. Reduce face database size
4. Use GPU acceleration if available
```

**Slow face recognition**
```
Solution:
1. Increase confidence_threshold in config
2. Reduce database size (archive old faces)
3. Use dedicated GPU if available
4. Check CPU temperature
```

## Production Deployment

### Windows EXE Distribution

```bash
# Create deploy directory
mkdir -p deploy
cd deploy

# Copy executable
copy .\bin\release\FaceRecognition.exe .

# Deploy Qt libraries
windeployqt FaceRecognition.exe

# Copy Luxand DLL
copy "C:\Program Files\Luxand\FaceSDK\bin\facesdk.dll" .

# Copy resources
xcopy ..\resources . /E /I

# Create installer (optional)
# Use NSIS or InnoSetup
```

### Linux AppImage

```bash
# Install appimagetool
wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool-x86_64.AppImage

# Create AppImage
./appimagetool-x86_64.AppImage bin/release/FaceRecognition AppImage
```

### macOS App Bundle

```bash
macdeployqt bin/release/FaceRecognition.app -dmg
```

## Next Steps

1. **Configure for Your Environment**
   - Update app_config.ini with your camera URLs
   - Set Luxand license key
   - Customize UI if needed

2. **Train Face Database**
   - Enroll employee/user faces
   - Use 10+ photos per person
   - Test recognition accuracy

3. **Integrate with Existing Systems**
   - Export recognition events to API
   - Set up database backups
   - Configure logging and monitoring

## Support

- GitHub Issues: https://github.com/proteus1208/FaceRecognition-Qt-Luxand-Professional/issues
- Luxand Support: https://www.luxand.com/support/
- Qt Support: https://www.qt.io/support
