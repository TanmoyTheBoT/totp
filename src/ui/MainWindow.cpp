#include "MainWindow.h"

#include <QApplication>
#include <QClipboard>
#include <QFont>
#include <QFrame>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFormLayout>
#include <QTextEdit>
#include <QDir>
#include <QFile>
#include <QCryptographicHash>
#include <QPainter>
#include <QPainterPath>

#include <ctime>
#include <algorithm>

#include "totp.h"
#include "version.h"
#include "qr/qrcode.h"
#include "dialogs/AddAccountDialog.h"
#include "dialogs/ShowQRDialog.h"
#include "dialogs/SettingsDialog.h"
#include "dialogs/DeleteConfirmDialog.h"
#include "dialogs/ImportExportDialog.h"
#include "dialogs/AboutDialog.h"
#include "widgets/AccountCard.h"
#include "styles/darktheme.h"
#include "styles/lighttheme.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentAccount(nullptr),
      m_settings("TanmoyTheBoT", "TOTP Manager"),
      m_minimizeToTray(false),
      m_startWithWindows(false)
{
    loadSettings();
    setupUI();
    setupMenu();
    setupSystemTray();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateOTP);
    timer->start(1000);

    updateOTP();
}

MainWindow::~MainWindow() = default;

void MainWindow::setDatabase(Database *database)
{
    m_database = database;
    loadAccounts();
}

