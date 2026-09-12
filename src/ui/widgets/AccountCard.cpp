#include "AccountCard.h"

#include <QFont>
#include <QFile>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QMouseEvent>

AccountCard::AccountCard(QWidget *parent)
    : QFrame(parent), m_isFavorite(false)
{
    setObjectName("AccountCard");
    setFrameShape(QFrame::StyledPanel);
    setCursor(Qt::PointingHandCursor);
    setMinimumHeight(90);
    setMaximumHeight(90);

    //---------------------------------
    // Icon
    //---------------------------------

    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(40, 40);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setScaledContents(false);
    iconLabel->setStyleSheet("border-radius: 20px; background: #21262d;");

    // Default icon
    QFont iconFont;
    iconFont.setPointSize(16);
    iconFont.setBold(true);
    iconLabel->setFont(iconFont);
    iconLabel->setText("🔐");

    //---------------------------------
    // Issuer
    //---------------------------------

    issuerLabel = new QLabel("Issuer", this);
    QFont issuerFont;
    issuerFont.setPointSize(10);
    issuerFont.setBold(true);
    issuerLabel->setFont(issuerFont);
    issuerLabel->setStyleSheet("color: #c9d1d9;");

    //---------------------------------
    // Username
    //---------------------------------

    usernameLabel = new QLabel("Account", this);
    QFont userFont;
    userFont.setPointSize(8);
    usernameLabel->setFont(userFont);
    usernameLabel->setStyleSheet("color: #8b949e;");

    //---------------------------------
    // Favorite Button
    //---------------------------------

    favoriteButton = new QPushButton("☆", this);
    favoriteButton->setFlat(true);
    favoriteButton->setFixedSize(28, 28);
    favoriteButton->setStyleSheet("border: none; background: transparent; font-size: 20px; color: #8b949e;");

    connect(favoriteButton, &QPushButton::clicked, this, &AccountCard::onFavoriteClicked);

    //---------------------------------
    // OTP
    //---------------------------------

    otpLabel = new QLabel("--- ---", this);
    QFont otpFont;
    otpFont.setPointSize(15);
    otpFont.setBold(true);
    otpFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    otpLabel->setFont(otpFont);
    otpLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    otpLabel->setStyleSheet("color: #58a6ff;");

    //---------------------------------
    // Progress
    //---------------------------------

    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(false);
    progressBar->setMinimum(0);
    progressBar->setMaximum(30);
    progressBar->setValue(30);
    progressBar->setFixedHeight(4);
    progressBar->setStyleSheet(R"(
        QProgressBar {
            border: none;
            background: #21262d;
            border-radius: 2px;
        }
        QProgressBar::chunk {
            background: #238636;
            border-radius: 2px;
        }
    )");

    //---------------------------------
    // Time
    //---------------------------------

    timeLabel = new QLabel("30s", this);
    timeLabel->setAlignment(Qt::AlignRight);
    QFont timeFont;
    timeFont.setPointSize(8);
    timeLabel->setFont(timeFont);
    timeLabel->setStyleSheet("color: #8b949e;");

    //---------------------------------
    // Layout
    //---------------------------------

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 8, 10, 8);
    mainLayout->setSpacing(6);

    auto *topLayout = new QHBoxLayout;
    auto *textLayout = new QVBoxLayout;
    textLayout->setSpacing(2);
    textLayout->addWidget(issuerLabel);
    textLayout->addWidget(usernameLabel);

    topLayout->addWidget(iconLabel);
    topLayout->addSpacing(8);
    topLayout->addLayout(textLayout, 1);
    topLayout->addSpacing(4);
    topLayout->addWidget(favoriteButton);

    auto *middleLayout = new QHBoxLayout;
    middleLayout->addStretch();
    middleLayout->addWidget(otpLabel);

    auto *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(progressBar, 1);
    bottomLayout->addSpacing(6);
    bottomLayout->addWidget(timeLabel);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(bottomLayout);

    //---------------------------------
    // Style
    //---------------------------------

    setStyleSheet(R"(
        #AccountCard {
            background: #0d1117;
            border: 1px solid #21262d;
            border-radius: 8px;
        }
        #AccountCard:hover {
            border: 1px solid #1f6feb;
            background: #161b22;
        }
    )");
}

void AccountCard::setAccount(const Account &account)
{
    m_account = account;
    m_isFavorite = account.favorite;

    issuerLabel->setText(QString::fromStdString(account.issuer));
    usernameLabel->setText(QString::fromStdString(account.username));

    // Load custom icon if available
    if (!account.iconPath.empty()) {
        setCustomIcon(QString::fromStdString(account.iconPath));
    } else {
        setIssuerIcon(QString::fromStdString(account.issuer));
    }

    updateFavoriteButton();
}

void AccountCard::setOTP(const QString &otp)
{
    // Format OTP as "XXX XXX" for 6 digits or "XXXX XXXX" for 8 digits
    QString formatted = otp;
    if (otp.length() == 6) {
        formatted = otp.left(3) + " " + otp.right(3);
    } else if (otp.length() == 8) {
        formatted = otp.left(4) + " " + otp.right(4);
    }
    otpLabel->setText(formatted);
}

