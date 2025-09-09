#include "Client.hpp"
#include <QDataStream>

Client::Client(const QString& filePath, QObject* parent)
    : QObject(parent), m_config(filePath)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    startClient();
}

Client::~Client() {
    stopClient();
}

bool Client::isConnected() const {
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

void Client::startClient() {
    QHostAddress aliceIP(m_config.getAliceIP());
    quint16 alicePort = m_config.getAlicePort();
    emit newLogMessage("Client: Connecting to " + aliceIP.toString() + ":" + QString::number(alicePort));
    m_socket->connectToHost(aliceIP, alicePort);
}

void Client::stopClient() {
    if (isConnected()) {
        m_socket->disconnectFromHost();
    }
}

void Client::onConnected() {
    emit connected();
    emit newLogMessage("Client: Connection successful.");
    int len = m_config.getNumberOfIterations();
    std::string seed = CryptoUtils::generateRandomSeed(32);
    m_auth.initChain(seed, len);
    emit newLogMessage("Client: Seed (hex): " + QString::fromStdString(CryptoUtils::convertToHex(seed)));
    emit newLogMessage("Client: Sending final hash h_n to server...");
    std::string hn = m_auth.getLastHash();
    m_socket->write(QByteArray::fromStdString(hn));
    m_socket->flush();
}

void Client::onDisconnected() {
    emit newLogMessage("Client: Disconnected from server.");
    emit disconnected();
}

void Client::onReadyRead() {
    QByteArray content = m_socket->readAll();
    int challengeNumber = getNumberFromQByteArray(content);
    if (challengeNumber <= 0) return;
    emit newLogMessage("Client: Received challenge #" + QString::number(challengeNumber));
    std::string response = m_auth.getOTPForChallenge(challengeNumber);
    emit newLogMessage("Client: Sending response h_" + QString::number(m_config.getNumberOfIterations() - challengeNumber));
    m_socket->write(QByteArray::fromStdString(response));
    m_socket->flush();
}

int Client::getNumberFromQByteArray(const QByteArray& input) {
    QDataStream ss(input);
    qint32 number;
    ss >> number;
    return number;
}

QByteArray Client::IntToArray(qint32 source) {
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}