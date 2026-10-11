QT += core gui multimedia sql network opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += warn_on

TARGET = FaceRecognition
TEMPLATE = app

# Application version
VERSION = 1.0.0
DEFINES += APP_VERSION=\"$$VERSION\"

# Output directory
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
    OBJECTS_DIR = $$PWD/build/debug
    MOC_DIR = $$PWD/build/debug/moc
    RCC_DIR = $$PWD/build/debug/rcc
    UI_DIR = $$PWD/build/debug/ui
} else {
    DESTDIR = $$PWD/bin/release
    OBJECTS_DIR = $$PWD/build/release
    MOC_DIR = $$PWD/build/release/moc
    RCC_DIR = $$PWD/build/release/rcc
    UI_DIR = $$PWD/build/release/ui
}

# Include paths
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/core
INCLUDEPATH += $$PWD/src/ui
INCLUDEPATH += $$PWD/src/camera
INCLUDEPATH += $$PWD/src/database
INCLUDEPATH += $$PWD/src/luxand
INCLUDEPATH += $$PWD/src/recognition

# Luxand FaceSDK paths (Windows)
win32 {
    LUXAND_SDK_PATH = C:/Program Files/Luxand/FaceSDK
    INCLUDEPATH += $$LUXAND_SDK_PATH/include
    LIBS += -L$$LUXAND_SDK_PATH/lib -lfacesdk
    # Copy DLL to output directory
    QMAKE_POST_LINK += $$quote(copy $$LUXAND_SDK_PATH/bin/facesdk.dll $$DESTDIR $$escape($$QMAKE_DIR_SEP))
}

# OpenCV paths (optional, for fallback face detection)
win32 {
    OPENCV_PATH = C:/opencv
    INCLUDEPATH += $$OPENCV_PATH/include
    LIBS += -L$$OPENCV_PATH/lib -lopencv_core4516 -lopencv_imgproc4516 -lopencv_videoio4516 -lopencv_face4516
}

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/core/application.cpp \
    src/ui/mainwindow.cpp \
    src/ui/enrollmentdialog.cpp \
    src/ui/settingsdialog.cpp \
    src/camera/camerahandler.cpp \
    src/camera/ipcamerahandler.cpp \
    src/camera/cameramanager.cpp \
    src/database/facedatabase.cpp \
    src/database/databasemanager.cpp \
    src/luxand/luxandfaceengine.cpp \
    src/luxand/facedetector.cpp \
    src/luxand/facerecognizer.cpp \
    src/recognition/recognitionworker.cpp \
    src/recognition/recognitionengine.cpp \
    src/utils/logger.cpp \
    src/utils/configmanager.cpp \
    src/utils/imageprocessor.cpp

# Header files
HEADERS += \
    src/mainwindow.h \
    src/core/application.h \
    src/core/constants.h \
    src/ui/mainwindow.h \
    src/ui/enrollmentdialog.h \
    src/ui/settingsdialog.h \
    src/camera/camerahandler.h \
    src/camera/ipcamerahandler.h \
    src/camera/cameramanager.h \
    src/database/facedatabase.h \
    src/database/databasemanager.h \
    src/database/facerecord.h \
    src/luxand/luxandfaceengine.h \
    src/luxand/facedetector.h \
    src/luxand/facerecognizer.h \
    src/recognition/recognitionworker.h \
    src/recognition/recognitionengine.h \
    src/recognition/recognitionresult.h \
    src/utils/logger.h \
    src/utils/configmanager.h \
    src/utils/imageprocessor.h

# UI files
FORMS += \
    src/ui/mainwindow.ui \
    src/ui/enrollmentdialog.ui \
    src/ui/settingsdialog.ui

# Resource files
RESOURCES += resources.qrc

# Compiler flags
CONFIG(debug, debug|release) {
    DEFINES += DEBUG_MODE
    QMAKE_CXXFLAGS += -Wall -Wextra
} else {
    DEFINES += QT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS_RELEASE += -O2
}
