#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ConfigManager.hpp"
#include "Client.hpp"
#include "Server.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI Slots
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();

    // Backend Slots
    void onBackendConnected();
    void onBackendDisconnected();
    void onAuthProcessStarted();
    void onAuthProcessStopped();
    void appendLogMessage(const QString &message);

private:
    void updateUIState();

    Ui::MainWindow *ui;
    Client* m_client = nullptr;
    Server* m_server = nullptr;
    ConfigManager m_config;
};
#endif // MAINWINDOW_H