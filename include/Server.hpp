#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include "ConfigManager.hpp"
#include "LamportAuth.hpp"

/**
 * @class Server
 * @brief Manages the server-side logic for the Lamport authentication.
 *
 * This class handles listening for incoming client connections, sending
 * authentication challenges, and verifying the responses (OTPs) received
 * from the client.
 */
class Server : public QTcpServer
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Server object.
     * @param filePath The path to the configuration file.
     * @param parent The parent QObject, for memory management.
     */
    explicit Server(const QString& filePath, QObject *parent = nullptr);

    /**
     * @brief Destroys the Server object.
     */
    ~Server();

    // --- Public methods for UI control ---

    /**
     * @brief Stops the server and disconnects any active client.
     */
    void stopServer();

    /**
     * @brief Starts the periodic sending of authentication challenges.
     */
    void startAuthentication();

    /**
     * @brief Stops the authentication process.
     */
    void stopAuthentication();

    // --- Public methods for UI state checking ---

    /**
     * @brief Checks if the server is currently listening for connections.
     * @return True if listening, false otherwise.
     */
    bool isListening() const;

    /**
     * @brief Checks if there is an active client connection.
     * @return True if a client is connected, false otherwise.
     */
    bool hasActiveClient() const;

    /**
     * @brief Checks if the authentication challenge process is running.
     * @return True if the process is active, false otherwise.
     */
    bool isAuthRunning() const;

private slots:
    // --- Private slots for handling asynchronous events ---

    /**
     * @brief Handles a new incoming connection from a client.
     */
    void handleNewConnection();

    /**
     * @brief Sends the next authentication challenge to the client.
     */
    void sendChallenge();

    /**
     * @brief Receives and processes a response (OTP) from the client.
     */
    void receiveResponse();

    /**
     * @brief Handles the disconnection of the client.
     */
    void onClientDisconnected();

signals:
    // --- Signals to communicate with the UI (MainWindow) ---

    /**
     * @brief Emitted when a client successfully connects.
     */
    void clientConnected();

    /**
     * @brief Emitted when a client disconnects.
     */
    void clientDisconnected();

    /**
     * @brief Emitted to send a log message to the UI.
     * @param message The log message.
     */
    void newLogMessage(const QString &message);

    /**
     * @brief Emitted when the authentication process starts.
     */
    void authProcessStarted();

    /**
     * @brief Emitted when the authentication process stops.
     */
    void authProcessStopped();

private:
    // --- Private helper methods and member variables ---

    /**
     * @brief Initializes the server and starts listening on the configured port.
     */
    void startServer();

    /**
     * @brief Serializes an integer into a QByteArray for network transmission.
     * @param source The integer to serialize.
     * @return The resulting QByteArray.
     */
    QByteArray IntToArray(qint32 source);

    QTcpSocket* m_clientSocket = nullptr; ///< Socket for the connected client.
    ConfigManager m_config;               ///< Manages configuration data.
    LamportAuth m_auth;                   ///< Handles Lamport authentication logic.
    QTimer* m_challengeTimer = nullptr;   ///< Timer for sending challenges periodically.
    int m_currentIteration = 1;           ///< Tracks the current authentication iteration/challenge number.
};

#endif // SERVER_HPP