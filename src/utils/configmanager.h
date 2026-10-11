#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QVariant>

class ConfigManager
{
public:
    explicit ConfigManager(const QString &filePath);
    ~ConfigManager();

    bool load();
    bool save();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);

private:
    QString filePath_;
};

#endif // CONFIGMANAGER_H
