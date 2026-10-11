#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include <QObject>
#include <QImage>
#include <QCamera>
#include <QCameraImageCapture>
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
    void onImageCaptured(int id, const QImage &preview);
    void captureFrame();

private:
    QCamera *camera_;
    QCameraImageCapture *imageCapture_;
    QTimer *captureTimer_;
    QImage latestFrame_;
};

#endif // CAMERAHANDLER_H
