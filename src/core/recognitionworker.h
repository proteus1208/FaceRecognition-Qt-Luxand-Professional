#ifndef RECOGNITIONWORKER_H
#define RECOGNITIONWORKER_H

#include <QObject>
#include <QImage>

class FaceDatabase;
class LuxandFaceEngine;

class RecognitionWorker : public QObject
{
    Q_OBJECT

public:
    explicit RecognitionWorker(QObject *parent = nullptr);
    ~RecognitionWorker();

    void setDatabase(FaceDatabase *database) { database_ = database; }
    void setFaceEngine(LuxandFaceEngine *engine) { faceEngine_ = engine; }

public slots:
    void processFrame(const QImage &frame);

signals:
    void recognitionResult(const QString &name, double confidence);

private:
    FaceDatabase *database_;
    LuxandFaceEngine *faceEngine_;
};

#endif // RECOGNITIONWORKER_H
