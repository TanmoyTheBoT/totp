#include "DeleteConfirmDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

DeleteConfirmDialog::DeleteConfirmDialog(const Account& account, QWidget *parent)
    : QDialog(parent), m_account(account)
{
    setWindowTitle("Delete Account");
    setModal(true);
    resize(400, 250);

    setupUI();
}

void DeleteConfirmDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Warning icon and title
    auto *headerLayout = new QHBoxLayout;

    auto *iconLabel = new QLabel("⚠");
    QFont iconFont;
    iconFont.setPointSize(32);
    iconLabel->setFont(iconFont);
    iconLabel->setStyleSheet("color: #f85149;");

    auto *titleLayout = new QVBoxLayout;
    warningLabel = new QLabel("Are you sure you want to delete this account?");
    QFont titleFont;
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    warningLabel->setFont(titleFont);

    accountLabel = new QLabel(QString::fromStdString(m_account.issuer));
    accountLabel->setStyleSheet("color: #58a6ff; font-size: 11pt;");

    auto *usernameLabel = new QLabel(QString::fromStdString(m_account.username));
    usernameLabel->setStyleSheet("color: #8b949e;");

    titleLayout->addWidget(warningLabel);
    titleLayout->addWidget(accountLabel);
    titleLayout->addWidget(usernameLabel);

    headerLayout->addWidget(iconLabel);
    headerLayout->addSpacing(10);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(20);

    // Message
    messageLabel = new QLabel("This action cannot be undone.");
    messageLabel->setStyleSheet("color: #f85149; font-weight: bold;");

    mainLayout->addWidget(messageLabel);
    mainLayout->addStretch();

    // Buttons
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel", this);
    deleteButton = new QPushButton("Delete", this);

    deleteButton->setStyleSheet("background-color: #da3633; color: white; font-weight: bold;");
    deleteButton->setDefault(false);
    cancelButton->setDefault(true);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(deleteButton);

    mainLayout->addLayout(buttonLayout);

    connect(deleteButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}
