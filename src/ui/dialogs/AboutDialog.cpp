#include "AboutDialog.h"
#include "version.h"
#include <QHBoxLayout>
#include <QFont>
#include <QScrollArea>
#include <QFrame>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QString("About %1").arg(Version::APP_NAME));
    setModal(true);
    resize(500, 600);
    setMinimumSize(450, 500);
    setStyleSheet("QDialog { background-color: #0d1117; }");

    setupUI();
}

void AboutDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Add scroll area
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollArea { background-color: #0d1117; border: none; }");

    auto *scrollContent = new QWidget;
    auto *contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(15);
    contentLayout->setContentsMargins(30, 30, 30, 30);

    // App Icon
    appIconLabel = new QLabel(scrollContent);
    appIconLabel->setFixedSize(80, 80);
    appIconLabel->setAlignment(Qt::AlignCenter);
    appIconLabel->setStyleSheet("font-size: 48px; background: #21262d; border-radius: 16px;");
    appIconLabel->setText("🔐");

    contentLayout->addWidget(appIconLabel, 0, Qt::AlignCenter);
    contentLayout->addSpacing(10);

    // App Name
    appNameLabel = new QLabel(Version::APP_NAME, scrollContent);
    QFont nameFont;
    nameFont.setPointSize(24);
    nameFont.setBold(true);
    appNameLabel->setFont(nameFont);
    appNameLabel->setAlignment(Qt::AlignCenter);
    appNameLabel->setStyleSheet("color: #58a6ff;");

    contentLayout->addWidget(appNameLabel);

    // Version
    versionLabel = new QLabel(Version::getVersionString(), scrollContent);
    QFont versionFont;
    versionFont.setPointSize(12);
    versionLabel->setFont(versionFont);
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("color: #8b949e;");

    contentLayout->addWidget(versionLabel);
    contentLayout->addSpacing(10);

    // Description
    descriptionLabel = new QLabel(Version::DESCRIPTION, scrollContent);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setStyleSheet("color: #c9d1d9; font-size: 11pt;");

    contentLayout->addWidget(descriptionLabel);
    contentLayout->addSpacing(20);

    // Info Section
    auto *infoWidget = new QWidget(scrollContent);
    auto *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setSpacing(8);
    infoWidget->setStyleSheet("background: #0d1117; border: 1px solid #21262d; border-radius: 8px; padding: 16px;");

    websiteLabel = new QLabel(QString("<a href='%1' style='color: #58a6ff; text-decoration: none;'>%1</a>")
                                .arg(Version::WEBSITE), scrollContent);
    websiteLabel->setOpenExternalLinks(true);
    websiteLabel->setAlignment(Qt::AlignCenter);

    developerLabel = new QLabel(QString("<b>Developer:</b> %1").arg(Version::DEVELOPER), scrollContent);
    developerLabel->setStyleSheet("color: #c9d1d9;");

    licenseLabel = new QLabel(QString("<b>License:</b> %1").arg(Version::LICENSE), scrollContent);
    licenseLabel->setStyleSheet("color: #c9d1d9;");

    buildDateLabel = new QLabel(Version::getBuildInfo(), scrollContent);
    buildDateLabel->setStyleSheet("color: #8b949e; font-size: 9pt;");
    buildDateLabel->setAlignment(Qt::AlignCenter);

    infoLayout->addWidget(websiteLabel);
    infoLayout->addSpacing(10);
    infoLayout->addWidget(developerLabel);
    infoLayout->addWidget(licenseLabel);
    infoLayout->addSpacing(5);
    infoLayout->addWidget(buildDateLabel);

    contentLayout->addWidget(infoWidget);
    contentLayout->addSpacing(10);

    // Features
    auto *featuresWidget = new QWidget(scrollContent);
    auto *featuresLayout = new QVBoxLayout(featuresWidget);
    featuresWidget->setStyleSheet("background: #0d1117; border: 1px solid #21262d; border-radius: 8px; padding: 16px;");

    auto *featuresTitle = new QLabel("<b>Features</b>", scrollContent);
    featuresTitle->setStyleSheet("color: #58a6ff; font-size: 11pt;");

    featuresLabel = new QLabel(
        "• Fast & Secure<br>"
        "• QR Code Support<br>"
        "• Multiple Accounts<br>"
        "• Dark Mode<br>"
        "• Cross Platform<br>"
        "• Import/Export<br>"
        "• Favorites & Notes",
        scrollContent
    );
    featuresLabel->setStyleSheet("color: #c9d1d9;");

    featuresLayout->addWidget(featuresTitle);
    featuresLayout->addSpacing(8);
    featuresLayout->addWidget(featuresLabel);

    contentLayout->addWidget(featuresWidget);
    contentLayout->addStretch();

    // Set scroll content
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // Close Button
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    closeButton = new QPushButton("Close", this);
    closeButton->setFixedSize(100, 36);
    closeButton->setDefault(true);

    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