void MainWindow::setupUI()
{
    resize(1400, 750);
    setMinimumSize(1350, 700);
    setWindowTitle(Version::getFullVersionString());
    setWindowIcon(QIcon(":/icons/app.png"));

    // Apply dark theme
    setStyleSheet(DarkTheme::getStyleSheet());

    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *rootLayout = new QHBoxLayout(centralWidget);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ============================================
    // LEFT PANEL - Account List
    // ============================================

    auto *leftPanel = new QFrame(this);
    leftPanel->setObjectName("leftPanel");
    leftPanel->setFixedWidth(380);

    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(15, 15, 15, 15);
    leftLayout->setSpacing(12);

    // Search
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Search accounts...");
    searchEdit->setFixedHeight(38);
    QFont searchFont;
    searchFont.setPointSize(10);
    searchEdit->setFont(searchFont);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::filterAccounts);

    leftLayout->addWidget(searchEdit);

    // Scroll area for account cards
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);

    accountCardsWidget = new QWidget;
    accountCardsLayout = new QVBoxLayout(accountCardsWidget);
    accountCardsLayout->setSpacing(8);
    accountCardsLayout->setContentsMargins(0, 0, 0, 0);
    accountCardsLayout->addStretch();

    scrollArea->setWidget(accountCardsWidget);
    leftLayout->addWidget(scrollArea, 1);

    // Buttons
    auto *buttonLayout = new QHBoxLayout;
    addButton = new QPushButton("Add", this);
    editButton = new QPushButton("Edit", this);
    deleteButton = new QPushButton("Delete", this);

    addButton->setObjectName("addButton");
    editButton->setObjectName("editButton");
    deleteButton->setObjectName("deleteButton");

    addButton->setFixedHeight(36);
    editButton->setFixedHeight(36);
    deleteButton->setFixedHeight(36);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    leftLayout->addLayout(buttonLayout);

    // Status bar at bottom
    statusLabel = new QLabel("Database connected", this);
    statusLabel->setStyleSheet("color: #2ea043; font-size: 9pt;");

    accountCountLabel = new QLabel("0 accounts", this);
    accountCountLabel->setStyleSheet("color: #8b949e; font-size: 9pt;");

    auto *statusLayout = new QHBoxLayout;
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(accountCountLabel);

    leftLayout->addLayout(statusLayout);

    // ============================================
    // RIGHT PANEL
    // ============================================

    auto *rightPanel = new QFrame(this);
    rightPanel->setObjectName("rightPanel");

    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(25, 25, 25, 25);
    rightLayout->setSpacing(16);

    // Account header with favorite star
    auto *headerLayout = new QHBoxLayout;

    // Icon
    detailIconLabel = new QLabel(this);
    detailIconLabel->setFixedSize(80, 80);
    detailIconLabel->setAlignment(Qt::AlignCenter);
    detailIconLabel->setStyleSheet("border-radius: 40px; background: #21262d;");

    auto *accountInfoLayout = new QVBoxLayout;
    issuerLabel = new QLabel("Select an account", this);
    QFont issuerHeaderFont;
    issuerHeaderFont.setPointSize(20);
    issuerHeaderFont.setBold(true);
    issuerLabel->setFont(issuerHeaderFont);
    issuerLabel->setStyleSheet("color: #c9d1d9;");

    accountLabel = new QLabel("", this);
    accountLabel->setStyleSheet("color: #8b949e; font-size: 11pt;");

    accountInfoLayout->addWidget(issuerLabel);
    accountInfoLayout->addWidget(accountLabel);
    accountInfoLayout->setSpacing(4);

    favoriteButton = new QPushButton(this);
    favoriteButton->setText("☆");
    favoriteButton->setFlat(true);
    favoriteButton->setFixedSize(40, 40);
    favoriteButton->setCursor(Qt::PointingHandCursor);
    favoriteButton->setToolTip("Toggle Favorite");
    QFont favFont;
    favFont.setPointSize(20);
    favoriteButton->setFont(favFont);
    favoriteButton->setStyleSheet("QPushButton { border: 1px solid #30363d; border-radius: 6px; color: #8b949e; background: #21262d; font-size: 20px; } QPushButton:hover { color: #ffd700; background: #30363d; border-color: #ffd700; }");

    editAccountButton = new QPushButton("✏️ Edit", this);
    editAccountButton->setFixedSize(90, 34);
    editAccountButton->setStyleSheet("QPushButton { background: #21262d; color: #c9d1d9; border: 1px solid #30363d; border-radius: 6px; font-weight: 600; } QPushButton:hover { background: #30363d; border-color: #1f6feb; }");

    headerLayout->addWidget(detailIconLabel);
    headerLayout->addSpacing(12);
    headerLayout->addLayout(accountInfoLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(favoriteButton);
    headerLayout->addWidget(editAccountButton);

    rightLayout->addLayout(headerLayout);
    rightLayout->addSpacing(8);

    // Horizontal layout for OTP and QR side-by-side
    auto *otpQrLayout = new QHBoxLayout;
    otpQrLayout->setSpacing(16);

    // OTP Display Section (LEFT)
    auto *otpSection = new QFrame(this);
    otpSection->setObjectName("otpSection");
    otpSection->setFixedHeight(180);
    otpSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto *otpLayout = new QVBoxLayout(otpSection);
    otpLayout->setSpacing(8);

    auto *currentOTPLabel = new QLabel("🟢 Current OTP");
    currentOTPLabel->setStyleSheet("color: #8b949e; font-size: 10pt; font-weight: 600;");

    otpLabel = new QLabel("--- ---", this);
    QFont otpFont;
    otpFont.setPointSize(48);
    otpFont.setBold(true);
    otpFont.setLetterSpacing(QFont::AbsoluteSpacing, 10);
    otpLabel->setFont(otpFont);
    otpLabel->setAlignment(Qt::AlignCenter);
    otpLabel->setStyleSheet("color: #58a6ff; padding: 10px;");

    auto *timerLayout = new QHBoxLayout;
    timerLayout->addStretch();

    timerLabel = new QLabel("30s", this);
    QFont timerFont;
    timerFont.setPointSize(14);
    timerFont.setBold(true);
    timerLabel->setFont(timerFont);
    timerLabel->setStyleSheet("color: #8b949e;");

    timerLayout->addWidget(timerLabel);
    timerLayout->addStretch();

    progressBar = new QProgressBar(this);
    progressBar->setMaximum(30);
    progressBar->setValue(30);
    progressBar->setTextVisible(false);
    progressBar->setFixedHeight(8);
    progressBar->setStyleSheet("QProgressBar { background: #21262d; border-radius: 4px; } QProgressBar::chunk { background: #238636; border-radius: 4px; }");

    otpLayout->addWidget(currentOTPLabel);
    otpLayout->addWidget(otpLabel);
    otpLayout->addLayout(timerLayout);
    otpLayout->addWidget(progressBar);

    // QR Code Section (RIGHT)
    auto *qrSection = new QFrame(this);
    qrSection->setObjectName("qrSection");
    qrSection->setFixedHeight(180);
    qrSection->setFixedWidth(180);
    qrSection->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto *qrLayout = new QVBoxLayout(qrSection);
    qrLayout->setSpacing(0);
    qrLayout->setAlignment(Qt::AlignCenter);
    qrLayout->setContentsMargins(10, 10, 10, 10);

    qrLabel = new QLabel(this);
    qrLabel->setFixedSize(160, 160);
    qrLabel->setAlignment(Qt::AlignCenter);
    qrLabel->setScaledContents(false);
    qrLabel->setStyleSheet("background: white; border-radius: 4px;");

    qrLayout->addWidget(qrLabel);

    otpQrLayout->addWidget(otpSection, 1);
    otpQrLayout->addWidget(qrSection, 0);

    rightLayout->addLayout(otpQrLayout);

    // Action Buttons
    auto *actionLayout = new QHBoxLayout;
    copyButton = new QPushButton("📋 Copy Code", this);
    copyButton->setObjectName("copyButton");
    copyButton->setFixedHeight(44);
    QFont copyFont;
    copyFont.setPointSize(11);
    copyFont.setBold(true);
    copyButton->setFont(copyFont);
    copyButton->setStyleSheet("QPushButton { background: #238636; color: white; border: none; border-radius: 6px; font-weight: 600; } QPushButton:hover { background: #2ea043; }");

    showQRButton = new QPushButton("🔲 Show QR", this);
    showQRButton->setFixedHeight(44);
    QFont qrFont;
    qrFont.setPointSize(10);
    showQRButton->setFont(qrFont);
    showQRButton->setStyleSheet("QPushButton { background: #21262d; color: #c9d1d9; border: 1px solid #30363d; border-radius: 6px; } QPushButton:hover { background: #30363d; border-color: #1f6feb; }");

    hideQRButton = new QPushButton("🔲 Hide QR", this);
    hideQRButton->setFixedHeight(44);
    hideQRButton->setFont(qrFont);
    hideQRButton->setStyleSheet("QPushButton { background: #21262d; color: #c9d1d9; border: 1px solid #30363d; border-radius: 6px; } QPushButton:hover { background: #30363d; }");
    hideQRButton->show();

    actionLayout->addWidget(copyButton, 2);
    actionLayout->addWidget(showQRButton, 1);
    actionLayout->addWidget(hideQRButton, 1);

    showQRButton->hide();

    rightLayout->addLayout(actionLayout);

    // Two-column layout for Account Details and Settings
    auto *twoColLayout = new QHBoxLayout;
    twoColLayout->setSpacing(20);

    // Account Details Section (LEFT)
    auto *detailsBox = new QFrame(this);
    detailsBox->setObjectName("detailsBox");
    detailsBox->setMinimumHeight(180);
    detailsBox->setStyleSheet("QFrame#detailsBox { background: #0d1117; border: 1px solid #21262d; border-radius: 8px; padding: 16px; }");

    auto *detailsLayout = new QVBoxLayout(detailsBox);
    detailsLayout->setSpacing(12);
    detailsLayout->setContentsMargins(16, 16, 16, 16);

    auto *detailsTitle = new QLabel("👤 Account Details");
    detailsTitle->setStyleSheet("color: #8b949e; font-size: 11pt; font-weight: 600;");

    auto *detailsGrid = new QVBoxLayout;
    detailsGrid->setSpacing(10);

    // Issuer row
    auto *issuerRow = new QVBoxLayout;
    issuerRow->setSpacing(2);
    auto *issuerLabelText = new QLabel("Issuer");
    issuerLabelText->setStyleSheet("color: #8b949e; font-size: 9pt;");

    auto *issuerValueRow = new QHBoxLayout;
    issuerDetailLabel = new QLabel("---");
    issuerDetailLabel->setStyleSheet("color: #c9d1d9; font-size: 10pt;");

    auto *copyIssuerButton = new QPushButton("📋");
    copyIssuerButton->setFlat(true);
    copyIssuerButton->setFixedSize(26, 26);
    copyIssuerButton->setCursor(Qt::PointingHandCursor);
    copyIssuerButton->setStyleSheet("QPushButton { border: 1px solid #30363d; border-radius: 4px; background: #21262d; font-size: 11pt; padding: 2px; } QPushButton:hover { background: #30363d; border-color: #58a6ff; }");
    connect(copyIssuerButton, &QPushButton::clicked, [this]() {
        if (currentAccount) QGuiApplication::clipboard()->setText(QString::fromStdString(currentAccount->account().issuer));
    });

    issuerValueRow->addWidget(issuerDetailLabel);
    issuerValueRow->addStretch();
    issuerValueRow->addWidget(copyIssuerButton);

    issuerRow->addWidget(issuerLabelText);
    issuerRow->addLayout(issuerValueRow);

    // Account row
    auto *accountRow = new QVBoxLayout;
    accountRow->setSpacing(2);
    auto *accountLabelText = new QLabel("Account");
    accountLabelText->setStyleSheet("color: #8b949e; font-size: 9pt;");

    auto *accountValueRow = new QHBoxLayout;
    accountDetailLabel = new QLabel("---");
    accountDetailLabel->setStyleSheet("color: #c9d1d9; font-size: 10pt;");

    auto *copyAccountButton = new QPushButton("📋");
    copyAccountButton->setFlat(true);
    copyAccountButton->setFixedSize(26, 26);
    copyAccountButton->setCursor(Qt::PointingHandCursor);
    copyAccountButton->setStyleSheet("QPushButton { border: 1px solid #30363d; border-radius: 4px; background: #21262d; font-size: 11pt; padding: 2px; } QPushButton:hover { background: #30363d; border-color: #58a6ff; }");
    connect(copyAccountButton, &QPushButton::clicked, [this]() {
        if (currentAccount) QGuiApplication::clipboard()->setText(QString::fromStdString(currentAccount->account().username));
    });

    accountValueRow->addWidget(accountDetailLabel);
    accountValueRow->addStretch();
    accountValueRow->addWidget(copyAccountButton);

    accountRow->addWidget(accountLabelText);
    accountRow->addLayout(accountValueRow);

    // Secret row
    auto *secretRow = new QVBoxLayout;
    secretRow->setSpacing(2);
    auto *secretLabelText = new QLabel("Secret");
    secretLabelText->setStyleSheet("color: #8b949e; font-size: 9pt;");

    auto *secretValueRow = new QHBoxLayout;
    secretDetailLabel = new QLabel("••••••••••••••••••••");
    secretDetailLabel->setStyleSheet("font-family: 'Courier New', monospace; color: #c9d1d9; font-size: 10pt;");

    showSecretButton = new QPushButton("👁");
    showSecretButton->setFlat(true);
    showSecretButton->setFixedSize(26, 26);
    showSecretButton->setCursor(Qt::PointingHandCursor);
    showSecretButton->setStyleSheet("QPushButton { border: 1px solid #30363d; border-radius: 4px; background: #21262d; font-size: 11pt; padding: 2px; } QPushButton:hover { background: #30363d; border-color: #58a6ff; }");

    copySecretButton = new QPushButton("📋");
    copySecretButton->setFlat(true);
    copySecretButton->setFixedSize(26, 26);
    copySecretButton->setCursor(Qt::PointingHandCursor);
    copySecretButton->setStyleSheet("QPushButton { border: 1px solid #30363d; border-radius: 4px; background: #21262d; font-size: 11pt; padding: 2px; } QPushButton:hover { background: #30363d; border-color: #58a6ff; }");

    secretValueRow->addWidget(secretDetailLabel);
    secretValueRow->addStretch();
    secretValueRow->addWidget(showSecretButton);
    secretValueRow->addWidget(copySecretButton);

    secretRow->addWidget(secretLabelText);
    secretRow->addLayout(secretValueRow);

    detailsGrid->addLayout(issuerRow);
    detailsGrid->addLayout(accountRow);
    detailsGrid->addLayout(secretRow);

    detailsLayout->addWidget(detailsTitle);
    detailsLayout->addLayout(detailsGrid);

    // Settings Section (RIGHT)
    auto *settingsBox = new QFrame(this);
    settingsBox->setObjectName("settingsBox");
    settingsBox->setMinimumHeight(180);
    settingsBox->setStyleSheet("QFrame#settingsBox { background: #0d1117; border: 1px solid #21262d; border-radius: 8px; padding: 16px; }");

    auto *settingsLayout = new QVBoxLayout(settingsBox);
    settingsLayout->setSpacing(12);
    settingsLayout->setContentsMargins(16, 16, 16, 16);

    auto *settingsTitle = new QLabel("⚙️ Settings");
    settingsTitle->setStyleSheet("color: #8b949e; font-size: 11pt; font-weight: 600;");

    auto *settingsGrid = new QVBoxLayout;
    settingsGrid->setSpacing(10);

    // Algorithm row
    auto *algoRow = new QVBoxLayout;
    algoRow->setSpacing(2);
    auto *algoLabelText = new QLabel("Algorithm");
    algoLabelText->setStyleSheet("color: #8b949e; font-size: 9pt;");
    algorithmDetailLabel = new QLabel("SHA1");
    algorithmDetailLabel->setStyleSheet("color: #c9d1d9; font-size: 10pt;");
    algoRow->addWidget(algoLabelText);
    algoRow->addWidget(algorithmDetailLabel);

    // Digits row
    auto *digitsRow = new QVBoxLayout;
    digitsRow->setSpacing(2);
    auto *digitsLabelText = new QLabel("Digits");
    digitsLabelText->setStyleSheet("color: #8b949e; font-size: 9pt;");
    digitsDetailLabel = new QLabel("6");
    digitsDetailLabel->setStyleSheet("color: #c9d1d9; font-size: 10pt;");
    digitsRow->addWidget(digitsLabelText);
    digitsRow->addWidget(digitsDetailLabel);

    // Period row
    auto *periodRow = new QVBoxLayout;
    periodRow->setSpacing(2);
    auto *periodLabelText = new QLabel("Period");
    periodLabelText->setStyleSheet("color: #8b949e; font-size: 9pt;");
    periodDetailLabel = new QLabel("30 seconds");
    periodDetailLabel->setStyleSheet("color: #c9d1d9; font-size: 10pt;");
    periodRow->addWidget(periodLabelText);
    periodRow->addWidget(periodDetailLabel);

    settingsGrid->addLayout(algoRow);
    settingsGrid->addLayout(digitsRow);
    settingsGrid->addLayout(periodRow);

    settingsLayout->addWidget(settingsTitle);
    settingsLayout->addLayout(settingsGrid);

    twoColLayout->addWidget(detailsBox, 1);
    twoColLayout->addWidget(settingsBox, 1);

    rightLayout->addLayout(twoColLayout);

    // Notes Section
    auto *notesBox = new QFrame(this);
    notesBox->setObjectName("notesBox");
    notesBox->setMinimumHeight(100);

    auto *notesLayout = new QVBoxLayout(notesBox);
    notesLayout->setSpacing(8);
    notesLayout->setContentsMargins(0, 0, 0, 0);

    auto *notesHeader = new QHBoxLayout;
    auto *notesTitle = new QLabel("📝 Notes");
    notesTitle->setStyleSheet("color: #8b949e; font-size: 11pt; font-weight: 600;");

    addNoteButton = new QPushButton("✏️ Add Note");
    addNoteButton->setFixedHeight(28);
    addNoteButton->setStyleSheet("QPushButton { background: #21262d; color: #c9d1d9; border: 1px solid #30363d; border-radius: 6px; padding: 4px 10px; font-size: 9pt; } QPushButton:hover { background: #30363d; border-color: #1f6feb; }");

    notesHeader->addWidget(notesTitle);
    notesHeader->addStretch();
    notesHeader->addWidget(addNoteButton);

    notesTextEdit = new QTextEdit(this);
    notesTextEdit->setPlaceholderText("No notes available for this account.");
    notesTextEdit->setMinimumHeight(60);
    notesTextEdit->setMaximumHeight(80);

    notesLayout->addLayout(notesHeader);
    notesLayout->addWidget(notesTextEdit);

    rightLayout->addWidget(notesBox);
    rightLayout->addStretch();

    rootLayout->addWidget(leftPanel);
    rootLayout->addWidget(rightPanel, 1);

    // Connect signals
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addAccount);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editAccount);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteAccount);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::copyOTP);
    connect(showQRButton, &QPushButton::clicked, this, &MainWindow::showQRCode);
    connect(hideQRButton, &QPushButton::clicked, this, &MainWindow::hideQRCode);
    connect(favoriteButton, &QPushButton::clicked, this, &MainWindow::toggleFavorite);
    connect(editAccountButton, &QPushButton::clicked, this, &MainWindow::editAccount);
    connect(showSecretButton, &QPushButton::clicked, this, &MainWindow::toggleSecretVisibility);
    connect(copySecretButton, &QPushButton::clicked, this, &MainWindow::copySecret);
    connect(addNoteButton, &QPushButton::clicked, this, &MainWindow::saveNote);
    connect(notesTextEdit, &QTextEdit::textChanged, this, &MainWindow::onNoteChanged);
}

