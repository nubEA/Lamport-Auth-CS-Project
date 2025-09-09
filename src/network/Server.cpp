#include "Server.hpp"
#include <QDataStream>

Server::Server(const QString& filePath, QObject *parent)
    : QTcpServer(parent), m_config(filePath)
{
    startServer();
}

Server::~Server(){
    stopServer();
}

bool Server::isListening() const {
    return QTcpServer::isListening();
}

bool Server::hasActiveClient() const {
    return m_clientSocket != nullptr && m_clientSocket->state() == QAbstractSocket::ConnectedState;
}

bool Server::isAuthRunning() const {
    return m_challengeTimer != nullptr && m_challengeTimer->isActive();
}

void Server::startServer()
{
    quint16 serverPort = m_config.getAlicePort();
    QHostAddress serverIP(m_config.getAliceIP());
    if(!this->listen(serverIP, serverPort)) {
        emit newLogMessage("Server: Error - Could not start listening on port " + QString::number(serverPort));
        return;
    }
    emit newLogMessage("Server: Started, listening on port " + QString::number(serverPort));
    connect(this, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

void Server::stopServer() {
    stopAuthentication();
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

void Server::startAuthentication() {
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
    m_challengeTimer = new QTimer(this);
    connect(m_challengeTimer, &QTimer::timeout, this, &Server::sendChallenge);
    m_challengeTimer->start(m_config.getSleepTime() * 1000);
    emit authProcessStarted();
}

void Server::stopAuthentication() {
    if (isAuthRunning()) {
        m_challengeTimer->stop();
        m_challengeTimer->deleteLater();
        m_challengeTimer = nullptr;
        m_currentIteration = 1;
        emit newLogMessage("Server: Authentication process stopped by user.");
        emit authProcessStopped();
    }
}

void Server::handleNewConnection()
{
    if (hasActiveClient()) {
        this->nextPendingConnection()->disconnectFromHost();
        return;
    }
    m_clientSocket = this->nextPendingConnection();
    if(m_clientSocket) {
        connect(m_clientSocket, &QTcpSocket::readyRead, this, &Server::receiveResponse);
        connect(m_clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        emit newLogMessage("Server: New connection from: " + m_clientSocket->peerAddress().toString());
        emit clientConnected();
    }
}

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

void Server::onClientDisconnected() {
    emit newLogMessage("Server: Client has disconnected.");
    stopAuthentication();
    if (m_clientSocket) {
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
    emit clientDisconnected();
}

void Server::receiveResponse(){
    if(!hasActiveClient()) return;
    QByteArray content = m_clientSocket->readAll();
    std::string latestHash = content.toStdString();

    if(m_auth.getLastVerifiedHash().empty()){
        m_auth.setLastHash(latestHash);
        emit newLogMessage("Server: Received initial hash (h_n). Ready to start authentication.");
    } else {
        bool ok = m_auth.verifyOTP(latestHash);
        emit newLogMessage("Server: Verification Result: " + QString(ok ? "Success" : "Failure"));
        if(!ok) {
            emit newLogMessage("Server: Verification failed. Terminating connection.");
            m_clientSocket->disconnectFromHost();
        }
    }
}

QByteArray Server::IntToArray(qint32 source) {
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}