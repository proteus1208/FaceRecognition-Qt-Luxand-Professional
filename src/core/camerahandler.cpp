#include "camerahandler.h"
#include "../utils/logger.h"

#include <QCameraInfo>
#include <QDebug>

CameraHandler::CameraHandler(QObject *parent)
    : QObject(parent),
      camera_(nullptr),
      imageCapture_(nullptr),
      captureTimer_(nullptr)
{
}

CameraHandler::~CameraHandler()
{
    stop();
}

bool CameraHandler::startWebcam(int cameraIndex)
{
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    if (cameras.isEmpty()) {
        Logger::error("No cameras found");
        emit errorOccurred("No camera devices available");
        return false;
    }

    if (cameraIndex < 0 || cameraIndex >= cameras.size()) {
        Logger::error(QString("Camera index %1 out of range").arg(cameraIndex));
        return false;
    }

    camera_ = new QCamera(cameras.at(cameraIndex), this);
    imageCapture_ = new QCameraImageCapture(camera_, this);
    imageCapture_->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    connect(imageCapture_, QOverload<int, const QImage &>::of(&QCameraImageCapture::imageCaptured),
            this, &CameraHandler::onImageCaptured);

    camera_->setCaptureMode(QCamera::CaptureStillImage);
    camera_->start();

    captureTimer_ = new QTimer(this);
    connect(captureTimer_, &QTimer::timeout, this, &CameraHandler::captureFrame);
    captureTimer_->start(200); // 5 FPS

    Logger::info(QString("Webcam %1 started").arg(cameraIndex));
    return true;
}

void CameraHandler::stop()
{
    if (captureTimer_) {
        captureTimer_->stop();
        captureTimer_->deleteLater();
        captureTimer_ = nullptr;
    }

    if (camera_) {
        camera_->stop();
        camera_->deleteLater();
        camera_ = nullptr;
    }

    if (imageCapture_) {
        imageCapture_->deleteLater();
        imageCapture_ = nullptr;
    }

    Logger::info("Camera stopped");
}

void CameraHandler::onImageCaptured(int id, const QImage &preview)
{
    latestFrame_ = preview;
    emit frameReady(preview);
}

void CameraHandler::captureFrame()
{
    if (imageCapture_) {
        imageCapture_->capture();
    }
}
