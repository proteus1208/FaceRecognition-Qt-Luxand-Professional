#include "recognitionworker.h"
#include "facedatabase.h"
#include "luxandfaceengine.h"
#include "../utils/logger.h"

RecognitionWorker::RecognitionWorker(QObject *parent)
    : QObject(parent),
      database_(nullptr),
      faceEngine_(nullptr)
{
}

RecognitionWorker::~RecognitionWorker()
{
}

void RecognitionWorker::processFrame(const QImage &frame)
{
    if (!database_ || !faceEngine_ || frame.isNull()) {
        emit recognitionResult("", 0.0);
        return;
    }

    QRect faceRect;
    if (!faceEngine_->detectFace(frame, faceRect)) {
        emit recognitionResult("", 0.0);
        return;
    }

    QByteArray templateData;
    if (!faceEngine_->extractTemplate(frame, faceRect, templateData)) {
        emit recognitionResult("", 0.0);
        return;
    }

    QString matchedName;
    double confidence = 0.0;
    if (database_->matchFace(templateData, matchedName, confidence)) {
        Logger::info(QString("Recognized: %1 (confidence: %2)").arg(matchedName).arg(confidence));
        emit recognitionResult(matchedName, confidence);
    } else {
        emit recognitionResult("Unknown", 0.0);
    }
}
