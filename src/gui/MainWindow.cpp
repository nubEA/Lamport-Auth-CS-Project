#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QScrollBar>

/**
 * @brief Constructs the main application window.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_config("/home/harshit/code/lamport-auth-qt/config.json") // Configuration manager instance
{
    // Set up the UI defined in the .ui file
    ui->setupUi(this);
    this->setWindowTitle("Lamport Authenticator");

    // Default to server mode on startup
    ui->serverRadioButton->setChecked(true);
    // Make the log display non-editable by the user
    ui->logDisplay->setReadOnly(true);
    // Set the initial state of all UI elements (e.g., enabled/disabled buttons)
    updateUIState();
}

/**
 * @brief Destructor for the MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

// --- UI SLOTS ---

/**
 * @brief Slot triggered when the 'Connect' button is clicked.
 * Initializes either a Server or Client instance based on the selected radio button.
 */
void MainWindow::on_connectButton_clicked()
{
    // Prevent creating new instances if one already exists
    if (m_client || m_server) return;

    QString configPath = "/home/harshit/code/lamport-auth-qt/config.json"; // Path to the configuration file

    // Check which mode is selected (Server or Client)
    if (ui->serverRadioButton->isChecked()) {
        // Create and configure a new Server instance
        m_server = new Server(configPath, this);
        connect(m_server, &Server::clientConnected, this, &MainWindow::onBackendConnected);
        connect(m_server, &Server::clientDisconnected, this, &MainWindow::onBackendDisconnected);
        connect(m_server, &Server::newLogMessage, this, &MainWindow::appendLogMessage);
        connect(m_server, &Server::authProcessStarted, this, &MainWindow::onAuthProcessStarted);
        connect(m_server, &Server::authProcessStopped, this, &MainWindow::onAuthProcessStopped);
    } else {
        // Create and configure a new Client instance
        m_client = new Client(configPath, this);
        connect(m_client, &Client::connected, this, &MainWindow::onBackendConnected);
        connect(m_client, &Client::disconnected, this, &MainWindow::onBackendDisconnected);
        connect(m_client, &Client::newLogMessage, this, &MainWindow::appendLogMessage);
    }
    // Refresh the UI to reflect the new state (e.g., disable connect button)
    updateUIState();
}

/**
 * @brief Slot triggered when the 'Disconnect' button is clicked.
 * Stops the active Server or Client.
 */
void MainWindow::on_disconnectButton_clicked()
{
    if (m_client) m_client->stopClient();
    if (m_server) m_server->stopServer();
}

/**
 * @brief Slot triggered when the 'Start Auth' button is clicked.
 * Initiates the authentication process on the server side.
 */
void MainWindow::on_startButton_clicked()
{
    if (m_server) {
        m_server->startAuthentication();
    } else {
        appendLogMessage("UI: Start button is only for the Server (Alice).");
    }
}

/**
 * @brief Slot triggered when the 'Stop Auth' button is clicked.
 * Stops the authentication process on the server side.
 */
void MainWindow::on_stopButton_clicked()
{
    if (m_server) {
        m_server->stopAuthentication();
    } else {
        appendLogMessage("UI: Stop button is only for the Server (Alice).");
    }
}


// --- BACKEND SLOTS ---

/**
 * @brief Slot triggered when the backend (Server or Client) establishes a connection.
 */
void MainWindow::onBackendConnected()
{
    appendLogMessage("UI: Backend connection established!");
    ui->statusbar->showMessage("Status: Connected");
    updateUIState();
}

/**
 * @brief Slot triggered when the backend (Server or Client) disconnects.
 * Cleans up the server/client instance.
 */
void MainWindow::onBackendDisconnected()
{
    appendLogMessage("UI: Backend has disconnected.");
    ui->statusbar->showMessage("Status: Disconnected");
    
    // Safely delete the client instance
    if (m_client) {
        m_client->deleteLater();
        m_client = nullptr;
    }
    // Safely delete the server instance
    if (m_server) {
        m_server->deleteLater();
        m_server = nullptr;
    }
    updateUIState();
}

/**
 * @brief Slot triggered when the server starts the authentication process.
 */
void MainWindow::onAuthProcessStarted() {
    appendLogMessage("UI: Authentication process has started.");
    updateUIState();
}

/**
 * @brief Slot triggered when the server stops the authentication process.
 */
void MainWindow::onAuthProcessStopped() {
    appendLogMessage("UI: Authentication process has stopped.");
    updateUIState();
}

/**
 * @brief Appends a new message to the log display text area.
 * @param message The message string to append.
 */
void MainWindow::appendLogMessage(const QString &message)
{
    ui->logDisplay->appendPlainText(message);
    // Auto-scroll to the bottom to show the latest message
    ui->logDisplay->verticalScrollBar()->setValue(ui->logDisplay->verticalScrollBar()->maximum());
}


// --- HELPER FUNCTION ---

/**
 * @brief Updates the enabled/disabled state of UI widgets based on the application's current state.
 */
void MainWindow::updateUIState()
{
    // Determine the current state of the application
    bool isListening = (m_server && m_server->isListening());
    bool isConnected = (m_client && m_client->isConnected()) || (m_server && m_server->hasActiveClient());
    bool isAuthRunning = (m_server && m_server->isAuthRunning());

    // Enable/disable connection-related controls
    ui->connectButton->setEnabled(!isListening && !isConnected);
    ui->disconnectButton->setEnabled(isListening || isConnected);
    ui->serverRadioButton->setEnabled(!isListening && !isConnected);
    ui->clientRadioButton->setEnabled(!isListening && !isConnected);

    // Enable/disable authentication-related controls (only for server)
    ui->startButton->setEnabled(isConnected && !isAuthRunning);
    ui->stopButton->setEnabled(isConnected && isAuthRunning);

    // Client cannot start/stop authentication, so disable these buttons if in client mode
    if (m_client) {
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
    }
}