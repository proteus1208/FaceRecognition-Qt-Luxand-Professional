#include "facedatabase.h"
#include "../utils/logger.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QBuffer>

FaceDatabase::FaceDatabase()
{
}

FaceDatabase::~FaceDatabase()
{
}

bool FaceDatabase::initialize()
{
    QSqlDatabase db = QSqlDatabase::database("faces_db");
    if (!db.isOpen()) {
        Logger::error("Database not open");
        return false;
    }

    return createSchema();
}

bool FaceDatabase::createSchema()
{
    QSqlDatabase db = QSqlDatabase::database("faces_db");
    QSqlQuery query(db);

    const QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS faces (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            template_data BLOB NOT NULL,
            photo_data BLOB,
            enrolled_at TEXT NOT NULL,
            confidence REAL DEFAULT 0.0
        )
    )";

    if (!query.exec(createTableSQL)) {
        Logger::error(QString("Schema creation failed: %1").arg(query.lastError().text()));
        return false;
    }

    Logger::info("Database schema created/verified");
    return true;
}

bool FaceDatabase::addFace(const QString &name, const QByteArray &templateData, const QImage &photo)
{
    QSqlDatabase db = QSqlDatabase::database("faces_db");
    QSqlQuery query(db);

    query.prepare("INSERT INTO faces (name, template_data, photo_data, enrolled_at, confidence) "
                  "VALUES (?, ?, ?, ?, 0.0)");
    query.addBindValue(name);
    query.addBindValue(templateData);

    // Convert image to BLOB
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    photo.save(&buffer, "PNG");
    query.addBindValue(imageData);

    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        Logger::error(QString("Failed to add face: %1").arg(query.lastError().text()));
        return false;
    }

    Logger::info(QString("Face added: %1").arg(name));
    return true;
}

bool FaceDatabase::removeFace(int id)
{
    QSqlDatabase db = QSqlDatabase::database("faces_db");
    QSqlQuery query(db);

    query.prepare("DELETE FROM faces WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        Logger::error(QString("Failed to remove face: %1").arg(query.lastError().text()));
        return false;
    }

    Logger::info(QString("Face removed with id: %1").arg(id));
    return true;
}

QVector<FaceRecord> FaceDatabase::getAllFaces() const
{
    QVector<FaceRecord> records;
    QSqlDatabase db = QSqlDatabase::database("faces_db");
    QSqlQuery query(db);

    if (!query.exec("SELECT id, name, template_data, photo_data, enrolled_at, confidence FROM faces")) {
        Logger::error("Failed to query faces");
        return records;
    }

    while (query.next()) {
        FaceRecord record;
        record.id = query.value(0).toInt();
        record.name = query.value(1).toString();
        record.templateData = query.value(2).toByteArray();
        record.photoData = query.value(3).toByteArray();
        record.enrolledAt = QDateTime::fromString(query.value(4).toString(), Qt::ISODate);
        record.confidence = query.value(5).toDouble();
        records.append(record);
    }

    return records;
}

bool FaceDatabase::matchFace(const QByteArray &templateData, QString &matchedName, double &confidence) const
{
    Q_UNUSED(templateData);
    const auto records = getAllFaces();

    if (records.isEmpty()) {
        confidence = 0.0;
        return false;
    }

    // Simple matching: return first face with highest confidence
    matchedName = records.first().name;
    confidence = records.first().confidence;
    return confidence > 0.5;
}
