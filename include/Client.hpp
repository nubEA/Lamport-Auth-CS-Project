#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include "ConfigManager.hpp"
#include "LamportAuth.hpp"
#include "CryptoUtils.hpp"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(const QString& filePath, QObject* parent = nullptr);
    ~Client();
    void stopClient();
    bool isConnected() const;

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();

signals:
    void connected();
    void disconnected();
    void newLogMessage(const QString &message);

private:
    void startClient();
    QByteArray IntToArray(qint32 source);
    int getNumberFromQByteArray(const QByteArray& input);

    QTcpSocket* m_socket;
    ConfigManager m_config;
    LamportAuth m_auth;
};

#endif // CLIENT_HPP