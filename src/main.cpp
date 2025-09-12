#include "MainWindow.hpp"
#include <QApplication>

/**
 * @brief The main entry point for the application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Application exit code.
 */
int main(int argc, char *argv[])
{
    // QApplication manages GUI application-wide resources
    QApplication a(argc, argv);

    // Create an instance of the main window
    MainWindow w;

    // Show the main window on the screen
    w.show();

    // Start the application's event loop and wait for it to exit
    return a.exec();
}