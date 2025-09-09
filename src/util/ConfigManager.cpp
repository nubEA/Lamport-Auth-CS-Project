#include "ConfigManager.hpp"
#include <iostream>

ConfigManager::ConfigManager(const QString& filePath) {
    loadConfig(filePath);
}

void ConfigManager::loadConfig(const QString& filePath) {
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        std::cerr << "ConfigManager: JSON file could not be opened: " << filePath.toStdString() << std::endl;
        return;
    }

    QByteArray data = configFile.readAll();
    configFile.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        std::cerr << "ConfigManager: Parse error at " << parseError.offset << ": "
                   << parseError.errorString().toStdString() << std::endl;
        return;
    }

    if (!doc.isObject()) {
        std::cerr << "ConfigManager: Config file does not contain a valid JSON object" << std::endl;
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
    return configObj.value("numberOfIterations").toInt();
}