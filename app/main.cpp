#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>

#include "MainWindow.h"
#include "database.h"
#include "version.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName(Version::APP_NAME);
    QApplication::setApplicationVersion(Version::VERSION);
    QApplication::setOrganizationName("TanmoyTheBoT");
    QApplication::setOrganizationDomain("tanmoythebot.github.io");

    // Setup config directory
    QString configPath = QDir::homePath() + "/.config/totpmanager";
    QDir configDir;
    if (!configDir.exists(configPath)) {
        if (!configDir.mkpath(configPath)) {
            QMessageBox::critical(
                nullptr,
                "Configuration Error",
                "Failed to create configuration directory at:\n" + configPath
            );
            return -1;
        }
    }

    // Database path in config directory
    QString dbPath = configPath + "/accounts.db";

    Database db;

    if (!db.open(dbPath.toStdString()))
    {
        QMessageBox::critical(
            nullptr,
            "Database Error",
            "Failed to open SQLite database at:\n" + dbPath
        );

        return -1;
    }

    db.createTables();

    MainWindow window;

    // Give MainWindow access to the database
    window.setDatabase(&db);

    window.show();

    int result = app.exec();

    db.close();

    return result;
}