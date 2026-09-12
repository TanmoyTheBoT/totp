#include "SettingsDialog.h"
#include "version.h"
#include <QGroupBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Account Settings");
    setModal(true);
    resize(500, 400);
    setMaximumSize(500, 450);

    setupUI();

    connect(saveButton, &QPushButton::clicked, this, &SettingsDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
}

void SettingsDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    tabWidget = new QTabWidget(this);

    setupGeneralTab();
    setupAppearanceTab();
    setupNotificationsTab();
    setupBackupTab();
    setupAboutTab();

    mainLayout->addWidget(tabWidget);

    // Buttons
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel", this);
    saveButton = new QPushButton("Save", this);
    saveButton->setDefault(true);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);
}

void SettingsDialog::setupGeneralTab()
{
    auto *generalWidget = new QWidget;
    auto *layout = new QVBoxLayout(generalWidget);

    auto *startupGroup = new QGroupBox("Startup");
    auto *startupLayout = new QVBoxLayout(startupGroup);

    startWithWindowsCheck = new QCheckBox("Start with Windows");
    minimizeToTrayCheck = new QCheckBox("Minimize to system tray");

    startupLayout->addWidget(startWithWindowsCheck);
    startupLayout->addWidget(minimizeToTrayCheck);

    layout->addWidget(startupGroup);
    layout->addStretch();

    tabWidget->addTab(generalWidget, "General");
}

void SettingsDialog::setupAppearanceTab()
{
    auto *appearanceWidget = new QWidget;
    auto *layout = new QVBoxLayout(appearanceWidget);

    auto *themeGroup = new QGroupBox("Theme");
    auto *themeLayout = new QFormLayout(themeGroup);

    themeCombo = new QComboBox;
    themeCombo->addItems({"Light", "Dark", "Auto"});

    darkModeCheck = new QCheckBox("Dark Mode");
    autoModeCheck = new QCheckBox("Auto");

    themeLayout->addRow("Application Theme:", themeCombo);

    auto *modeGroup = new QGroupBox("Mode");
    auto *modeLayout = new QVBoxLayout(modeGroup);
    modeLayout->addWidget(darkModeCheck);
    modeLayout->addWidget(autoModeCheck);

    auto *langGroup = new QGroupBox("Language");
    auto *langLayout = new QFormLayout(langGroup);

    languageCombo = new QComboBox;
    languageCombo->addItems({"English", "Spanish", "French", "German", "Chinese"});

    langLayout->addRow("Language:", languageCombo);

    layout->addWidget(themeGroup);
    layout->addWidget(modeGroup);
    layout->addWidget(langGroup);
    layout->addStretch();

    tabWidget->addTab(appearanceWidget, "Appearance");
}

void SettingsDialog::setupNotificationsTab()
{
    auto *notifWidget = new QWidget;
    auto *layout = new QVBoxLayout(notifWidget);

    auto *notifGroup = new QGroupBox("Notifications");
    auto *notifLayout = new QVBoxLayout(notifGroup);

    showNotificationsCheck = new QCheckBox("Show notifications");
    copyOTPCheck = new QCheckBox("Copy OTP to clipboard");

    notifLayout->addWidget(showNotificationsCheck);
    notifLayout->addWidget(copyOTPCheck);

    layout->addWidget(notifGroup);
    layout->addStretch();

    tabWidget->addTab(notifWidget, "Notifications");
}

void SettingsDialog::setupBackupTab()
{
    auto *backupWidget = new QWidget;
    auto *layout = new QVBoxLayout(backupWidget);

    auto *backupGroup = new QGroupBox("Backup");
    auto *backupLayout = new QVBoxLayout(backupGroup);

    auto *exportBtn = new QPushButton("Export Accounts");
    auto *importBtn = new QPushButton("Import Accounts");

    backupLayout->addWidget(exportBtn);
    backupLayout->addWidget(importBtn);

    layout->addWidget(backupGroup);
    layout->addStretch();

    tabWidget->addTab(backupWidget, "Backup");
}

