#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QTimer>

class CameraHandler;
class IPCameraHandler;
class FaceDatabase;
class LuxandFaceEngine;
class RecognitionWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartWebcamClicked();
    void onStartIPCameraClicked();
    void onEnrollClicked();
    void onSettingsClicked();
    void onFrameReady(const QImage &frame);
    void onRecognitionResult(const QString &name, double confidence);
    void updateFeedback();

private:
    void setupUI();
    void setupConnections();
    void createMenuBar();
    void initializeComponents();

    std::unique_ptr<CameraHandler> webcamHandler_;
    std::unique_ptr<IPCameraHandler> ipCameraHandler_;
    std::unique_ptr<FaceDatabase> database_;
    std::unique_ptr<LuxandFaceEngine> faceEngine_;
    std::unique_ptr<RecognitionWorker> recognitionWorker_;
    std::unique_ptr<QTimer> updateTimer_;

    QLabel *statusLabel_;
    QLabel *cameraLabel_;
    QListWidget *resultsList_;
    QPushButton *startWebcamBtn_;
    QPushButton *startIPCameraBtn_;
    QPushButton *enrollBtn_;
    QPushButton *settingsBtn_;
};

#endif // MAINWINDOW_H
