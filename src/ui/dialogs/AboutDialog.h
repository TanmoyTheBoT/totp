#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    void setupUI();

    QLabel* appIconLabel;
    QLabel* appNameLabel;
    QLabel* versionLabel;
    QLabel* descriptionLabel;
    QLabel* websiteLabel;
    QLabel* developerLabel;
    QLabel* licenseLabel;
    QLabel* buildDateLabel;
    QLabel* featuresLabel;
    QPushButton* closeButton;
};

#endif // ABOUTDIALOG_H
