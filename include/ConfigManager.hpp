#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QString>
#include <QDebug>

class ConfigManager {
private:
    QJsonObject configObj;
    void loadConfig(const QString& filePath);

public:
    ConfigManager(const QString& filePath);

    QString getAliceIP() const;
    QString getBobIP() const;
    quint16 getBobPort() const;
    quint16 getAlicePort() const;
    int getSleepTime() const;
    int getNumberOfIterations() const;
};

#endif
