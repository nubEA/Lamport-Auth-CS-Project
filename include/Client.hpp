#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include "ConfigManager.hpp"
#include "LamportAuth.hpp"
#include "CryptoUtils.hpp"

/**
 * @class Client
 * @brief Manages the client-side logic for Lamport authentication.
 *
 * This class handles connecting to the server, generating the Lamport hash chain,
 * sending the initial hash, and responding to authentication challenges with the
 * appropriate one-time passwords (OTPs).
 */
class Client : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Client object.
     * @param filePath The path to the configuration file.
     * @param parent The parent QObject, for memory management.
     */
    explicit Client(const QString& filePath, QObject* parent = nullptr);

    /**
     * @brief Destroys the Client object.
     */
    ~Client();

    /**
     * @brief Disconnects the client from the server.
     */
    void stopClient();

    /**
     * @brief Checks if the client is currently connected to the server.
     * @return True if connected, false otherwise.
     */
    bool isConnected() const;

private slots:
    // --- Private slots for handling socket events ---

    /**
     * @brief Handles a successful connection to the server.
     */
    void onConnected();

    /**
     * @brief Handles disconnection from the server.
     */
    void onDisconnected();

    /**
     * @brief Handles incoming data (challenges) from the server.
     */
    void onReadyRead();

signals:
    // --- Signals to communicate with the UI (MainWindow) ---

    /**
     * @brief Emitted when the client successfully connects to the server.
     */
    void connected();

    /**
     * @brief Emitted when the client disconnects from the server.
     */
    void disconnected();

    /**
     * @brief Emitted to send a log message to the UI.
     * @param message The log message.
     */
    void newLogMessage(const QString &message);

private:
    // --- Private helper methods and member variables ---

    /**
     * @brief Initiates the connection to the server.
     */
    void startClient();

    /**
     * @brief Serializes an integer into a QByteArray.
     * @param source The integer to serialize.
     * @return The resulting QByteArray.
     */
    QByteArray IntToArray(qint32 source);

    /**
     * @brief Deserializes a QByteArray into an integer.
     * @param input The QByteArray to deserialize.
     * @return The resulting integer.
     */
    int getNumberFromQByteArray(const QByteArray& input);

    QTcpSocket* m_socket;   ///< The TCP socket for communication with the server.
    ConfigManager m_config; ///< Manages configuration data.
    LamportAuth m_auth;     ///< Handles Lamport authentication logic.
};

#endif // CLIENT_HPP