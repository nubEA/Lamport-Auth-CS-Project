#include "ConfigManager.hpp"
#include <iostream>

/**
 * @brief Constructs a ConfigManager and loads configuration from a file.
 * @param filePath Path to the JSON configuration file.
 */
ConfigManager::ConfigManager(const QString& filePath) {
    loadConfig(filePath);
}

/**
 * @brief Loads and parses a JSON configuration file.
 * @param filePath Path to the JSON file.
 */
void ConfigManager::loadConfig(const QString& filePath) {
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        // Log error if file cannot be opened
        std::cerr << "ConfigManager: JSON file could not be opened: " << filePath.toStdString() << std::endl;
        return;
    }

    // Read all data from the file
    QByteArray data = configFile.readAll();
    configFile.close();

    // Parse the JSON data
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    // Check for parsing errors
    if (parseError.error != QJsonParseError::NoError) {
        std::cerr << "ConfigManager: Parse error at " << parseError.offset << ": "
                   << parseError.errorString().toStdString() << std::endl;
        return;
    }

    // Ensure the root of the JSON is an object
    if (!doc.isObject()) {
        std::cerr << "ConfigManager: Config file does not contain a valid JSON object" << std::endl;
        return;
    }

    // Store the main JSON object
    configObj = doc.object();
}

// --- Getters for Configuration Values ---

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