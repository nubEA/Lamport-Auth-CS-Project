#include <QCoreApplication>
#include "Server.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    if (argc < 2) {
        std::cerr << "Usage: lamport-server <config.json>" << std::endl;
        return -1;
    }

    QString configPath = argv[1];

    Server server(configPath);

    return app.exec();
}