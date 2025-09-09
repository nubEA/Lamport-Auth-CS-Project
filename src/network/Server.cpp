#include "Server.hpp"
#include <iostream>

Server::Server(const QString& filePath): config(filePath)
{
    startServer();
}

Server::~Server(){
    stopServer();
}

void Server::stopServer() {
    if (challengeTimer) {
        challengeTimer->stop();
        challengeTimer->deleteLater();
        challengeTimer = nullptr;
    }

    if (clientSocket) {
        if (clientSocket->state() == QAbstractSocket::ConnectedState) {
            clientSocket->disconnectFromHost();
            clientSocket->waitForDisconnected(1000);
        }
        clientSocket->deleteLater();
        clientSocket = nullptr;
    }

    if (this->isListening()) {
        this->close();
        std::cout << "Server: Server stopped." << std::endl;
    }

    currentIteration = 1;
}


void Server::startServer()
{
    quint16 serverPort = config.getAlicePort();
    QHostAddress serverIP(config.getAliceIP());
    if(!this->listen(serverIP, serverPort))
    {
        std::cerr << "Server: Error occurred while creating the socket" << std::endl;
        return;
    }
    else{
        std::cout << "Server: Started, listening on port: " << serverPort << std::endl;
        connect(this, &QTcpServer::newConnection, this, &Server::handleNewConnection);
    }
}

QByteArray Server::IntToArray(qint32 source)
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}


void Server::handleNewConnection()
{
    clientSocket = this->nextPendingConnection();
    if(clientSocket)
    {
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::receiveResponse);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
        std::cout << "Server: New Connection from: " << clientSocket->peerAddress().toString().toStdString() << std::endl;

        challengeTimer = new QTimer(this);
        connect(challengeTimer, &QTimer::timeout, this, &Server::sendChallenge);
        challengeTimer->start(config.getSleepTime()*1000);
    }
}


void Server::sendChallenge()
{
    if(clientSocket && clientSocket->state() == QAbstractSocket::ConnectedState) {

        if(auth.getLastVerifiedHash().empty()) return;

        if(currentIteration < config.getNumberOfIterations()) {
            std::cout << "Server: Sent challenge #" << currentIteration << std::endl;
            clientSocket->write(IntToArray(currentIteration));
            clientSocket->flush();
            currentIteration++;
        } else {
            challengeTimer->stop();
            currentIteration = 1;
        }
    }
}


void Server::clientDisconnected() {
    if (clientSocket) {
        std::cout << "Server: Client disconnected: "
                 << clientSocket->peerAddress().toString().toStdString()
                 << ":" << clientSocket->peerPort() << std::endl;
        clientSocket->deleteLater();
        clientSocket = nullptr;
    }

    if (challengeTimer) {
        challengeTimer->stop();
        challengeTimer->deleteLater();
        challengeTimer = nullptr;
    }

    currentIteration = 1;
}

void Server::receiveResponse(){

    QByteArray content = clientSocket->readAll();
    std::string latestHash = content.toStdString();

    if(auth.getLastVerifiedHash().empty()){
        auth.setLastHash(latestHash);
        std::cout << "Server: Received initial hash (h_n)." << std::endl;
    }
    else{
        bool ok = auth.verifyOTP(latestHash);
        std::cout << "Server: Verification Result: " << (ok ? "Success" : "Failure") << std::endl;
        if(!ok)
        {
            std::cerr << "Server: Verification failed. Closing connection." << std::endl;

            if (challengeTimer) {
                challengeTimer->stop();
                challengeTimer->deleteLater();
                challengeTimer = nullptr;
            }

            if (clientSocket) {
                 clientSocket->disconnectFromHost();
                 clientSocket->deleteLater();
                 clientSocket = nullptr;
            }

            this->close();
        }
    }
}