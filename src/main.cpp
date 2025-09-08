#include <QCoreApplication>
#include "ConfigManager.hpp"

int main(int argc, char *argv[]) {
    qDebug() << "Starting test...";


    QCoreApplication app(argc, argv);

    QString configPath = "/home/harshit/code/lamport-auth-qt/config.json";

    ConfigManager config(configPath);

    qDebug() << "Alice IP:" << config.getAliceIP();
    qDebug() << "Alice Port:" << config.getAlicePort();
    qDebug() << "Bob IP:" << config.getBobIP();
    qDebug() << "Bob Port:" << config.getBobPort();
    qDebug() << "Sleep Duration:" << config.getSleepTime();
    qDebug() << "Number of Rounds:" << config.getNumberOfIterations();

    return 0;
}
