#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ConfigManager.hpp"
#include "Client.hpp"
#include "Server.hpp"

// Forward declaration of the UI class from the .ui file
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main window of the application, managing the UI and interactions.
 *
 * This class is responsible for creating and managing the user interface,
 * handling user input, and coordinating the creation and destruction of
 * the Server and Client objects.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the MainWindow.
     * @param parent The parent widget, if any.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow.
     */
    ~MainWindow();

private slots:
    // --- UI SLOTS ---
    // These slots are connected to signals from UI widgets (e.g., button clicks).

    /**
     * @brief Handles the click event of the 'Connect' button.
     */
    void on_connectButton_clicked();

    /**
     * @brief Handles the click event of the 'Disconnect' button.
     */
    void on_disconnectButton_clicked();

    /**
     * @brief Handles the click event of the 'Start Auth' button.
     */
    void on_startButton_clicked();

    /**
     * @brief Handles the click event of the 'Stop Auth' button.
     */
    void on_stopButton_clicked();


    // --- BACKEND SLOTS ---
    // These slots are connected to signals from the Server or Client objects.

    /**
     * @brief Slot to handle a successful connection from the backend (Client or Server).
     */
    void onBackendConnected();

    /**
     * @brief Slot to handle a disconnection from the backend.
     */
    void onBackendDisconnected();

    /**
     * @brief Slot to handle the start of the authentication process.
     */
    void onAuthProcessStarted();

    /**
     * @brief Slot to handle the end of the authentication process.
     */
    void onAuthProcessStopped();

    /**
     * @brief Appends a message to the log display.
     * @param message The message to append.
     */
    void appendLogMessage(const QString &message);

private:
    /**
     * @brief Updates the enabled/disabled state of UI elements based on the application's state.
     */
    void updateUIState();

    // --- MEMBER VARIABLES ---

    Ui::MainWindow *ui;         ///< Pointer to the UI components generated from the .ui file.
    Client* m_client = nullptr; ///< Pointer to the active Client instance, if any.
    Server* m_server = nullptr; ///< Pointer to the active Server instance, if any.
    ConfigManager m_config;     ///< Manages loading and accessing configuration data.
};
#endif // MAINWINDOW_H