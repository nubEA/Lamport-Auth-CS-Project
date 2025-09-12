#include "Server.hpp"
#include <QDataStream>

/**
 * @brief Constructs a Server object.
 * @param filePath Path to the configuration file.
 * @param parent The parent QObject.
 */
Server::Server(const QString& filePath, QObject *parent)
    : QTcpServer(parent), m_config(filePath)
{
    startServer();
}

/**
 * @brief Destructor for the Server. Ensures the server is stopped cleanly.
 */
Server::~Server(){
    stopServer();
}

/**
 * @brief Checks if the server is actively listening for incoming connections.
 * @return True if listening, false otherwise.
 */
bool Server::isListening() const {
    return QTcpServer::isListening();
}

/**
 * @brief Checks if there is an active and connected client.
 * @return True if a client is connected, false otherwise.
 */
bool Server::hasActiveClient() const {
    return m_clientSocket != nullptr && m_clientSocket->state() == QAbstractSocket::ConnectedState;
}

/**
 * @brief Checks if the authentication challenge-response process is currently running.
 * @return True if the challenge timer is active, false otherwise.
 */
bool Server::isAuthRunning() const {
    return m_challengeTimer != nullptr && m_challengeTimer->isActive();
}

/**
 * @brief Starts the TCP server to listen for incoming connections.
 */
void Server::startServer()
{
    quint16 serverPort = m_config.getAlicePort();
    QHostAddress serverIP(m_config.getAliceIP());
    // Attempt to listen on the configured IP and port
    if(!this->listen(serverIP, serverPort)) {
        emit newLogMessage("Server: Error - Could not start listening on port " + QString::number(serverPort));
        return;
    }
    emit newLogMessage("Server: Started, listening on port " + QString::number(serverPort));
    // Connect the newConnection signal to our handler
    connect(this, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

/**
 * @brief Stops the server, disconnects any client, and stops listening.
 */
void Server::stopServer() {
    stopAuthentication(); // Ensure the auth process is stopped
    if (m_clientSocket) {
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
    if (this->isListening()) {
        this->close();
        emit newLogMessage("Server: Listener stopped.");
    }
}

/**
 * @brief Starts the authentication process by initiating a timer to send challenges.
 */
void Server::startAuthentication() {
    // Pre-condition checks
    if (!hasActiveClient()) {
        emit newLogMessage("Server: Cannot start, no client connected.");
        return;
    }
    if (m_auth.getLastVerifiedHash().empty()){
        emit newLogMessage("Server: Cannot start, initial hash (h_n) not yet received.");
        return;
    }
    if (isAuthRunning()) {
        emit newLogMessage("Server: Authentication process is already running.");
        return;
    }

    emit newLogMessage("Server: Starting authentication process...");
    // Set up a timer to periodically call sendChallenge
    m_challengeTimer = new QTimer(this);
    connect(m_challengeTimer, &QTimer::timeout, this, &Server::sendChallenge);
    m_challengeTimer->start(m_config.getSleepTime() * 1000); // Convert seconds to ms
    emit authProcessStarted();
}

/**
 * @brief Stops the authentication process and cleans up the timer.
 */
void Server::stopAuthentication() {
    if (isAuthRunning()) {
        m_challengeTimer->stop();
        m_challengeTimer->deleteLater();
        m_challengeTimer = nullptr;
        m_currentIteration = 1; // Reset iteration counter
        emit newLogMessage("Server: Authentication process stopped by user.");
        emit authProcessStopped();
    }
}

/**
 * @brief Handles a new incoming client connection.
 * Accepts only one client at a time.
 */
void Server::handleNewConnection()
{
    // If a client is already connected, reject the new one
    if (hasActiveClient()) {
        this->nextPendingConnection()->disconnectFromHost();
        return;
    }
    // Accept the new connection
    m_clientSocket = this->nextPendingConnection();
    if(m_clientSocket) {
        connect(m_clientSocket, &QTcpSocket::readyRead, this, &Server::receiveResponse);
        connect(m_clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        emit newLogMessage("Server: New connection from: " + m_clientSocket->peerAddress().toString());
        emit clientConnected();
    }
}

/**
 * @brief Sends the next authentication challenge (iteration number) to the client.
 */
void Server::sendChallenge()
{
    if(hasActiveClient()) {
        if(m_currentIteration < m_config.getNumberOfIterations()) {
            emit newLogMessage("Server: Sent challenge #" + QString::number(m_currentIteration));
            m_clientSocket->write(IntToArray(m_currentIteration));
            m_clientSocket->flush();
            m_currentIteration++;
        } else {
            emit newLogMessage("Server: All challenges sent. Authentication complete.");
            stopAuthentication();
        }
    }
}

/**
 * @brief Slot called when the client disconnects. Cleans up resources.
 */
void Server::onClientDisconnected() {
    emit newLogMessage("Server: Client has disconnected.");
    stopAuthentication(); // Stop the auth process if it's running
    if (m_clientSocket) {
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
    emit clientDisconnected();
}

/**
 * @brief Slot called when data is received from the client.
 * Handles both the initial hash and subsequent OTP responses.
 */
void Server::receiveResponse(){
    if(!hasActiveClient()) return;
    QByteArray content = m_clientSocket->readAll();
    std::string latestHash = content.toStdString();

    // If this is the first hash received, store it as the initial h_n
    if(m_auth.getLastVerifiedHash().empty()){
        m_auth.setLastHash(latestHash);
        emit newLogMessage("Server: Received initial hash (h_n). Ready to start authentication.");
    } else {
        // Otherwise, verify the received OTP against the last known hash
        bool ok = m_auth.verifyOTP(latestHash);
        emit newLogMessage("Server: Verification Result: " + QString(ok ? "Success" : "Failure"));
        if(!ok) {
            emit newLogMessage("Server: Verification failed. Terminating connection.");
            m_clientSocket->disconnectFromHost();
        }
    }
}

/**
 * @brief Serializes a 32-bit integer into a QByteArray for network transmission.
 * @param source The integer to serialize.
 * @return The resulting QByteArray.
 */
QByteArray Server::IntToArray(qint32 source) {
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}