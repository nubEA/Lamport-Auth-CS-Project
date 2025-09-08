#include "ConfigManager.hpp"

ConfigManager::ConfigManager(const QString& filePath) {
    loadConfig(filePath);
}

void ConfigManager::loadConfig(const QString& filePath) {
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        qWarning() << "JSON file could not be opened:" << filePath;
        return;
    }

    QByteArray data = configFile.readAll();
    configFile.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at" << parseError.offset << ":"
                   << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "Config file does not contain a valid JSON object";
        return;
    }

    configObj = doc.object();
}

QString ConfigManager::getAliceIP() const {
    return configObj.value("aliceIP").toString();
}

QString ConfigManager::getBobIP() const {
    return configObj.value("bobIP").toString();
}

quint16 ConfigManager::getAlicePort() const {
    return static_cast<quint16>(configObj.value("alicePort").toInt());
}

quint16 ConfigManager::getBobPort() const {
    return static_cast<quint16>(configObj.value("bobPort").toInt());
}

int ConfigManager::getSleepTime() const {
    return configObj.value("sleepDuration").toInt();
}

int ConfigManager::getNumberOfIterations() const {
    return configObj.value("numRounds").toInt();
}