void MainWindow::setupMenu()
{
    auto *fileMenu = menuBar()->addMenu("&File");
    auto *helpMenu = menuBar()->addMenu("&Help");

    addAction = fileMenu->addAction("&Add Account");
    importAction = fileMenu->addAction("&Import...");
    exportAction = fileMenu->addAction("&Export...");
    settingsAction = fileMenu->addAction("&Settings");
    fileMenu->addSeparator();
    exitAction = fileMenu->addAction("E&xit");

    aboutAction = helpMenu->addAction("&About");

    connect(addAction, &QAction::triggered, this, &MainWindow::addAccount);
    connect(importAction, &QAction::triggered, this, &MainWindow::importAccounts);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportAccounts);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::openSettings);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::loadAccounts()
{
    if (!m_database) return;

    // Clear existing cards
    for (auto *card : accountCards) {
        accountCardsLayout->removeWidget(card);
        card->deleteLater();
    }
    accountCards.clear();

    auto accounts = m_database->getAccounts();

    // Sort: favorites first, then by issuer name
    std::sort(accounts.begin(), accounts.end(), [](const Account &a, const Account &b) {
        if (a.favorite != b.favorite) return a.favorite > b.favorite;
        return a.issuer < b.issuer;
    });

    for (const auto &account : accounts) {
        auto *card = new AccountCard(this);
        card->setAccount(account);
        card->installEventFilter(this);

        connect(card, &AccountCard::favoriteToggled, this, &MainWindow::onCardFavoriteToggled);
        connect(card, &AccountCard::clicked, this, [this, card]() {
            // Auto-copy OTP when card is clicked
            if (card == currentAccount) {
                copyOTP();
            }
        });

        accountCardsLayout->insertWidget(accountCardsLayout->count() - 1, card);
        accountCards.append(card);
    }

    accountCountLabel->setText(QString("%1 accounts").arg(accounts.size()));

    if (!accountCards.isEmpty()) {
        selectAccountCard(accountCards.first());
    }
}

