#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "account.h"

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);

    // Return the account after Add/Edit
    Account account() const;

    // Load an existing account into the dialog
    void setAccount(const Account& account);

private slots:
    void save();

private:
    //==============================
    // Input Fields
    //==============================

    QLineEdit* issuerEdit;
    QLineEdit* usernameEdit;
    QLineEdit* secretEdit;

    QComboBox* algorithmBox;
    QComboBox* digitsBox;
    QComboBox* periodBox;

    //==============================
    // Buttons
    //==============================

    QPushButton* saveButton;
    QPushButton* cancelButton;

    //==============================
    // Data
    //==============================

    Account m_account;
};

#endif // ADDACCOUNTDIALOG_H