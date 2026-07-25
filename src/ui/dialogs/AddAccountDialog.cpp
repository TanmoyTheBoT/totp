#include "AddAccountDialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>

AddAccountDialog::AddAccountDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add Account");
    setModal(true);
    resize(450, 360);

    issuerEdit = new QLineEdit(this);
    usernameEdit = new QLineEdit(this);
    secretEdit = new QLineEdit(this);

    issuerEdit->setPlaceholderText("Google");
    usernameEdit->setPlaceholderText("user@gmail.com");
    secretEdit->setPlaceholderText("Base32 Secret");

    algorithmBox = new QComboBox(this);
    algorithmBox->addItems({
        "SHA1",
        "SHA256",
        "SHA512"
    });

    digitsBox = new QComboBox(this);
    digitsBox->addItems({
        "6",
        "8"
    });

    periodBox = new QComboBox(this);
    periodBox->addItems({
        "30",
        "60"
    });

    auto *form = new QFormLayout;

    form->addRow("Issuer", issuerEdit);
    form->addRow("Account", usernameEdit);
    form->addRow("Secret Key", secretEdit);
    form->addRow("Algorithm", algorithmBox);
    form->addRow("Digits", digitsBox);
    form->addRow("Period", periodBox);

    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);

    auto *buttons = new QHBoxLayout;
    buttons->addStretch();
    buttons->addWidget(saveButton);
    buttons->addWidget(cancelButton);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addStretch();
    layout->addLayout(buttons);

    connect(
        saveButton,
        &QPushButton::clicked,
        this,
        &AddAccountDialog::save);

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &QDialog::reject);
}

void AddAccountDialog::setAccount(const Account& account)
{
    m_account = account;

    issuerEdit->setText(
        QString::fromStdString(account.issuer));

    usernameEdit->setText(
        QString::fromStdString(account.username));

    secretEdit->setText(
        QString::fromStdString(account.secret));

    algorithmBox->setCurrentText(
        QString::fromStdString(account.algorithm));

    digitsBox->setCurrentText(
        QString::number(account.digits));

    periodBox->setCurrentText(
        QString::number(account.period));

    setWindowTitle("Edit Account");

    saveButton->setText("Update");
}

void AddAccountDialog::save()
{
    if (issuerEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Validation Error",
            "Issuer cannot be empty.");
        return;
    }

    if (usernameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Validation Error",
            "Account name cannot be empty.");
        return;
    }

    if (secretEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Validation Error",
            "Secret key cannot be empty.");
        return;
    }

    m_account.issuer =
        issuerEdit->text().trimmed().toStdString();

    m_account.username =
        usernameEdit->text().trimmed().toStdString();

    m_account.secret =
        secretEdit->text().trimmed().toUpper().toStdString();

    m_account.algorithm =
        algorithmBox->currentText().toStdString();

    m_account.digits =
        digitsBox->currentText().toInt();

    m_account.period =
        periodBox->currentText().toInt();

    accept();
}

Account AddAccountDialog::account() const
{
    return m_account;
}