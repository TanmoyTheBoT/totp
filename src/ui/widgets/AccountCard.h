#ifndef ACCOUNTCARD_H
#define ACCOUNTCARD_H

#include <QFrame>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "account.h"

class AccountCard : public QFrame
{
    Q_OBJECT

public:
    explicit AccountCard(QWidget *parent = nullptr);

    void setAccount(const Account &account);

    void setOTP(const QString &otp);

    void setRemainingTime(int remaining, int period);

    Account account() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();
    void copyRequested(const QString &otp);
    void favoriteToggled(int accountId, bool isFavorite);

private slots:
    void onFavoriteClicked();

private:
    void setIssuerIcon(const QString &issuer);
    void setCustomIcon(const QString &iconPath);
    void updateFavoriteButton();
    void downloadIcon(const QString &url);

    Account m_account;
    bool m_isFavorite;

    //=========================
    // Top Row
    //=========================

    QLabel *iconLabel;

    QLabel *issuerLabel;

    QLabel *usernameLabel;

    QPushButton *favoriteButton;

    //=========================
    // OTP
    //=========================

    QLabel *otpLabel;

    //=========================
    // Bottom
    //=========================

    QProgressBar *progressBar;

    QLabel *timeLabel;
};

#endif