#include "ShowQRDialog.h"
#include "qrcode.h"
#include "services/otp_uri.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFont>

ShowQRDialog::ShowQRDialog(const Account& account, QWidget *parent)
    : QDialog(parent), m_account(account)
{
    setWindowTitle("Show QR Code");
    setModal(true);
    resize(450, 550);

    setupUI();
    generateQR();
}

void ShowQRDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Title
    auto *titleLabel = new QLabel("Google");
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *accountLabel = new QLabel(QString::fromStdString(m_account.username));
    accountLabel->setAlignment(Qt::AlignCenter);
    accountLabel->setStyleSheet("color: #8b949e;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(accountLabel);
    mainLayout->addSpacing(20);

    // QR Code
    qrLabel = new QLabel(this);
    qrLabel->setFixedSize(300, 300);
    qrLabel->setAlignment(Qt::AlignCenter);
    qrLabel->setScaledContents(true);
    qrLabel->setStyleSheet("border: 1px solid #2d3139; border-radius: 8px; background: white; padding: 10px;");

    mainLayout->addWidget(qrLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);

    // Scan instructions
    auto *scanLabel = new QLabel("Scan this QR code to add to your\nauthenticator app");
    scanLabel->setAlignment(Qt::AlignCenter);
    scanLabel->setStyleSheet("color: #8b949e;");

    mainLayout->addWidget(scanLabel);
    mainLayout->addSpacing(10);

    // URI Label
    auto *uriTitleLabel = new QLabel("URL:");
    uriTitleLabel->setStyleSheet("font-weight: bold;");

    uriLabel = new QLabel(this);
    uriLabel->setWordWrap(true);
    uriLabel->setStyleSheet("color: #8b949e; font-size: 8pt; background: #1a1f26; border: 1px solid #2d3139; border-radius: 4px; padding: 8px;");

    mainLayout->addWidget(uriTitleLabel);
    mainLayout->addWidget(uriLabel);

    // Secret Label
    auto *secretTitleLabel = new QLabel("Secret:");
    secretTitleLabel->setStyleSheet("font-weight: bold;");

    secretLabel = new QLabel(QString::fromStdString(m_account.secret));
    secretLabel->setStyleSheet("color: #8b949e; font-family: monospace; background: #1a1f26; border: 1px solid #2d3139; border-radius: 4px; padding: 8px;");
    secretLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    mainLayout->addWidget(secretTitleLabel);
    mainLayout->addWidget(secretLabel);

    mainLayout->addStretch();

    // Close Button
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    closeButton = new QPushButton("Close", this);
    closeButton->setDefault(true);

    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void ShowQRDialog::generateQR()
{
    std::string uri = OTPURI::createTOTP(
        m_account.issuer,
        m_account.username,
        m_account.secret,
        m_account.digits,
        m_account.period
    );

    uriLabel->setText(QString::fromStdString(uri));

    QRCode qr;
    std::vector<uint8_t> png = qr.generatePNG(uri, 300);

    if (!png.empty()) {
        QPixmap pixmap;
        if (pixmap.loadFromData(png.data(), png.size(), "PNG")) {
            qrLabel->setPixmap(pixmap);
        }
    }
}
