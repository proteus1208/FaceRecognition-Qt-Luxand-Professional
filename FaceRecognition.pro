QT += core gui widgets multimedia sql network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += warn_on

TARGET = FaceRecognition
TEMPLATE = app

VERSION = 1.0.0
DEFINES += APP_VERSION=\"$$VERSION\"

# Output paths
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc
UI_DIR = $$PWD/build/ui

# Include paths
INCLUDEPATH += $$PWD/src

# Source files
SOURCES += \
    src/main.cpp \
    src/ui/mainwindow.cpp \
    src/core/luxandfaceengine.cpp \
    src/core/camerahandler.cpp \
    src/core/ipcamerahandler.cpp \
    src/core/facedatabase.cpp \
    src/core/recognitionworker.cpp \
    src/utils/logger.cpp \
    src/utils/configmanager.cpp

HEADERS += \
    src/ui/mainwindow.h \
    src/core/luxandfaceengine.h \
    src/core/camerahandler.h \
    src/core/ipcamerahandler.h \
    src/core/facedatabase.h \
    src/core/recognitionworker.h \
    src/core/recognitionresult.h \
    src/utils/logger.h \
    src/utils/configmanager.h

FORMS += src/ui/mainwindow.ui

RESOURCES += resources/resources.qrc

# Luxand FaceSDK configuration (Windows example)
win32 {
    LUXAND_PATH = C:/Program Files/Luxand/FaceSDK
    INCLUDEPATH += $$LUXAND_PATH/include
    LIBS += -L$$LUXAND_PATH/lib -lfacesdk
}

# Linux configuration
linux {
    LUXAND_PATH = /opt/luxand/facesdk
    INCLUDEPATH += $$LUXAND_PATH/include
    LIBS += -L$$LUXAND_PATH/lib -lfacesdk
}

# Compiler flags
CONFIG(debug, debug|release) {
    DEFINES += DEBUG_MODE
} else {
    QMAKE_CXXFLAGS_RELEASE += -O2
}
