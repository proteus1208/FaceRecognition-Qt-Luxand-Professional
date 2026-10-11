#ifndef FACEDATABASE_H
#define FACEDATABASE_H

#include <QString>
#include <QVector>
#include <QByteArray>
#include <QDateTime>
#include <QImage>

struct FaceRecord {
    int id;
    QString name;
    QByteArray templateData;
    QByteArray photoData;
    QDateTime enrolledAt;
    double confidence;
};

class FaceDatabase
{
public:
    FaceDatabase();
    ~FaceDatabase();

    bool initialize();
    bool addFace(const QString &name, const QByteArray &templateData, const QImage &photo);
    bool removeFace(int id);
    QVector<FaceRecord> getAllFaces() const;
    bool matchFace(const QByteArray &templateData, QString &matchedName, double &confidence) const;

private:
    bool createSchema();
};

#endif // FACEDATABASE_H
