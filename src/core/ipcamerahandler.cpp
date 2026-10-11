#include "ipcamerahandler.h"
#include "../utils/logger.h"

IPCameraHandler::IPCameraHandler(QObject *parent)
    : QObject(parent)
{
}

IPCameraHandler::~IPCameraHandler()
{
    disconnect();
}

bool IPCameraHandler::connect(const QString &rtspUrl)
{
    if (rtspUrl.isEmpty()) {
        Logger::error("RTSP URL is empty");
        emit errorOccurred("RTSP URL is empty");
        return false;
    }

    rtspUrl_ = rtspUrl;
    Logger::info(QString("Connecting to IP camera: %1").arg(rtspUrl));
    emit connectionStatusChanged(true);
    return true;
}

void IPCameraHandler::disconnect()
{
    rtspUrl_.clear();
    latestFrame_ = QImage();
    emit connectionStatusChanged(false);
    Logger::info("Disconnected from IP camera");
}
