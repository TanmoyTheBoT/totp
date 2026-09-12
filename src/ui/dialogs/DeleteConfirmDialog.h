#ifndef DELETECONFIRMDIALOG_H
#define DELETECONFIRMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "account.h"

class DeleteConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteConfirmDialog(const Account& account, QWidget *parent = nullptr);

private:
    void setupUI();

    Account m_account;
    QLabel* warningLabel;
    QLabel* accountLabel;
    QLabel* messageLabel;
    QPushButton* deleteButton;
    QPushButton* cancelButton;
};

#endif // DELETECONFIRMDIALOG_H
