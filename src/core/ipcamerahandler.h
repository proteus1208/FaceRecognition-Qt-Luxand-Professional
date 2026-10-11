#ifndef IPCAMERAHANDLER_H
#define IPCAMERAHANDLER_H

#include <QObject>
#include <QString>
#include <QImage>

class IPCameraHandler : public QObject
{
    Q_OBJECT

public:
    explicit IPCameraHandler(QObject *parent = nullptr);
    ~IPCameraHandler();

    bool connect(const QString &rtspUrl);
    void disconnect();
    QImage getLatestFrame() const { return latestFrame_; }

signals:
    void frameReady(const QImage &frame);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

private:
    QString rtspUrl_;
    QImage latestFrame_;
};

#endif // IPCAMERAHANDLER_H
