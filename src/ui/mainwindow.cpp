#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QPixmap>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include "core/camerahandler.h"
#include "core/ipcamerahandler.h"
#include "core/facedatabase.h"
#include "core/luxandfaceengine.h"
#include "core/recognitionworker.h"
#include "utils/logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createMenuBar();
    initializeComponents();
    setupConnections();

    Logger::info("MainWindow initialized");
}

MainWindow::~MainWindow()
{
    if (webcamHandler_) {
        webcamHandler_->stop();
    }
    if (ipCameraHandler_) {
        ipCameraHandler_->disconnect();
    }
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    startWebcamBtn_ = new QPushButton("Start Webcam");
    startIPCameraBtn_ = new QPushButton("Start IP Camera");
    enrollBtn_ = new QPushButton("Enroll Face");
    settingsBtn_ = new QPushButton("Settings");

    buttonLayout->addWidget(startWebcamBtn_);
    buttonLayout->addWidget(startIPCameraBtn_);
    buttonLayout->addWidget(enrollBtn_);
    buttonLayout->addWidget(settingsBtn_);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Camera and results layout
    QHBoxLayout *contentLayout = new QHBoxLayout();

    cameraLabel_ = new QLabel();
    cameraLabel_->setMinimumSize(640, 480);
    cameraLabel_->setStyleSheet("QLabel { background-color: #1a1a1a; color: white; border: 2px solid #333; }");
    cameraLabel_->setAlignment(Qt::AlignCenter);
    cameraLabel_->setText("Camera Feed");
    contentLayout->addWidget(cameraLabel_, 1);

    resultsList_ = new QListWidget();
    resultsList_->setMaximumWidth(300);
    contentLayout->addWidget(resultsList_);

    mainLayout->addLayout(contentLayout, 1);

    // Status label
    statusLabel_ = new QLabel("Status: Ready");
    statusLabel_->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 5px; }");
    mainLayout->addWidget(statusLabel_);

    setCentralWidget(centralWidget);
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *exitAction = fileMenu->addAction("E&xit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *helpMenu = menuBar->addMenu("&Help");
    QAction *aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, "About",
                                "Face Recognition System v1.0.0\n\n"
                                "A professional face recognition application\n"
                                "powered by Luxand FaceSDK.");
    });
}

void MainWindow::initializeComponents()
{
    database_ = std::make_unique<FaceDatabase>();
    if (!database_->initialize()) {
        Logger::error("Failed to initialize database");
        statusLabel_->setText("Status: Database initialization failed");
    } else {
        Logger::info("Database initialized successfully");
    }

    faceEngine_ = std::make_unique<LuxandFaceEngine>();
    if (!faceEngine_->initialize("DEMO")) {
        Logger::warning("Luxand FaceSDK not initialized. Running in demo mode.");
        statusLabel_->setText("Status: Demo mode (Luxand SDK not available)");
    } else {
        Logger::info("Luxand FaceSDK initialized");
        statusLabel_->setText("Status: Ready (Luxand SDK active)");
    }

    recognitionWorker_ = std::make_unique<RecognitionWorker>();
    recognitionWorker_->setDatabase(database_.get());
    recognitionWorker_->setFaceEngine(faceEngine_.get());

    updateTimer_ = std::make_unique<QTimer>();
    connect(updateTimer_.get(), &QTimer::timeout, this, &MainWindow::updateFeedback);
}

void MainWindow::setupConnections()
{
    connect(startWebcamBtn_, &QPushButton::clicked, this, &MainWindow::onStartWebcamClicked);
    connect(startIPCameraBtn_, &QPushButton::clicked, this, &MainWindow::onStartIPCameraClicked);
    connect(enrollBtn_, &QPushButton::clicked, this, &MainWindow::onEnrollClicked);
    connect(settingsBtn_, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);

    if (recognitionWorker_) {
        connect(recognitionWorker_.get(), QOverload<const QString &, double>::of(&RecognitionWorker::recognitionResult),
                this, &MainWindow::onRecognitionResult);
    }
}

void MainWindow::onStartWebcamClicked()
{
    if (!webcamHandler_) {
        webcamHandler_ = std::make_unique<CameraHandler>();
        connect(webcamHandler_.get(), &CameraHandler::frameReady,
                this, &MainWindow::onFrameReady);
    }

    if (webcamHandler_->startWebcam(0)) {
        statusLabel_->setText("Status: Webcam started");
        Logger::info("Webcam started successfully");
        updateTimer_->start(200);
    } else {
        statusLabel_->setText("Status: Failed to start webcam");
        Logger::error("Failed to start webcam");
    }
}

void MainWindow::onStartIPCameraClicked()
{
    statusLabel_->setText("Status: IP Camera feature requires configuration");
    QMessageBox::information(this, "IP Camera",
                            "Configure your RTSP URL in Settings first.");
}

void MainWindow::onEnrollClicked()
{
    QMessageBox::information(this, "Enroll Face",
                            "Face enrollment feature coming soon.");
}

void MainWindow::onSettingsClicked()
{
    QMessageBox::information(this, "Settings",
                            "Settings dialog coming soon.");
}

void MainWindow::onFrameReady(const QImage &frame)
{
    if (!frame.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(frame);
        QPixmap scaled = pixmap.scaledToWidth(cameraLabel_->width(), Qt::SmoothTransformation);
        cameraLabel_->setPixmap(scaled);
    }
}

void MainWindow::onRecognitionResult(const QString &name, double confidence)
{
    QString result = QString("%1 (%.2f%%)").arg(name).arg(confidence * 100);
    resultsList_->addItem(result);

    if (resultsList_->count() > 10) {
        delete resultsList_->takeItem(0);
    }
}

void MainWindow::updateFeedback()
{
    if (webcamHandler_) {
        QImage frame = webcamHandler_->getLatestFrame();
        if (!frame.isNull()) {
            onFrameReady(frame);
            if (recognitionWorker_) {
                recognitionWorker_->processFrame(frame);
            }
        }
    }
}
