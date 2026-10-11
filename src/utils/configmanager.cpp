#include "configmanager.h"
#include "logger.h"

#include <QSettings>

ConfigManager::ConfigManager(const QString &filePath)
    : filePath_(filePath)
{
}

ConfigManager::~ConfigManager()
{
}

bool ConfigManager::load()
{
    Logger::info(QString("Loading config from: %1").arg(filePath_));
    return true;
}

bool ConfigManager::save()
{
    Logger::info(QString("Saving config to: %1").arg(filePath_));
    return true;
}

QVariant ConfigManager::value(const QString &key, const QVariant &defaultValue) const
{
    QSettings settings(filePath_, QSettings::IniFormat);
    return settings.value(key, defaultValue);
}

void ConfigManager::setValue(const QString &key, const QVariant &value)
{
    QSettings settings(filePath_, QSettings::IniFormat);
    settings.setValue(key, value);
}
