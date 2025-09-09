#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_config("/home/harshit/code/lamport-auth-qt/config.json") // Use a relative path
{
    ui->setupUi(this);
    this->setWindowTitle("Lamport Authenticator");
    ui->serverRadioButton->setChecked(true);
    ui->logDisplay->setReadOnly(true);
    updateUIState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- UI SLOTS ---

void MainWindow::on_connectButton_clicked()
{
    if (m_client || m_server) return;

    QString configPath = "/home/harshit/code/lamport-auth-qt/config.json"; // Define path once

    if (ui->serverRadioButton->isChecked()) {
        m_server = new Server(configPath, this);
        connect(m_server, &Server::clientConnected, this, &MainWindow::onBackendConnected);
        connect(m_server, &Server::clientDisconnected, this, &MainWindow::onBackendDisconnected);
        connect(m_server, &Server::newLogMessage, this, &MainWindow::appendLogMessage);
        connect(m_server, &Server::authProcessStarted, this, &MainWindow::onAuthProcessStarted);
        connect(m_server, &Server::authProcessStopped, this, &MainWindow::onAuthProcessStopped);
    } else {
        m_client = new Client(configPath, this);
        connect(m_client, &Client::connected, this, &MainWindow::onBackendConnected);
        connect(m_client, &Client::disconnected, this, &MainWindow::onBackendDisconnected);
        connect(m_client, &Client::newLogMessage, this, &MainWindow::appendLogMessage);
    }
    updateUIState();
}

void MainWindow::on_disconnectButton_clicked()
{
    if (m_client) m_client->stopClient();
    if (m_server) m_server->stopServer();
}

void MainWindow::on_startButton_clicked()
{
    if (m_server) {
        m_server->startAuthentication();
    } else {
        appendLogMessage("UI: Start button is only for the Server (Alice).");
    }
}

void MainWindow::on_stopButton_clicked()
{
    if (m_server) {
        m_server->stopAuthentication();
    } else {
        appendLogMessage("UI: Stop button is only for the Server (Alice).");
    }
}


// --- BACKEND SLOTS ---

void MainWindow::onBackendConnected()
{
    appendLogMessage("UI: Backend connection established!");
    ui->statusbar->showMessage("Status: Connected");
    updateUIState();
}

void MainWindow::onBackendDisconnected()
{
    appendLogMessage("UI: Backend has disconnected.");
    ui->statusbar->showMessage("Status: Disconnected");
    if (m_client) {
        m_client->deleteLater();
        m_client = nullptr;
    }
    if (m_server) {
        m_server->deleteLater();
        m_server = nullptr;
    }
    updateUIState();
}

void MainWindow::onAuthProcessStarted() {
    appendLogMessage("UI: Authentication process has started.");
    updateUIState();
}

void MainWindow::onAuthProcessStopped() {
    appendLogMessage("UI: Authentication process has stopped.");
    updateUIState();
}

void MainWindow::appendLogMessage(const QString &message)
{
    ui->logDisplay->appendPlainText(message);
    ui->logDisplay->verticalScrollBar()->setValue(ui->logDisplay->verticalScrollBar()->maximum());
}


// --- HELPER FUNCTION ---

void MainWindow::updateUIState()
{
    bool isListening = (m_server && m_server->isListening());
    bool isConnected = (m_client && m_client->isConnected()) || (m_server && m_server->hasActiveClient());
    bool isAuthRunning = (m_server && m_server->isAuthRunning());

    ui->connectButton->setEnabled(!isListening && !isConnected);
    ui->disconnectButton->setEnabled(isListening || isConnected);
    ui->serverRadioButton->setEnabled(!isListening && !isConnected);
    ui->clientRadioButton->setEnabled(!isListening && !isConnected);

    ui->startButton->setEnabled(isConnected && !isAuthRunning);
    ui->stopButton->setEnabled(isConnected && isAuthRunning);

    if (m_client) {
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
    }
}