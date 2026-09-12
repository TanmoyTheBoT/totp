#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QPixmap>

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
    void switchToManual();
    void switchToScan();
    void browseQRImage();

private:
    void setupUI();

    //==============================
    // Mode Selection
    //==============================
    QPushButton* manualButton;
    QPushButton* scanQRButton;
    QStackedWidget* stackedWidget;
    bool scanMode;

    //==============================
    // Manual Input Fields
    //==============================
    QLineEdit* issuerEdit;
    QLineEdit* usernameEdit;
    QLineEdit* secretEdit;
    QLineEdit* iconUrlEdit;

    QComboBox* algorithmBox;
    QComboBox* digitsBox;
    QComboBox* periodBox;

    //==============================
    // QR Scan Fields
    //==============================
    QLabel* qrScanLabel;
    QPushButton* browseQRButton;

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