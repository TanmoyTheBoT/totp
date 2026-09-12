#ifndef SHOWQRDIALOG_H
#define SHOWQRDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "account.h"

class ShowQRDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowQRDialog(const Account& account, QWidget *parent = nullptr);

private:
    void setupUI();
    void generateQR();

    Account m_account;
    QLabel* qrLabel;
    QLabel* uriLabel;
    QLabel* secretLabel;
    QPushButton* closeButton;
};

#endif // SHOWQRDIALOG_H
