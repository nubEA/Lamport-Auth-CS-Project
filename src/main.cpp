#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication is used for GUI applications
    QApplication a(argc, argv);

    // Create an instance of your main window
    MainWindow w;

    // Show the window on the screen
    w.show();

    // Start the application's event loop
    return a.exec();
}