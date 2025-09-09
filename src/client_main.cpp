#include <QCoreApplication>
#include "Client.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    if (argc < 2) {
        std::cerr << "Usage: lamport-client <config.json>" << std::endl;
        return -1;
    }

    QString configPath = argv[1];

    Client client(configPath, &app);

    QObject::connect(&client, &Client::disconnected, &app, &QCoreApplication::quit);

    return app.exec();
}