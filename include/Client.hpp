#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ConfigManager.hpp"
#include "CryptoUtils.hpp"
#include "LamportAuth.hpp"
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>
#include <string>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(const QString& filePath, QObject* parent = nullptr);
    ~Client();

    void startClient();
    void stopClient();
    QByteArray IntToArray(qint32 source);
    int getNumberFromQByteArray(const QByteArray& input);
    
signals:
    void disconnected();

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* socket{};
    LamportAuth auth;
    ConfigManager config;
};

#endif // CLIENT_HPP