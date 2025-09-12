#include "Client.hpp"
#include <QDataStream>

/**
 * @brief Constructs a Client object.
 * @param filePath Path to the configuration file.
 * @param parent The parent QObject.
 */
Client::Client(const QString& filePath, QObject* parent)
    : QObject(parent), m_config(filePath)
{
    // Initialize the TCP socket
    m_socket = new QTcpSocket(this);
    // Connect socket signals to the client's slots
    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    // Attempt to connect to the server
    startClient();
}

/**
 * @brief Destructor for the Client.
 */
Client::~Client() {
    stopClient();
}

/**
 * @brief Checks if the client is currently connected to the server.
 * @return True if connected, false otherwise.
 */
bool Client::isConnected() const {
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

/**
 * @brief Initiates a connection to the server using settings from the config file.
 */
void Client::startClient() {
    QHostAddress aliceIP(m_config.getAliceIP());
    quint16 alicePort = m_config.getAlicePort();
    emit newLogMessage("Client: Connecting to " + aliceIP.toString() + ":" + QString::number(alicePort));
    m_socket->connectToHost(aliceIP, alicePort);
}

/**
 * @brief Disconnects the client from the server.
 */
void Client::stopClient() {
    if (isConnected()) {
        m_socket->disconnectFromHost();
    }
}

/**
 * @brief Slot called upon successful connection to the server.
 * Generates the hash chain and sends the final hash (h_n) to the server.
 */
void Client::onConnected() {
    emit connected();
    emit newLogMessage("Client: Connection successful.");
    // Generate the Lamport hash chain
    int len = m_config.getNumberOfIterations();
    std::string seed = CryptoUtils::generateRandomSeed(32);
    m_auth.initChain(seed, len);
    emit newLogMessage("Client: Seed (hex): " + QString::fromStdString(CryptoUtils::convertToHex(seed)));
    
    // Send the last hash of the chain (h_n) to the server for setup
    emit newLogMessage("Client: Sending final hash h_n to server...");
    std::string hn = m_auth.getLastHash();
    m_socket->write(QByteArray::fromStdString(hn));
    m_socket->flush();
}

/**
 * @brief Slot called when disconnected from the server.
 */
void Client::onDisconnected() {
    emit newLogMessage("Client: Disconnected from server.");
    emit disconnected();
}

/**
 * @brief Slot called when data is received from the server (a challenge).
 * Responds with the appropriate one-time password (OTP).
 */
void Client::onReadyRead() {
    QByteArray content = m_socket->readAll();
    // Deserialize the challenge number from the byte array
    int challengeNumber = getNumberFromQByteArray(content);
    if (challengeNumber <= 0) return; // Ignore invalid challenges
    
    emit newLogMessage("Client: Received challenge #" + QString::number(challengeNumber));
    
    // Get the correct OTP from the LamportAuth logic
    std::string response = m_auth.getOTPForChallenge(challengeNumber);
    emit newLogMessage("Client: Sending response h_" + QString::number(m_config.getNumberOfIterations() - challengeNumber));
    
    // Send the OTP back to the server
    m_socket->write(QByteArray::fromStdString(response));
    m_socket->flush();
}

/**
 * @brief Deserializes a QByteArray into a 32-bit integer.
 * @param input The QByteArray received from the network.
 * @return The deserialized integer.
 */
int Client::getNumberFromQByteArray(const QByteArray& input) {
    QDataStream ss(input);
    qint32 number;
    ss >> number;
    return number;
}

/**
 * @brief Serializes a 32-bit integer into a QByteArray for network transmission.
 * @param source The integer to serialize.
 * @return The resulting QByteArray.
 */
QByteArray Client::IntToArray(qint32 source) {
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}