void AccountCard::setRemainingTime(int remaining, int period)
{
    progressBar->setMaximum(period);
    progressBar->setValue(remaining);
    timeLabel->setText(QString("%1s").arg(remaining));

    // Change color based on remaining time
    if (remaining <= 5) {
        progressBar->setStyleSheet(R"(
            QProgressBar {
                border: none;
                background: #1a1f26;
                border-radius: 3px;
            }
            QProgressBar::chunk {
                background: #f85149;
                border-radius: 3px;
            }
        )");
    } else if (remaining <= 10) {
        progressBar->setStyleSheet(R"(
            QProgressBar {
                border: none;
                background: #1a1f26;
                border-radius: 3px;
            }
            QProgressBar::chunk {
                background: #d29922;
                border-radius: 3px;
            }
        )");
    } else {
        progressBar->setStyleSheet(R"(
            QProgressBar {
                border: none;
                background: #1a1f26;
                border-radius: 3px;
            }
            QProgressBar::chunk {
                background: #2ea043;
                border-radius: 3px;
            }
        )");
    }
}

Account AccountCard::account() const
{
    return m_account;
}

void AccountCard::setIssuerIcon(const QString &issuer)
{
    QString lowerIssuer = issuer.toLower();
    QString iconPath;
    QString homeDir = QDir::homePath();
    QString iconDir = homeDir + "/.config/totpmanager/img/";

    // Check for custom icon files
    if (lowerIssuer.contains("google")) {
        iconPath = iconDir + "google.png";
    } else if (lowerIssuer.contains("github")) {
        iconPath = iconDir + "github.png";
    } else if (lowerIssuer.contains("microsoft")) {
        iconPath = iconDir + "microsoft.png";
    } else if (lowerIssuer.contains("discord")) {
        iconPath = iconDir + "discord.png";
    } else if (lowerIssuer.contains("binance")) {
        iconPath = iconDir + "binance.png";
    } else if (lowerIssuer.contains("steam")) {
        iconPath = iconDir + "steam.png";
    } else if (lowerIssuer.contains("telegram")) {
        iconPath = iconDir + "telegram.png";
    } else if (lowerIssuer.contains("facebook")) {
        iconPath = iconDir + "facebook.png";
    } else if (lowerIssuer.contains("aws")) {
        iconPath = iconDir + "aws.png";
    } else if (lowerIssuer.contains("twitter") || lowerIssuer.contains("x.com")) {
        iconPath = iconDir + "twitter.png";
    } else {
        iconPath = iconDir + "default.png";
    }

    // Load icon or fallback to emoji
    QPixmap iconPixmap(iconPath);
    if (!iconPixmap.isNull()) {
        // Create 40x40 canvas with circular border
        QPixmap canvas(40, 40);
        canvas.fill(Qt::transparent);

        QPainter painter(&canvas);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw circular background
        painter.setBrush(QColor("#21262d"));
        painter.setPen(QPen(QColor("#30363d"), 1));
        painter.drawEllipse(0, 0, 40, 40);

        // Scale and clip icon to circle
        QPixmap scaled = iconPixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int x = (40 - scaled.width()) / 2;
        int y = (40 - scaled.height()) / 2;

        QPainterPath clipPath;
        clipPath.addEllipse(0, 0, 40, 40);
        painter.setClipPath(clipPath);
        painter.drawPixmap(x, y, scaled);
        painter.end();

        iconLabel->setPixmap(canvas);
        iconLabel->setStyleSheet("");
        iconLabel->setText("");
    } else {
        // Fallback to emoji if no image found
        QString icon = "🔐";
        if (lowerIssuer.contains("google")) icon = "G";
        else if (lowerIssuer.contains("github")) icon = "G";
        else if (lowerIssuer.contains("microsoft")) icon = "M";
        else if (lowerIssuer.contains("discord")) icon = "D";
        else if (lowerIssuer.contains("binance")) icon = "B";
        else if (lowerIssuer.contains("steam")) icon = "S";
        else if (lowerIssuer.contains("telegram")) icon = "T";
        else if (lowerIssuer.contains("facebook")) icon = "F";
        else if (lowerIssuer.contains("aws")) icon = "A";
        else if (lowerIssuer.contains("twitter") || lowerIssuer.contains("x.com")) icon = "X";

        QFont iconFont;
        iconFont.setPointSize(16);
        iconFont.setBold(true);
        iconLabel->setFont(iconFont);
        iconLabel->setText(icon);
        iconLabel->setStyleSheet("background: #21262d; border-radius: 20px; color: #58a6ff;");
    }
}

void AccountCard::updateFavoriteButton()
{
    if (m_isFavorite) {
        favoriteButton->setText("★");
        favoriteButton->setStyleSheet("border: none; background: transparent; font-size: 20px; color: #ffd700;");
    } else {
        favoriteButton->setText("☆");
        favoriteButton->setStyleSheet("border: none; background: transparent; font-size: 20px; color: #8b949e;");
    }
}

void AccountCard::onFavoriteClicked()
{
    m_isFavorite = !m_isFavorite;
    m_account.favorite = m_isFavorite;
    updateFavoriteButton();
    emit favoriteToggled(m_account.id, m_isFavorite);
}

void AccountCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QFrame::mousePressEvent(event);
}