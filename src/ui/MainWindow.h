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
#include <QVBoxLayout>
#include <QTextEdit>
#include <QStatusBar>
#include <QScrollArea>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QSettings>

#include "database.h"
#include "widgets/AccountCard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setDatabase(Database *database);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    Database *m_database = nullptr;
    AccountCard *currentAccount;
    QList<AccountCard*> accountCards;
    bool secretVisible = false;

private slots:
    void updateOTP();
    void addAccount();
    void editAccount();
    void deleteAccount();
    void copyOTP();
    void showQRCode();
    void hideQRCode();
    void about();
    void filterAccounts();
    void toggleFavorite();
    void toggleSecretVisibility();
    void copySecret();
    void saveNote();
    void onNoteChanged();
    void onCardFavoriteToggled(int accountId, bool isFavorite);
    void openSettings();
    void importAccounts();
    void exportAccounts();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showFromTray();
    void quitApplication();

private:
    void setupUI();
    void setupMenu();
    void loadAccounts();
    void refreshCurrentAccount();
    void selectAccountCard(AccountCard *card);
    void showNotification(const QString &message);
    void loadDetailIcon(const Account &account);
    void loadDetailIconFromFile(const QString &filePath);
    void setupSystemTray();
    void loadSettings();
    void saveSettings();
    void setStartWithWindows(bool enable);

    //==============================
    // Settings
    //==============================
    QSettings m_settings;
    bool m_minimizeToTray;
    bool m_startWithWindows;

    //==============================
    // Left Panel
    //==============================
    QLineEdit* searchEdit;
    QWidget* accountCardsWidget;
    QVBoxLayout* accountCardsLayout;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QLabel* statusLabel;
    QLabel* accountCountLabel;

    //==============================
    // Right Panel
    //==============================
    QLabel* issuerLabel;
    QLabel* accountLabel;
    QLabel* otpLabel;
    QLabel* timerLabel;
    QLabel* qrLabel;
    QLabel* qrDescLabel;
    QLabel* uriLabel;

    QPushButton* copyButton;
    QPushButton* showQRButton;
    QPushButton* hideQRButton;
    QPushButton* favoriteButton;
    QPushButton* editAccountButton;

    QProgressBar* progressBar;

    // Account Details
    QLabel* detailIconLabel;
    QLabel* issuerDetailLabel;
    QLabel* accountDetailLabel;
    QLabel* secretDetailLabel;
    QPushButton* showSecretButton;
    QPushButton* copySecretButton;

    // Settings
    QLabel* algorithmDetailLabel;
    QLabel* digitsDetailLabel;
    QLabel* periodDetailLabel;

    // Notes
    QTextEdit* notesTextEdit;
    QPushButton* addNoteButton;

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
    QAction* settingsAction;
    QAction* exitAction;
    QAction* aboutAction;

    //==============================
    // System Tray
    //==============================
    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;
    QAction* showAction;
    QAction* quitAction;
};

#endif