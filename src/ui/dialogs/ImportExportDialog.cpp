#include "ImportExportDialog.h"
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>

// ============================================
// Import Dialog
// ============================================

ImportDialog::ImportDialog(QWidget *parent)
    : QDialog(parent), m_importType(FromFile)
{
    setWindowTitle("Import Accounts");
    setModal(true);
    resize(550, 450);

    setupUI();
}

void ImportDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Title
    auto *titleLabel = new QLabel("Import Accounts");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);

    // From File
    auto *fileGroup = new QGroupBox("From File");
    auto *fileLayout = new QVBoxLayout(fileGroup);

    fromFileButton = new QPushButton("Import from a backup file");
    fromFileButton->setCheckable(true);
    fromFileButton->setChecked(true);

    QLabel *fileDesc = new QLabel("Import from a backup file");
    fileDesc->setStyleSheet("color: #8b949e; font-size: 9pt;");

    fileLayout->addWidget(fromFileButton);
    fileLayout->addWidget(fileDesc);

    // From CSV
    auto *csvGroup = new QGroupBox("From CSV");
    auto *csvLayout = new QVBoxLayout(csvGroup);

    fromCSVButton = new QPushButton("Import from CSV file");
    fromCSVButton->setCheckable(true);

    QLabel *csvDesc = new QLabel("Import from CSV file");
    csvDesc->setStyleSheet("color: #8b949e; font-size: 9pt;");

    csvLayout->addWidget(fromCSVButton);
    csvLayout->addWidget(csvDesc);

    // From Text
    auto *textGroup = new QGroupBox("From Text");
    auto *textLayout = new QVBoxLayout(textGroup);

    fromTextButton = new QPushButton("Import from text data");
    fromTextButton->setCheckable(true);

    QLabel *textDesc = new QLabel("Import from text data");
    textDesc->setStyleSheet("color: #8b949e; font-size: 9pt;");

    textLayout->addWidget(fromTextButton);
    textLayout->addWidget(textDesc);

    mainLayout->addWidget(fileGroup);
    mainLayout->addWidget(csvGroup);
    mainLayout->addWidget(textGroup);

    // Drop zone
    dropLabel = new QLabel("Drag and drop file here\nor");
    dropLabel->setAlignment(Qt::AlignCenter);
    dropLabel->setStyleSheet("border: 2px dashed #2d3139; border-radius: 8px; padding: 30px; color: #8b949e;");

    browseButton = new QPushButton("Browse File");

    mainLayout->addWidget(dropLabel);
    mainLayout->addWidget(browseButton, 0, Qt::AlignCenter);

    mainLayout->addStretch();

    // Buttons
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel");
    importButton = new QPushButton("Import");
    importButton->setDefault(true);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(importButton);

    mainLayout->addLayout(buttonLayout);

    connect(fromFileButton, &QPushButton::clicked, this, &ImportDialog::onFromFileClicked);
    connect(fromCSVButton, &QPushButton::clicked, this, &ImportDialog::onFromCSVClicked);
    connect(fromTextButton, &QPushButton::clicked, this, &ImportDialog::onFromTextClicked);
    connect(browseButton, &QPushButton::clicked, this, &ImportDialog::onBrowseClicked);
    connect(importButton, &QPushButton::clicked, this, &ImportDialog::onImportClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ImportDialog::onFromFileClicked()
{
    m_importType = FromFile;
    fromFileButton->setChecked(true);
    fromCSVButton->setChecked(false);
    fromTextButton->setChecked(false);
}

void ImportDialog::onFromCSVClicked()
{
    m_importType = FromCSV;
    fromFileButton->setChecked(false);
    fromCSVButton->setChecked(true);
    fromTextButton->setChecked(false);
}

void ImportDialog::onFromTextClicked()
{
    m_importType = FromText;
    fromFileButton->setChecked(false);
    fromCSVButton->setChecked(false);
    fromTextButton->setChecked(true);
}

void ImportDialog::onBrowseClicked()
{
    QString filter;
    if (m_importType == FromFile) {
        filter = "Backup Files (*.json *.totp);;All Files (*)";
    } else if (m_importType == FromCSV) {
        filter = "CSV Files (*.csv);;All Files (*)";
    } else {
        filter = "Text Files (*.txt);;All Files (*)";
    }

    m_filePath = QFileDialog::getOpenFileName(this, "Select File", "", filter);

    if (!m_filePath.isEmpty()) {
        dropLabel->setText(m_filePath);
    }
}

void ImportDialog::onImportClicked()
{
    if (m_filePath.isEmpty()) {
        onBrowseClicked();
        if (m_filePath.isEmpty()) {
            return;
        }
    }

    accept();
}

// ============================================
// Export Dialog
// ============================================

ExportDialog::ExportDialog(QWidget *parent)
    : QDialog(parent), m_exportType(ToFile)
{
    setWindowTitle("Export Accounts");
    setModal(true);
    resize(550, 400);

    setupUI();
}

void ExportDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Title
    auto *titleLabel = new QLabel("Export Accounts");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);

    // To File
    auto *fileGroup = new QGroupBox("To File");
    auto *fileLayout = new QVBoxLayout(fileGroup);

    toFileButton = new QPushButton("Export to encrypted file");
    toFileButton->setCheckable(true);
    toFileButton->setChecked(true);
    toFileButton->setIcon(QIcon(":/icons/file.png"));

    QLabel *fileDesc = new QLabel("Export to encrypted file");
    fileDesc->setStyleSheet("color: #8b949e; font-size: 9pt;");

    fileLayout->addWidget(toFileButton);
    fileLayout->addWidget(fileDesc);

    // To CSV
    auto *csvGroup = new QGroupBox("To CSV");
    auto *csvLayout = new QVBoxLayout(csvGroup);

    toCSVButton = new QPushButton("Export to CSV file");
    toCSVButton->setCheckable(true);
    toCSVButton->setIcon(QIcon(":/icons/csv.png"));

    QLabel *csvDesc = new QLabel("Start with CSV file");
    csvDesc->setStyleSheet("color: #8b949e; font-size: 9pt;");

    csvLayout->addWidget(toCSVButton);
    csvLayout->addWidget(csvDesc);

    // To Text
    auto *textGroup = new QGroupBox("To Text");
    auto *textLayout = new QVBoxLayout(textGroup);

    toTextButton = new QPushButton("Export to text file");
    toTextButton->setCheckable(true);
    toTextButton->setIcon(QIcon(":/icons/text.png"));

    QLabel *textDesc = new QLabel("Export to text file");
    textDesc->setStyleSheet("color: #8b949e; font-size: 9pt;");

    textLayout->addWidget(toTextButton);
    textLayout->addWidget(textDesc);

    mainLayout->addWidget(fileGroup);
    mainLayout->addWidget(csvGroup);
    mainLayout->addWidget(textGroup);

    mainLayout->addStretch();

    // Buttons
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel");
    exportButton = new QPushButton("Export");
    exportButton->setDefault(true);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(exportButton);

    mainLayout->addLayout(buttonLayout);

    connect(toFileButton, &QPushButton::clicked, this, &ExportDialog::onToFileClicked);
    connect(toCSVButton, &QPushButton::clicked, this, &ExportDialog::onToCSVClicked);
    connect(toTextButton, &QPushButton::clicked, this, &ExportDialog::onToTextClicked);
    connect(exportButton, &QPushButton::clicked, this, &ExportDialog::onExportClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ExportDialog::onToFileClicked()
{
    m_exportType = ToFile;
    toFileButton->setChecked(true);
    toCSVButton->setChecked(false);
    toTextButton->setChecked(false);
}

void ExportDialog::onToCSVClicked()
{
    m_exportType = ToCSV;
    toFileButton->setChecked(false);
    toCSVButton->setChecked(true);
    toTextButton->setChecked(false);
}

void ExportDialog::onToTextClicked()
{
    m_exportType = ToText;
    toFileButton->setChecked(false);
    toCSVButton->setChecked(false);
    toTextButton->setChecked(true);
}

void ExportDialog::onExportClicked()
{
    QString filter;
    if (m_exportType == ToFile) {
        filter = "Backup Files (*.json);;All Files (*)";
    } else if (m_exportType == ToCSV) {
        filter = "CSV Files (*.csv);;All Files (*)";
    } else {
        filter = "Text Files (*.txt);;All Files (*)";
    }

    m_filePath = QFileDialog::getSaveFileName(this, "Save File", "", filter);

    if (!m_filePath.isEmpty()) {
        accept();
    }
}
