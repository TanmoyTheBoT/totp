#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>

#include "database.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setDatabase(Database *database);

private:

    Database *m_database = nullptr;

private slots:

    void updateOTP();

    void addAccount();

    void editAccount();

    void deleteAccount();

    void copyOTP();

    void showQRCode();

    void hideQRCode();

    void accountSelectionChanged();

    void about();

private:

    void setupUI();

    void setupMenu();

    void loadAccounts();

    void refreshCurrentAccount();



    //==============================
    // Left Panel
    //==============================

    QWidget* central;

    QLineEdit* searchEdit;

    QListWidget* accountList;

    QPushButton* addButton;

    QPushButton* editButton;

    QPushButton* deleteButton;



    //==============================
    // Right Panel
    //==============================

    QLabel* issuerLabel;

    QLabel* accountLabel;

    QLabel* otpLabel;

    QLabel* timerLabel;

    QLabel* qrLabel;

    QLabel* uriLabel;



    QPushButton* copyButton;

    QPushButton* showQRButton;

    QPushButton* hideQRButton;



    QProgressBar* progressBar;



    //==============================
    // Timer
    //==============================

    QTimer* timer;



    //==============================
    // Menu
    //==============================

    QMenu* fileMenu;

    QMenu* accountMenu;

    QMenu* helpMenu;



    QAction* addAction;

    QAction* importAction;

    QAction* exportAction;

    QAction* exitAction;

    QAction* aboutAction;
};

#endif