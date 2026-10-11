#include "luxandfaceengine.h"
#include "../utils/logger.h"

LuxandFaceEngine::LuxandFaceEngine()
    : initialized_(false)
{
}

LuxandFaceEngine::~LuxandFaceEngine()
{
}

bool LuxandFaceEngine::initialize(const QString &licenseKey)
{
    licenseKey_ = licenseKey;

    if (licenseKey.isEmpty() || licenseKey == "DEMO") {
        Logger::warning("Luxand FaceSDK running in DEMO mode");
        initialized_ = true;
        return true;
    }

    // TODO: Integrate actual Luxand FaceSDK C++ API
    // FSDK_ActivateLibrary(licenseKey.toStdString().c_str());
    // FSDK_Initialize();

    initialized_ = true;
    Logger::info("Luxand FaceSDK initialized with license key");
    return true;
}

bool LuxandFaceEngine::detectFace(const QImage &image, QRect &faceRect)
{
    if (image.isNull()) {
        return false;
    }

    // TODO: Integrate actual Luxand face detection
    // For now, return a demo rectangle
    int w = image.width();
    int h = image.height();
    faceRect = QRect(w / 4, h / 4, w / 2, h / 2);

    return true;
}

bool LuxandFaceEngine::extractTemplate(const QImage &image, const QRect &faceRect, QByteArray &templateData)
{
    if (image.isNull() || faceRect.isNull()) {
        return false;
    }

    // TODO: Extract Luxand face template
    // For now, use image data as placeholder
    templateData = QByteArray(reinterpret_cast<const char*>(image.bits()), image.sizeInBytes());
    return true;
}

bool LuxandFaceEngine::matchFaces(const QByteArray &template1, const QByteArray &template2, double &similarity)
{
    if (template1.isEmpty() || template2.isEmpty()) {
        similarity = 0.0;
        return false;
    }

    // TODO: Use Luxand face matching
    // For now, compute basic similarity
    int matchCount = 0;
    int minSize = qMin(template1.size(), template2.size());

    for (int i = 0; i < minSize; ++i) {
        if (template1[i] == template2[i]) {
            matchCount++;
        }
    }

    similarity = static_cast<double>(matchCount) / minSize;
    return similarity > 0.5;
}
