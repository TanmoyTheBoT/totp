#include <QApplication>
#include <QMessageBox>

#include "MainWindow.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("TOTP Manager");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("Tanmoy");
    QApplication::setOrganizationDomain("tanmoy.dev");

    Database db;

    if (!db.open("data/accounts.db"))
    {
        QMessageBox::critical(
            nullptr,
            "Database Error",
            "Failed to open SQLite database."
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