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

#include <ctime>

#include "totp.h"
#include "dialogs/AddAccountDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupMenu();

    timer = new QTimer(this);

    connect(timer,
        &QTimer::timeout,
        this,
        &MainWindow::updateOTP);

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
    resize(1200, 700);
    setMinimumSize(1000, 650);
    setWindowTitle("TOTP Manager");

    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    central = centralWidget;

    auto *rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(15, 15, 15, 15);
    rootLayout->setSpacing(15);

    auto *leftPanel = new QFrame(this);
    leftPanel->setFixedWidth(320);
    leftPanel->setObjectName("leftPanel");

    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(10);

    auto *rightPanel = new QFrame(this);
    rightPanel->setObjectName("rightPanel");

    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(15);

    rootLayout->addWidget(leftPanel);
    rootLayout->addWidget(rightPanel, 1);

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Search accounts...");
    leftLayout->addWidget(searchEdit);

    accountList = new QListWidget(this);
    accountList->setSpacing(4);
    accountList->setAlternatingRowColors(false);
    accountList->setSelectionMode(QAbstractItemView::SingleSelection);
    leftLayout->addWidget(accountList, 1);

    auto *buttonLayout = new QHBoxLayout;
    addButton = new QPushButton("Add", this);
    editButton = new QPushButton("Edit", this);
    deleteButton = new QPushButton("Delete", this);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    leftLayout->addLayout(buttonLayout);

    issuerLabel = new QLabel("Issuer", this);
    accountLabel = new QLabel("Account", this);
    otpLabel = new QLabel("------", this);

    QFont otpFont;
    otpFont.setPointSize(34);
    otpFont.setBold(true);
    otpLabel->setFont(otpFont);
    otpLabel->setAlignment(Qt::AlignCenter);

    timerLabel = new QLabel("30 seconds", this);
    timerLabel->setAlignment(Qt::AlignCenter);

    progressBar = new QProgressBar(this);
    progressBar->setMaximum(30);
    progressBar->setValue(30);
    progressBar->setTextVisible(false);

    qrLabel = new QLabel(this);
    qrLabel->setFixedSize(240, 240);
    qrLabel->setAlignment(Qt::AlignCenter);
    qrLabel->setScaledContents(true);

    uriLabel = new QLabel(this);
    uriLabel->setWordWrap(true);

    copyButton = new QPushButton("Copy Code", this);
    showQRButton = new QPushButton("Show QR", this);
    hideQRButton = new QPushButton("Hide QR", this);

    auto *qrButtons = new QHBoxLayout;
    qrButtons->addWidget(copyButton);
    qrButtons->addWidget(showQRButton);
    qrButtons->addWidget(hideQRButton);

    rightLayout->addWidget(issuerLabel);
    rightLayout->addWidget(accountLabel);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(otpLabel);
    rightLayout->addWidget(timerLabel);
    rightLayout->addWidget(progressBar);
    rightLayout->addSpacing(20);
    rightLayout->addWidget(qrLabel, 0, Qt::AlignCenter);
    rightLayout->addWidget(uriLabel);
    rightLayout->addLayout(qrButtons);
}

void MainWindow::setupMenu()
{
    fileMenu = menuBar()->addMenu("File");
    accountMenu = menuBar()->addMenu("Account");
    helpMenu = menuBar()->addMenu("Help");

    addAction = fileMenu->addAction("Add Account");
    importAction = fileMenu->addAction("Import");
    exportAction = fileMenu->addAction("Export");
    exitAction = fileMenu->addAction("Exit");
    aboutAction = helpMenu->addAction("About");

    connect(addAction, &QAction::triggered, this, &MainWindow::addAccount);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::copyOTP);
    connect(showQRButton, &QPushButton::clicked, this, &MainWindow::showQRCode);
    connect(hideQRButton, &QPushButton::clicked, this, &MainWindow::hideQRCode);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addAccount);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editAccount);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteAccount);
    connect(accountList, &QListWidget::itemSelectionChanged, this, &MainWindow::accountSelectionChanged);
}

void MainWindow::loadAccounts()
{
    if (!m_database || !accountList) {
        return;
    }

accountList->clear();

auto accounts =
    m_database->getAccounts();

for (const auto &account : accounts)
{
    auto *item =
    new QListWidgetItem(
        QString::fromStdString(
            account.issuer +
            "\n" +
            account.username
        )
    );

    item->setData(
        Qt::UserRole,
        account.id
    );

    accountList->addItem(item);
}

if (accountList->count() > 0)
{
    accountList->setCurrentRow(0);
}
}

