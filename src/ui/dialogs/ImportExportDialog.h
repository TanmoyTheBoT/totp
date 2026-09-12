#ifndef IMPORTEXPORTDIALOG_H
#define IMPORTEXPORTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = nullptr);

    enum ImportType {
        FromFile,
        FromCSV,
        FromText
    };

    ImportType getImportType() const { return m_importType; }
    QString getFilePath() const { return m_filePath; }

private slots:
    void onFromFileClicked();
    void onFromCSVClicked();
    void onFromTextClicked();
    void onBrowseClicked();
    void onImportClicked();

private:
    void setupUI();

    QPushButton* fromFileButton;
    QPushButton* fromCSVButton;
    QPushButton* fromTextButton;
    QPushButton* browseButton;
    QPushButton* importButton;
    QPushButton* cancelButton;

    QLabel* filePathLabel;
    QLabel* dropLabel;

    ImportType m_importType;
    QString m_filePath;
};

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);

    enum ExportType {
        ToFile,
        ToCSV,
        ToText
    };

    ExportType getExportType() const { return m_exportType; }
    QString getFilePath() const { return m_filePath; }

private slots:
    void onToFileClicked();
    void onToCSVClicked();
    void onToTextClicked();
    void onExportClicked();

private:
    void setupUI();

    QPushButton* toFileButton;
    QPushButton* toCSVButton;
    QPushButton* toTextButton;
    QPushButton* exportButton;
    QPushButton* cancelButton;

    QLabel* descLabel;

    ExportType m_exportType;
    QString m_filePath;
};

#endif // IMPORTEXPORTDIALOG_H
