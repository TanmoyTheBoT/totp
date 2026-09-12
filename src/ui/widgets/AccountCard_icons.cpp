#include "AccountCard.h"
#include <QFile>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QPainter>
#include <QPainterPath>
#include <QColor>

void AccountCard::setCustomIcon(const QString &iconPath)
{
    if (iconPath.isEmpty()) {
        setIssuerIcon(QString::fromStdString(m_account.issuer));
        return;
    }

    // Check if it's a URL
    if (iconPath.startsWith("http://") || iconPath.startsWith("https://")) {
        // Check cache first
        QString cacheDir = QDir::homePath() + "/.config/totpmanager/icons";
        QDir().mkpath(cacheDir);

        QString hash = QString(QCryptographicHash::hash(iconPath.toUtf8(), QCryptographicHash::Md5).toHex());
        QString cachedPath = cacheDir + "/" + hash + ".png";

        if (QFile::exists(cachedPath)) {
            // Load from cache
            QPixmap pixmap(cachedPath);
            if (!pixmap.isNull()) {
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
                QPixmap scaled = pixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
                return;
            }
        }

        // Download the icon
        downloadIcon(iconPath);
    } else {
        // Local file path
        QPixmap pixmap(iconPath);
        if (!pixmap.isNull()) {
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
            QPixmap scaled = pixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
            setIssuerIcon(QString::fromStdString(m_account.issuer));
        }
    }
}

void AccountCard::downloadIcon(const QString &url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, [this, url](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);

            if (!pixmap.isNull()) {
                // Cache the icon
                QString cacheDir = QDir::homePath() + "/.config/totpmanager/icons";
                QString hash = QString(QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5).toHex());
                QString cachedPath = cacheDir + "/" + hash + ".png";

                pixmap.save(cachedPath, "PNG");

                // Display the icon with circular border
                QPixmap canvas(40, 40);
                canvas.fill(Qt::transparent);

                QPainter painter(&canvas);
                painter.setRenderHint(QPainter::Antialiasing);

                // Draw circular background
                painter.setBrush(QColor("#21262d"));
                painter.setPen(QPen(QColor("#30363d"), 1));
                painter.drawEllipse(0, 0, 40, 40);

                // Scale and clip icon to circle
                QPixmap scaled = pixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
            }
        } else {
            // Fallback to default icon on error
            setIssuerIcon(QString::fromStdString(m_account.issuer));
        }

        reply->deleteLater();
    });

    QNetworkRequest request(url);
    manager->get(request);
}
