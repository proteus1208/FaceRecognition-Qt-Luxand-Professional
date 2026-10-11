#ifndef LUXANDFACEENGINE_H
#define LUXANDFACEENGINE_H

#include <QByteArray>
#include <QImage>
#include <QString>
#include <QRect>

struct Face {
    QRect rect;
    QByteArray templateData;
    double confidence = 0.0;
};

class LuxandFaceEngine
{
public:
    LuxandFaceEngine();
    ~LuxandFaceEngine();

    bool initialize(const QString &licenseKey);
    bool detectFace(const QImage &image, QRect &faceRect);
    bool extractTemplate(const QImage &image, const QRect &faceRect, QByteArray &templateData);
    bool matchFaces(const QByteArray &template1, const QByteArray &template2, double &similarity);
    bool isInitialized() const { return initialized_; }

private:
    bool initialized_;
    QString licenseKey_;
};

#endif // LUXANDFACEENGINE_H
