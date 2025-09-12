#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QString>
#include <QDebug>

/**
 * @class ConfigManager
 * @brief Handles loading and accessing configuration settings from a JSON file.
 *
 * This class parses a JSON configuration file upon construction and provides
 * getter methods to retrieve specific configuration values like IP addresses,
 * ports, and other parameters.
 */
class ConfigManager {
private:
    QJsonObject configObj; ///< Stores the parsed JSON configuration object.

    /**
     * @brief Loads and parses the specified JSON configuration file.
     * @param filePath The path to the JSON file.
     */
    void loadConfig(const QString& filePath);

public:
    /**
     * @brief Constructs a ConfigManager object and loads the configuration.
     * @param filePath The path to the JSON configuration file.
     */
    ConfigManager(const QString& filePath);

    // --- Getters for configuration values ---

    QString getAliceIP() const;
    QString getBobIP() const;
    quint16 getBobPort() const;
    quint16 getAlicePort() const;
    int getSleepTime() const;
    int getNumberOfIterations() const;
};

#endif