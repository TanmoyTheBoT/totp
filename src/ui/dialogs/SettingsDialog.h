#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    struct Settings {
        QString theme;
        QString language;
        bool startWithWindows;
        bool showNotifications;
        bool copyOTPToClipboard;
        bool minimizeToTray;
        bool darkMode;
        bool autoMode;
    };

    Settings getSettings() const;
    void setSettings(const Settings& settings);

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupGeneralTab();
    void setupAppearanceTab();
    void setupNotificationsTab();
    void setupBackupTab();
    void setupAboutTab();

    QTabWidget* tabWidget;

    // General Tab
    QCheckBox* startWithWindowsCheck;
    QCheckBox* minimizeToTrayCheck;

    // Appearance Tab
    QComboBox* themeCombo;
    QCheckBox* darkModeCheck;
    QCheckBox* autoModeCheck;
    QComboBox* languageCombo;

    // Notifications Tab
    QCheckBox* showNotificationsCheck;
    QCheckBox* copyOTPCheck;

    // Buttons
    QPushButton* saveButton;
    QPushButton* cancelButton;

    Settings m_settings;
};

#endif // SETTINGSDIALOG_H
