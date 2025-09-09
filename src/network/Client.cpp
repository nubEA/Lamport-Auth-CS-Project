#include "Client.hpp"
#include <iostream>

Client::Client(const QString& filePath, QObject* parent)
    : QObject(parent), config(filePath)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    startClient();
}

Client::~Client() {
    stopClient();  // graceful cleanup
}

void Client::startClient() {
    QHostAddress aliceIP(config.getAliceIP());
    quint16 alicePort = config.getAlicePort();

    std::cout << "Client: Connecting to Alice at " << aliceIP.toString().toStdString() << ":" << alicePort << std::endl;
    socket->connectToHost(aliceIP, alicePort);
}

void Client::stopClient() {
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected(1000);
    }
}

QByteArray Client::IntToArray(qint32 source)
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void Client::onConnected() {
    int len = config.getNumberOfIterations();

    std::string seed = CryptoUtils::generateRandomSeed(32);
    auth.initChain(seed, len);

    std::cout << "Client: Seed (hex): " << CryptoUtils::convertToHex(seed) << std::endl;

    std::string hn = auth.getLastHash();
    QByteArray content = QByteArray::fromStdString(hn);
    socket->write(content);
    socket->flush();
}

int Client::getNumberFromQByteArray(const QByteArray& input) {
    QDataStream ss(input);
    qint32 number;
    ss >> number;
    return number;
}

void Client::onReadyRead() {
    QByteArray content = socket->readAll();

    int challengeNumber = getNumberFromQByteArray(content);
    if (challengeNumber < 0) return;

    std::cout << "Client: Received challenge #" << challengeNumber << std::endl;

    std::string challengeResponse = auth.getOTPForChallenge(challengeNumber);
    QByteArray challengeResponseArray = QByteArray::fromStdString(challengeResponse);

    socket->write(challengeResponseArray);
    socket->flush();
}

void Client::onDisconnected() {
    std::cout << "Client: Disconnected from Alice." << std::endl;
    emit disconnected();
}