#ifndef SERVER_HPP
#define SERVER_HPP

#include "ConfigManager.hpp"
#include "LamportAuth.hpp"
#include <QTcpServer>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>
#include <QByteArray>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(const QString& filePath);
    ~Server();

    void startServer();
    void stopServer();
    void sendChallenge();
    QByteArray IntToArray(qint32 source);

public slots:
    void handleNewConnection();
    void receiveResponse();
    void clientDisconnected();

private:
    QTcpSocket* clientSocket{};
    LamportAuth auth{};
    ConfigManager config;
    int currentIteration{1};
    QTimer* challengeTimer{};
};

#endif // SERVER_HPP