#include "camerahandler.h"
#include "../utils/logger.h"

#include <QCameraInfo>
#include <QDebug>
#include <QVideoFrame>

CameraHandler::CameraHandler(QObject *parent)
    : QObject(parent),
      camera_(nullptr),
      captureSession_(nullptr),
      videoSink_(nullptr)
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
    captureSession_ = new QMediaCaptureSession(this);
    captureSession_->setCamera(camera_);

    videoSink_ = new QVideoSink(this);
    captureSession_->setVideoOutput(videoSink_);

    connect(videoSink_, &QVideoSink::videoFrameReceived,
            this, &CameraHandler::onVideoFrameReceived);

    camera_->start();

    Logger::info(QString("Webcam %1 started").arg(cameraIndex));
    return true;
}

void CameraHandler::stop()
{
    if (camera_) {
        camera_->stop();
        camera_->deleteLater();
        camera_ = nullptr;
    }

    if (captureSession_) {
        captureSession_->deleteLater();
        captureSession_ = nullptr;
    }

    if (videoSink_) {
        videoSink_->deleteLater();
        videoSink_ = nullptr;
    }

    Logger::info("Camera stopped");
}

void CameraHandler::onVideoFrameReceived(const QVideoFrame &frame)
{
    QVideoFrame clonedFrame = frame;
    clonedFrame.map(QVideoFrame::ReadOnly);
    
    if (clonedFrame.isValid()) {
        latestFrame_ = clonedFrame.toImage();
        emit frameReady(latestFrame_);
    }
}