void MainWindow::refreshCurrentAccount()
{
    if (currentAccount) {
        selectAccountCard(currentAccount);
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        for (auto *card : accountCards) {
            if (watched == card) {
                selectAccountCard(card);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::selectAccountCard(AccountCard *card)
{
    if (!card) return;

    // Save current note before switching
    if (currentAccount && m_database) {
        Account currentAcc = currentAccount->account();
        QString currentNote = notesTextEdit->toPlainText();
        if (currentNote != QString::fromStdString(currentAcc.notes)) {
            currentAcc.notes = currentNote.toStdString();
            m_database->updateAccount(currentAcc);
            currentAccount->setAccount(currentAcc);
        }
    }

    // Deselect all cards
    for (auto *c : accountCards) {
        c->setProperty("selected", false);
        c->style()->unpolish(c);
        c->style()->polish(c);
    }

    // Select this card
    card->setProperty("selected", true);
    card->style()->unpolish(card);
    card->style()->polish(card);

    currentAccount = card;

    const Account &acc = card->account();

    // Update header
    issuerLabel->setText(QString::fromStdString(acc.issuer));
    accountLabel->setText(QString::fromStdString(acc.username));

    // Update favorite button
    favoriteButton->setText(acc.favorite ? "★" : "☆");
    favoriteButton->setStyleSheet(acc.favorite
        ? "QPushButton { border: 1px solid #ffd700; border-radius: 6px; color: #ffd700; background: #21262d; font-size: 20px; } QPushButton:hover { background: #30363d; }"
        : "QPushButton { border: 1px solid #30363d; border-radius: 6px; color: #8b949e; background: #21262d; font-size: 20px; } QPushButton:hover { color: #ffd700; background: #30363d; border-color: #ffd700; }");

    // Update details
    issuerDetailLabel->setText(QString::fromStdString(acc.issuer));
    accountDetailLabel->setText(QString::fromStdString(acc.username));

    // Update detail icon
    loadDetailIcon(acc);

    if (secretVisible) {
        secretDetailLabel->setText(QString::fromStdString(acc.secret));
    } else {
        secretDetailLabel->setText("••••••••••••••••••••");
    }

    // Update settings
    algorithmDetailLabel->setText(QString::fromStdString(acc.algorithm));
    digitsDetailLabel->setText(QString::number(acc.digits));
    periodDetailLabel->setText(QString("%1 seconds").arg(acc.period));

    // Update notes - load from database
    notesTextEdit->blockSignals(true);
    notesTextEdit->setText(QString::fromStdString(acc.notes));
    notesTextEdit->blockSignals(false);

    // Auto-generate and show QR code for selected account
    showQRCode();

    updateOTP();
}

void MainWindow::updateOTP()
{
    if (!currentAccount) return;

    const Account &acc = currentAccount->account();

    std::string otp = TOTP::generate(acc.secret, acc.digits, acc.period);
    otpLabel->setText(QString::fromStdString(otp));

    int remaining = acc.period - (std::time(nullptr) % acc.period);
    timerLabel->setText(QString("%1s").arg(remaining));

    progressBar->setMaximum(acc.period);
    progressBar->setValue(remaining);

    // Update card
    currentAccount->setOTP(QString::fromStdString(otp));
    currentAccount->setRemainingTime(remaining, acc.period);

    // Update all cards
    for (auto *card : accountCards) {
        const Account &cardAcc = card->account();
        std::string cardOtp = TOTP::generate(cardAcc.secret, cardAcc.digits, cardAcc.period);
        int cardRemaining = cardAcc.period - (std::time(nullptr) % cardAcc.period);
        card->setOTP(QString::fromStdString(cardOtp));
        card->setRemainingTime(cardRemaining, cardAcc.period);
    }
}

void MainWindow::addAccount()
{
    AddAccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Account account = dialog.account();
        if (m_database->addAccount(account)) {
            loadAccounts();
            showNotification("Account added successfully");
        }
    }
}

void MainWindow::editAccount()
{
    if (!currentAccount) return;

    Account acc = currentAccount->account();
    AddAccountDialog dialog(this);
    dialog.setAccount(acc);
    dialog.setWindowTitle("Edit Account");

    if (dialog.exec() == QDialog::Accepted) {
        Account updated = dialog.account();
        updated.id = acc.id;
        if (m_database->updateAccount(updated)) {
            loadAccounts();
            showNotification("Account updated successfully");
        }
    }
}

void MainWindow::deleteAccount()
{
    if (!currentAccount) return;

    const Account &acc = currentAccount->account();

    DeleteConfirmDialog dialog(acc, this);
    if (dialog.exec() == QDialog::Accepted) {
        if (m_database->deleteAccount(acc.id)) {
            loadAccounts();
            showNotification("Account deleted successfully");
        }
    }
}

void MainWindow::copyOTP()
{
    if (!currentAccount) return;

    QString otp = otpLabel->text();
    QGuiApplication::clipboard()->setText(otp);
    showNotification("OTP copied to clipboard");
}

void MainWindow::showQRCode()
{
    if (!currentAccount) return;

    const Account &acc = currentAccount->account();

    // Generate TOTP URI
    std::string uri = "otpauth://totp/" + acc.issuer + ":" + acc.username +
                      "?secret=" + acc.secret +
                      "&issuer=" + acc.issuer +
                      "&algorithm=" + acc.algorithm +
                      "&digits=" + std::to_string(acc.digits) +
                      "&period=" + std::to_string(acc.period);

    // Generate QR code at exact display size (no border, no scaling needed)
    std::vector<uint8_t> pngData = QRCode::generatePNG(uri, 160);

    if (!pngData.empty()) {
        QPixmap pixmap;
        if (pixmap.loadFromData(pngData.data(), pngData.size(), "PNG")) {
            // No cropping or scaling - perfect 1:1 pixel mapping
            qrLabel->setPixmap(pixmap);
        }
    }

    // Show QR section
    qrLabel->parentWidget()->show();

    showQRButton->hide();
    hideQRButton->show();
}

void MainWindow::hideQRCode()
{
    // Hide QR section
    qrLabel->parentWidget()->hide();

    showQRButton->show();
    hideQRButton->hide();
}

void MainWindow::about()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::filterAccounts()
{
    QString filter = searchEdit->text().toLower();

    for (auto *card : accountCards) {
        const Account &acc = card->account();
        QString issuer = QString::fromStdString(acc.issuer).toLower();
        QString username = QString::fromStdString(acc.username).toLower();

        bool visible = filter.isEmpty() || issuer.contains(filter) || username.contains(filter);
        card->setVisible(visible);
    }
}

void MainWindow::toggleFavorite()
{
    if (!currentAccount || !m_database) return;

    Account acc = currentAccount->account();
    acc.favorite = !acc.favorite;

    if (m_database->updateAccount(acc)) {
        loadAccounts();
        showNotification(acc.favorite ? "Added to favorites" : "Removed from favorites");
    }
}

void MainWindow::toggleSecretVisibility()
{
    secretVisible = !secretVisible;
    showSecretButton->setText(secretVisible ? "🙈" : "👁");

    if (currentAccount) {
        const Account &acc = currentAccount->account();
        if (secretVisible) {
            secretDetailLabel->setText(QString::fromStdString(acc.secret));
        } else {
            secretDetailLabel->setText("••••••••••••••••••••");
        }
    }
}

void MainWindow::copySecret()
{
    if (!currentAccount) return;

    const Account &acc = currentAccount->account();
    QGuiApplication::clipboard()->setText(QString::fromStdString(acc.secret));
    showNotification("Secret copied to clipboard");
}

void MainWindow::saveNote()
{
    if (!currentAccount || !m_database) return;

    Account acc = currentAccount->account();
    QString noteText = notesTextEdit->toPlainText();
    acc.notes = noteText.toStdString();

    if (m_database->updateAccount(acc)) {
        // Update the card's internal account data
        currentAccount->setAccount(acc);
        showNotification("Note saved successfully");
    } else {
        showNotification("Failed to save note");
    }
}

void MainWindow::onNoteChanged()
{
    // Auto-save could be implemented here
}

void MainWindow::onCardFavoriteToggled(int accountId, bool isFavorite)
{
    if (!m_database) return;

    auto accounts = m_database->getAccounts();
    for (auto &acc : accounts) {
        if (acc.id == accountId) {
            acc.favorite = isFavorite;
            m_database->updateAccount(acc);
            break;
        }
    }
}

void MainWindow::openSettings()
{
    SettingsDialog dialog(this);

    // Load current settings
    SettingsDialog::Settings settings;
    settings.darkMode = true; // Currently always dark
    settings.theme = "Dark";
    settings.language = "English";
    settings.showNotifications = true;
    settings.copyOTPToClipboard = false;
    settings.startWithWindows = m_startWithWindows;
    settings.minimizeToTray = m_minimizeToTray;
    settings.autoMode = false;

    dialog.setSettings(settings);

    if (dialog.exec() == QDialog::Accepted) {
        auto newSettings = dialog.getSettings();

        // Apply start with windows
        if (newSettings.startWithWindows != m_startWithWindows) {
            m_startWithWindows = newSettings.startWithWindows;
            setStartWithWindows(m_startWithWindows);
            saveSettings();
            showNotification(m_startWithWindows ? "Start with Windows enabled" : "Start with Windows disabled");
        }

        // Apply minimize to tray
        if (newSettings.minimizeToTray != m_minimizeToTray) {
            m_minimizeToTray = newSettings.minimizeToTray;
            saveSettings();
            showNotification(m_minimizeToTray ? "Minimize to tray enabled" : "Minimize to tray disabled");
        }

        // Apply theme change based on theme combo selection
        if (newSettings.theme == "Light") {
            setStyleSheet(LightTheme::getStyleSheet());
            showNotification("Light theme applied");
        } else if (newSettings.theme == "Dark") {
            setStyleSheet(DarkTheme::getStyleSheet());
            showNotification("Dark theme applied");
        } else if (newSettings.theme == "Auto") {
            // Auto mode - detect system theme (for now default to dark)
            setStyleSheet(DarkTheme::getStyleSheet());
            showNotification("Auto theme applied (currently dark)");
        }
    }
}

void MainWindow::importAccounts()
{
    QMessageBox::information(this, "Import", "Import feature coming soon");
}

void MainWindow::exportAccounts()
{
    QMessageBox::information(this, "Export", "Export feature coming soon");
}

void MainWindow::showNotification(const QString &message)
{
    statusLabel->setText(message);
    statusLabel->setStyleSheet("color: #2ea043; font-size: 9pt;");

    QTimer::singleShot(3000, this, [this]() {
        statusLabel->setText("Database connected");
    });
}

void MainWindow::loadDetailIcon(const Account &account)
{
    QString iconPath = QString::fromStdString(account.iconPath);

    // Check if custom icon URL/path exists
    if (!iconPath.isEmpty()) {
        if (iconPath.startsWith("http://") || iconPath.startsWith("https://")) {
            // Check cache first
            QString cacheDir = QDir::homePath() + "/.config/totpmanager/icons";
            QString hash = QString(QCryptographicHash::hash(iconPath.toUtf8(), QCryptographicHash::Md5).toHex());
            QString cachedPath = cacheDir + "/" + hash + ".png";

            if (QFile::exists(cachedPath)) {
                loadDetailIconFromFile(cachedPath);
                return;
            }
        } else {
            // Local file
            if (QFile::exists(iconPath)) {
                loadDetailIconFromFile(iconPath);
                return;
            }
        }
    }

    // Fallback to issuer-based icon
    QString issuer = QString::fromStdString(account.issuer).toLower();
    QString iconDir = QDir::homePath() + "/.config/totpmanager/img/";
    QString fallbackPath;

    if (issuer.contains("google")) fallbackPath = iconDir + "google.png";
    else if (issuer.contains("github")) fallbackPath = iconDir + "github.png";
    else if (issuer.contains("microsoft")) fallbackPath = iconDir + "microsoft.png";
    else if (issuer.contains("discord")) fallbackPath = iconDir + "discord.png";
    else fallbackPath = iconDir + "default.png";

    if (QFile::exists(fallbackPath)) {
        loadDetailIconFromFile(fallbackPath);
    } else {
        // Text fallback
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        detailIconLabel->setFont(font);
        detailIconLabel->setText(issuer.isEmpty() ? "🔐" : QString(issuer[0].toUpper()));
        detailIconLabel->setPixmap(QPixmap());
    }
}

void MainWindow::loadDetailIconFromFile(const QString &filePath)
{
    QPixmap pixmap(filePath);
    if (!pixmap.isNull()) {
        // Create 80x80 circular icon with proper border
        QPixmap canvas(80, 80);
        canvas.fill(Qt::transparent);

        QPainter painter(&canvas);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw circular background with border
        painter.setBrush(QColor("#21262d"));
        painter.setPen(QPen(QColor("#30363d"), 2));
        painter.drawEllipse(1, 1, 78, 78);

        // Scale and clip icon to circle with padding
        QPixmap scaled = pixmap.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int x = (80 - scaled.width()) / 2;
        int y = (80 - scaled.height()) / 2;

        QPainterPath clipPath;
        clipPath.addEllipse(1, 1, 78, 78);
        painter.setClipPath(clipPath);
        painter.drawPixmap(x, y, scaled);
        painter.end();

        detailIconLabel->setPixmap(canvas);
        detailIconLabel->setText("");
    }
}

void MainWindow::setupSystemTray()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/app.png"));
    trayIcon->setToolTip("TOTP Manager");

    trayMenu = new QMenu(this);
    showAction = trayMenu->addAction("Show");
    trayMenu->addSeparator();
    quitAction = trayMenu->addAction("Quit");

    trayIcon->setContextMenu(trayMenu);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
    connect(showAction, &QAction::triggered, this, &MainWindow::showFromTray);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quitApplication);

    trayIcon->show();
}

void MainWindow::loadSettings()
{
    m_minimizeToTray = m_settings.value("minimizeToTray", false).toBool();
    m_startWithWindows = m_settings.value("startWithWindows", false).toBool();
}

void MainWindow::saveSettings()
{
    m_settings.setValue("minimizeToTray", m_minimizeToTray);
    m_settings.setValue("startWithWindows", m_startWithWindows);
}

void MainWindow::setStartWithWindows(bool enable)
{
#ifdef Q_OS_WIN
    QSettings startupSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (enable) {
        QString appPath = QCoreApplication::applicationFilePath();
        appPath = QDir::toNativeSeparators(appPath);
        startupSettings.setValue("TOTP Manager", "\"" + appPath + "\"");
    } else {
        startupSettings.remove("TOTP Manager");
    }
#endif
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_minimizeToTray && trayIcon->isVisible()) {
        hide();
        event->ignore();
        trayIcon->showMessage("TOTP Manager", "Application minimized to tray", QSystemTrayIcon::Information, 2000);
    } else {
        event->accept();
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (m_minimizeToTray && isMinimized()) {
            hide();
            event->ignore();
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        showFromTray();
    }
}

void MainWindow::showFromTray()
{
    showNormal();
    activateWindow();
    raise();
}

void MainWindow::quitApplication()
{
    QApplication::quit();
}