void MainWindow::refreshCurrentAccount()
{
    if (!m_database)
        return;

    QListWidgetItem *item =
        accountList->currentItem();

    if (!item)
        return;

    int id =
        item->data(Qt::UserRole).toInt();

    auto accounts =
        m_database->getAccounts();

    for (const auto &account : accounts)
    {
        if (account.id != id)
            continue;

        issuerLabel->setText(
            QString::fromStdString(account.issuer));

        accountLabel->setText(
            QString::fromStdString(account.username));

        uriLabel->setText(
            QString::fromStdString(account.secret));

        return;
    }
}

void MainWindow::updateOTP()
{
    if (!m_database)
        return;

    QListWidgetItem *item =
        accountList->currentItem();

    if (!item)
        return;

    int id =
        item->data(Qt::UserRole).toInt();

    auto accounts =
        m_database->getAccounts();

    for (const auto &account : accounts)
    {
        if (account.id != id)
            continue;

        std::string otp =
            TOTP::generate(
                account.secret,
                account.digits,
                account.period);

        otpLabel->setText(
            QString::fromStdString(otp));

        int remaining =
            account.period -
            (std::time(nullptr) % account.period);

        timerLabel->setText(
            QString("%1 seconds").arg(remaining));

        progressBar->setMaximum(account.period);
        progressBar->setValue(remaining);

        return;
    }
}

void MainWindow::addAccount()
{
    if (!m_database)
        return;

    AddAccountDialog dialog(this);

    if (dialog.exec() != QDialog::Accepted)
        return;

    Account account = dialog.account();

    if (m_database->addAccount(account))
    {
        loadAccounts();

        QMessageBox::information(
            this,
            "Success",
            "Account added successfully."
        );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Database Error",
            "Failed to save account."
        );
    }
}

void MainWindow::editAccount()
{
    if (!m_database)
        return;

    QListWidgetItem *item =
        accountList->currentItem();

    if (!item)
    {
        QMessageBox::information(
            this,
            "Edit Account",
            "Please select an account."
        );
        return;
    }

    int id =
        item->data(Qt::UserRole).toInt();

    auto accounts =
        m_database->getAccounts();

    for (auto account : accounts)
    {
        if (account.id != id)
            continue;

        AddAccountDialog dialog(this);

        dialog.setAccount(account);

        if (dialog.exec() == QDialog::Accepted)
        {
            Account updated =
                dialog.account();

            updated.id = id;

            if (m_database->updateAccount(updated))
            {
                loadAccounts();

                for (int i = 0; i < accountList->count(); i++)
                {
                    if (accountList->item(i)->data(Qt::UserRole).toInt() == id)
                    {
                        accountList->setCurrentRow(i);
                        break;
                    }
                }

                refreshCurrentAccount();
            }
            else
            {
                QMessageBox::warning(
                    this,
                    "Database",
                    "Failed to update account."
                );
            }
        }

        return;
    }
}

void MainWindow::deleteAccount()
{
    if (!m_database)
        return;

    QListWidgetItem *item =
        accountList->currentItem();

    if (!item)
        return;

    int id =
        item->data(Qt::UserRole).toInt();

    QString name =
        item->text();

    auto reply =
        QMessageBox::question(
            this,
            "Delete Account",
            "Delete \"" + name + "\"?",
            QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
        return;

    if (m_database->deleteAccount(id))
    {
        loadAccounts();

        if (accountList->count() > 0)
            accountList->setCurrentRow(0);

        refreshCurrentAccount();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Database",
            "Failed to delete account."
        );
    }
}

void MainWindow::copyOTP()
{
    QGuiApplication::clipboard()->setText(otpLabel->text());
}

void MainWindow::showQRCode()
{
    QMessageBox::information(this, "QR Code", "QR code display is not implemented yet.");
}

void MainWindow::hideQRCode()
{
    qrLabel->clear();
}

void MainWindow::accountSelectionChanged()
{
    refreshCurrentAccount();
}

void MainWindow::about()
{
    QMessageBox::about(this, "About", "TOTP Manager\nA simple Qt-based TOTP manager.");
}
