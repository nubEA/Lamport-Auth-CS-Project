#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include "ConfigManager.hpp"
#include "LamportAuth.hpp"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    // ✅ MODIFIED: Added QObject *parent parameter
    explicit Server(const QString& filePath, QObject *parent = nullptr);
    ~Server();

    // Public methods for UI control
    void stopServer();
    void startAuthentication();
    void stopAuthentication();

    // Public methods for UI state checking
    bool isListening() const;
    bool hasActiveClient() const;
    bool isAuthRunning() const;

private slots:
    void handleNewConnection();
    void sendChallenge();
    void receiveResponse();
    void onClientDisconnected();

signals:
    void clientConnected();
    void clientDisconnected();
    void newLogMessage(const QString &message);
    void authProcessStarted();
    void authProcessStopped();

private:
    void startServer();
    QByteArray IntToArray(qint32 source);

    QTcpSocket* m_clientSocket = nullptr;
    ConfigManager m_config;
    LamportAuth m_auth;
    QTimer* m_challengeTimer = nullptr;
    int m_currentIteration = 1;
};

#endif // SERVER_HPP