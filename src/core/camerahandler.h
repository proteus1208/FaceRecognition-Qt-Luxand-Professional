#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include <QObject>
#include <QImage>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QTimer>

class CameraHandler : public QObject
{
    Q_OBJECT

public:
    explicit CameraHandler(QObject *parent = nullptr);
    ~CameraHandler();

    bool startWebcam(int cameraIndex = 0);
    void stop();
    QImage getLatestFrame() const { return latestFrame_; }

signals:
    void frameReady(const QImage &frame);
    void errorOccurred(const QString &error);

private slots:
    void onVideoFrameReceived(const QVideoFrame &frame);

private:
    QCamera *camera_;
    QMediaCaptureSession *captureSession_;
    QVideoSink *videoSink_;
    QImage latestFrame_;
};

#endif // CAMERAHANDLER_H
