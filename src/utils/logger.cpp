#include "logger.h"
#include <QDebug>
#include <QDateTime>

void Logger::info(const QString &message)
{
    qInfo() << QDateTime::currentDateTime().toString(Qt::ISODate) << "[INFO]" << message;
}

void Logger::warning(const QString &message)
{
    qWarning() << QDateTime::currentDateTime().toString(Qt::ISODate) << "[WARNING]" << message;
}

void Logger::error(const QString &message)
{
    qCritical() << QDateTime::currentDateTime().toString(Qt::ISODate) << "[ERROR]" << message;
}

void Logger::debug(const QString &message)
{
#ifdef DEBUG_MODE
    qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << "[DEBUG]" << message;
#else
    Q_UNUSED(message);
#endif
}
