#include "AddAccountDialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFileDialog>

AddAccountDialog::AddAccountDialog(QWidget *parent)
    : QDialog(parent), scanMode(false)
{
    setWindowTitle("Add Account");
    setModal(true);
    resize(550, 500);

    setupUI();
}

void AddAccountDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Title
    auto *titleLabel = new QLabel("Add Account");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);

    // Tab-like buttons for input method
    auto *modeLayout = new QHBoxLayout;

    manualButton = new QPushButton("Manual Entry");
    manualButton->setCheckable(true);
    manualButton->setChecked(true);

    scanQRButton = new QPushButton("Scan QR Code");
    scanQRButton->setCheckable(true);

    modeLayout->addWidget(manualButton);
    modeLayout->addWidget(scanQRButton);
    modeLayout->addStretch();

    mainLayout->addLayout(modeLayout);
    mainLayout->addSpacing(10);

    // Stacked widget for switching between modes
    stackedWidget = new QStackedWidget(this);

    // Manual entry page
    auto *manualPage = new QWidget;
    auto *manualLayout = new QVBoxLayout(manualPage);

    issuerEdit = new QLineEdit(this);
    usernameEdit = new QLineEdit(this);
    secretEdit = new QLineEdit(this);
    iconUrlEdit = new QLineEdit(this);

    issuerEdit->setPlaceholderText("e.g. Google, GitHub, Microsoft...");
    usernameEdit->setPlaceholderText("e.g. username or email");
    secretEdit->setPlaceholderText("Base32-encoded secret key");
    iconUrlEdit->setPlaceholderText("Icon URL or leave empty for default");

    algorithmBox = new QComboBox(this);
    algorithmBox->addItems({"SHA1", "SHA256", "SHA512"});

    digitsBox = new QComboBox(this);
    digitsBox->addItems({"6", "8"});

    periodBox = new QComboBox(this);
    periodBox->addItems({"30", "60"});

    auto *form = new QFormLayout;
    form->setSpacing(12);

    form->addRow("Issuer:", issuerEdit);
    form->addRow("Account:", usernameEdit);
    form->addRow("Secret Key:", secretEdit);
    form->addRow("Icon URL:", iconUrlEdit);
    form->addRow("Algorithm:", algorithmBox);
    form->addRow("Digits:", digitsBox);
    form->addRow("Period:", periodBox);

    manualLayout->addLayout(form);
    manualLayout->addStretch();

    // QR Scan page
    auto *scanPage = new QWidget;
    auto *scanLayout = new QVBoxLayout(scanPage);

    auto *scanGroup = new QGroupBox("Scan QR Code");
    auto *scanGroupLayout = new QVBoxLayout(scanGroup);

    qrScanLabel = new QLabel(this);
    qrScanLabel->setFixedSize(320, 320);
    qrScanLabel->setAlignment(Qt::AlignCenter);
    qrScanLabel->setStyleSheet("border: 2px dashed #2d3139; border-radius: 8px; background: #151b23;");
    qrScanLabel->setText("📷\n\nScan QR Code\n\nPoint your camera at the QR code\nfrom your authenticator app");

    scanGroupLayout->addWidget(qrScanLabel, 0, Qt::AlignCenter);

    auto *orLabel = new QLabel("OR");
    orLabel->setAlignment(Qt::AlignCenter);
    orLabel->setStyleSheet("color: #8b949e; font-weight: bold;");

    browseQRButton = new QPushButton("Browse Image");
    browseQRButton->setFixedHeight(36);

    scanLayout->addWidget(scanGroup);
    scanLayout->addSpacing(10);
    scanLayout->addWidget(orLabel);
    scanLayout->addWidget(browseQRButton, 0, Qt::AlignCenter);
    scanLayout->addStretch();

    stackedWidget->addWidget(manualPage);
    stackedWidget->addWidget(scanPage);

    mainLayout->addWidget(stackedWidget, 1);

    // Buttons
    auto *buttons = new QHBoxLayout;
    buttons->addStretch();

    cancelButton = new QPushButton("Cancel", this);
    saveButton = new QPushButton("Add Account", this);
    saveButton->setDefault(true);

    buttons->addWidget(cancelButton);
    buttons->addWidget(saveButton);

    mainLayout->addLayout(buttons);

    connect(manualButton, &QPushButton::clicked, this, &AddAccountDialog::switchToManual);
    connect(scanQRButton, &QPushButton::clicked, this, &AddAccountDialog::switchToScan);
    connect(browseQRButton, &QPushButton::clicked, this, &AddAccountDialog::browseQRImage);
    connect(saveButton, &QPushButton::clicked, this, &AddAccountDialog::save);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void AddAccountDialog::switchToManual()
{
    stackedWidget->setCurrentIndex(0);
    manualButton->setChecked(true);
    scanQRButton->setChecked(false);
    scanMode = false;
}

void AddAccountDialog::switchToScan()
{
    stackedWidget->setCurrentIndex(1);
    manualButton->setChecked(false);
    scanQRButton->setChecked(true);
    scanMode = true;
}

void AddAccountDialog::browseQRImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select QR Code Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            qrScanLabel->setPixmap(pixmap.scaled(320, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            // ponytail: QR decode logic would parse otpauth:// URI here
            QMessageBox::information(this, "QR Code", "QR code loaded. Decoding not yet implemented.\nPlease use manual entry for now.");
        }
    }
}

void AddAccountDialog::setAccount(const Account& account)
{
    m_account = account;

    issuerEdit->setText(QString::fromStdString(account.issuer));
    usernameEdit->setText(QString::fromStdString(account.username));
    secretEdit->setText(QString::fromStdString(account.secret));
    iconUrlEdit->setText(QString::fromStdString(account.iconPath));
    algorithmBox->setCurrentText(QString::fromStdString(account.algorithm));
    digitsBox->setCurrentText(QString::number(account.digits));
    periodBox->setCurrentText(QString::number(account.period));

    setWindowTitle("Edit Account");
    saveButton->setText("Save Changes");

    // Switch to manual mode when editing
    switchToManual();
    scanQRButton->setEnabled(false);
}

void AddAccountDialog::save()
{
    if (issuerEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Issuer cannot be empty.");
        return;
    }

    if (usernameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Account name cannot be empty.");
        return;
    }

    if (secretEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Secret key cannot be empty.");
        return;
    }

    m_account.issuer = issuerEdit->text().trimmed().toStdString();
    m_account.username = usernameEdit->text().trimmed().toStdString();
    m_account.secret = secretEdit->text().trimmed().toUpper().toStdString();
    m_account.iconPath = iconUrlEdit->text().trimmed().toStdString();
    m_account.algorithm = algorithmBox->currentText().toStdString();
    m_account.digits = digitsBox->currentText().toInt();
    m_account.period = periodBox->currentText().toInt();

    accept();
}

Account AddAccountDialog::account() const
{
    return m_account;
}