void SettingsDialog::setupAboutTab()
{
    auto *aboutWidget = new QWidget;
    auto *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto *scrollContent = new QWidget;
    auto *layout = new QVBoxLayout(scrollContent);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 15, 20, 15);

    // App Icon
    auto *appIconLabel = new QLabel(this);
    appIconLabel->setFixedSize(60, 60);
    appIconLabel->setAlignment(Qt::AlignCenter);
    appIconLabel->setStyleSheet("font-size: 36px; background: #21262d; border-radius: 12px;");
    appIconLabel->setText("🔐");

    layout->addWidget(appIconLabel, 0, Qt::AlignCenter);
    layout->addSpacing(5);

    // App Name
    auto *titleLabel = new QLabel(Version::APP_NAME);
    QFont nameFont;
    nameFont.setPointSize(18);
    nameFont.setBold(true);
    titleLabel->setFont(nameFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #58a6ff;");

    layout->addWidget(titleLabel);

    // Version
    auto *versionLabel = new QLabel(Version::getVersionString());
    QFont versionFont;
    versionFont.setPointSize(10);
    versionLabel->setFont(versionFont);
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("color: #8b949e;");

    layout->addWidget(versionLabel);
    layout->addSpacing(8);

    // Description
    auto *descLabel = new QLabel(Version::DESCRIPTION);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setStyleSheet("color: #c9d1d9; font-size: 9pt;");
    descLabel->setWordWrap(true);

    layout->addWidget(descLabel);
    layout->addSpacing(10);

    // Info Section
    auto *infoWidget = new QWidget(this);
    auto *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setSpacing(6);
    infoWidget->setStyleSheet("background: #0d1117; border: 1px solid #21262d; border-radius: 6px; padding: 12px;");

    auto *websiteLabel = new QLabel(QString("<a href='%1' style='color: #58a6ff; text-decoration: none;'>%1</a>")
                                .arg(Version::WEBSITE));
    websiteLabel->setOpenExternalLinks(true);
    websiteLabel->setAlignment(Qt::AlignCenter);
    websiteLabel->setStyleSheet("font-size: 9pt;");

    auto *developerLabel = new QLabel(QString("<b>Developer:</b> %1").arg(Version::DEVELOPER));
    developerLabel->setStyleSheet("color: #c9d1d9; font-size: 9pt;");

    auto *licenseLabel = new QLabel(QString("<b>License:</b> %1").arg(Version::LICENSE));
    licenseLabel->setStyleSheet("color: #c9d1d9; font-size: 9pt;");

    auto *buildLabel = new QLabel(Version::getBuildInfo());
    buildLabel->setStyleSheet("color: #8b949e; font-size: 8pt;");
    buildLabel->setAlignment(Qt::AlignCenter);

    infoLayout->addWidget(websiteLabel);
    infoLayout->addSpacing(6);
    infoLayout->addWidget(developerLabel);
    infoLayout->addWidget(licenseLabel);
    infoLayout->addSpacing(4);
    infoLayout->addWidget(buildLabel);

    layout->addWidget(infoWidget);
    layout->addSpacing(8);

    // Features
    auto *featuresWidget = new QWidget(this);
    auto *featuresLayout = new QVBoxLayout(featuresWidget);
    featuresWidget->setStyleSheet("background: #0d1117; border: 1px solid #21262d; border-radius: 6px; padding: 12px;");

    auto *featuresTitle = new QLabel("<b>Features</b>");
    featuresTitle->setStyleSheet("color: #58a6ff; font-size: 10pt;");

    auto *featuresList = new QLabel(
        "• Fast & Secure<br>"
        "• QR Code Support<br>"
        "• Multiple Accounts<br>"
        "• Dark Mode<br>"
        "• Cross Platform<br>"
        "• Import/Export<br>"
        "• Favorites & Notes"
    );
    featuresList->setStyleSheet("color: #c9d1d9; font-size: 9pt;");

    featuresLayout->addWidget(featuresTitle);
    featuresLayout->addSpacing(6);
    featuresLayout->addWidget(featuresList);

    layout->addWidget(featuresWidget);
    layout->addStretch();

    scrollArea->setWidget(scrollContent);

    auto *tabLayout = new QVBoxLayout(aboutWidget);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(scrollArea);

    tabWidget->addTab(aboutWidget, "About");
}

void SettingsDialog::onSaveClicked()
{
    m_settings.startWithWindows = startWithWindowsCheck->isChecked();
    m_settings.minimizeToTray = minimizeToTrayCheck->isChecked();
    m_settings.theme = themeCombo->currentText();
    m_settings.darkMode = darkModeCheck->isChecked();
    m_settings.autoMode = autoModeCheck->isChecked();
    m_settings.language = languageCombo->currentText();
    m_settings.showNotifications = showNotificationsCheck->isChecked();
    m_settings.copyOTPToClipboard = copyOTPCheck->isChecked();

    accept();
}

void SettingsDialog::onCancelClicked()
{
    reject();
}

SettingsDialog::Settings SettingsDialog::getSettings() const
{
    return m_settings;
}

void SettingsDialog::setSettings(const Settings& settings)
{
    m_settings = settings;

    startWithWindowsCheck->setChecked(settings.startWithWindows);
    minimizeToTrayCheck->setChecked(settings.minimizeToTray);
    themeCombo->setCurrentText(settings.theme);
    darkModeCheck->setChecked(settings.darkMode);
    autoModeCheck->setChecked(settings.autoMode);
    languageCombo->setCurrentText(settings.language);
    showNotificationsCheck->setChecked(settings.showNotifications);
    copyOTPCheck->setChecked(settings.copyOTPToClipboard);
}
