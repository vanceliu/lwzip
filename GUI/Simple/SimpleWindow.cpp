#include "SimpleWindow.h"
#include "EncodingSelector.h"
#include "LanguageSelector.h"
#include "PasswordDialog.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>

SimpleWindow::SimpleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();

    setWindowTitle(tr("LWZip"));
    setMinimumSize(600, 450);
    setAcceptDrops(true);
}

void SimpleWindow::setupUI()
{
    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    // Top toolbar area: encoding + language
    auto *topBar = new QHBoxLayout();
    m_encodingSelector = new EncodingSelector(this);
    m_languageSelector = new LanguageSelector(this);
    topBar->addWidget(m_encodingSelector);
    topBar->addStretch();
    topBar->addWidget(m_languageSelector);
    mainLayout->addLayout(topBar);

    // File list
    auto *fileGroup = new QGroupBox(tr("Files"), this);
    auto *fileLayout = new QVBoxLayout(fileGroup);
    m_fileList = new QListWidget(this);
    m_fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileLayout->addWidget(m_fileList);

    auto *fileButtons = new QHBoxLayout();
    auto *addBtn = new QPushButton(tr("Add Files..."), this);
    auto *removeBtn = new QPushButton(tr("Remove"), this);
    fileButtons->addWidget(addBtn);
    fileButtons->addWidget(removeBtn);
    fileButtons->addStretch();
    fileLayout->addLayout(fileButtons);
    mainLayout->addWidget(fileGroup);

    connect(addBtn, &QPushButton::clicked, this, &SimpleWindow::onAddFiles);
    connect(removeBtn, &QPushButton::clicked, this, &SimpleWindow::onRemoveSelected);

    // Format + action area
    auto *actionLayout = new QHBoxLayout();
    actionLayout->addWidget(new QLabel(tr("Format:"), this));
    m_formatCombo = new QComboBox(this);
    m_formatCombo->addItem("ZIP", "zip");
    m_formatCombo->addItem("7z", "7z");
    m_formatCombo->addItem("TAR", "tar");
    m_formatCombo->addItem("GZip", "gz");
    m_formatCombo->addItem("BZip2", "bz2");
    m_formatCombo->addItem("XZ", "xz");
    m_formatCombo->addItem("WIM", "wim");
    actionLayout->addWidget(m_formatCombo);
    actionLayout->addStretch();

    auto *compressBtn = new QPushButton(tr("Compress"), this);
    auto *extractBtn = new QPushButton(tr("Extract"), this);
    compressBtn->setMinimumWidth(100);
    extractBtn->setMinimumWidth(100);
    actionLayout->addWidget(compressBtn);
    actionLayout->addWidget(extractBtn);
    mainLayout->addLayout(actionLayout);

    connect(compressBtn, &QPushButton::clicked, this, &SimpleWindow::onCompress);
    connect(extractBtn, &QPushButton::clicked, this, &SimpleWindow::onExtract);
    connect(m_formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SimpleWindow::onFormatChanged);

    // Progress
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    setCentralWidget(central);
}

void SimpleWindow::setupMenuBar()
{
    auto *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open Archive..."), this, &SimpleWindow::onExtract, QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("E&xit"), this, &QWidget::close, QKeySequence::Quit);

    auto *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About LWZip"), [this]() {
        QMessageBox::about(this, tr("About LWZip"),
            tr("LWZip - Cross-Platform Archive Manager\n"
               "Based on 7-Zip by Igor Pavlov\n"
               "Version 0.1.0"));
    });
}

void SimpleWindow::setupToolBar()
{
    auto *toolbar = addToolBar(tr("Main"));
    toolbar->addAction(tr("Add"), this, &SimpleWindow::onAddFiles);
    toolbar->addAction(tr("Compress"), this, &SimpleWindow::onCompress);
    toolbar->addAction(tr("Extract"), this, &SimpleWindow::onExtract);
}

void SimpleWindow::setupStatusBar()
{
    m_statusLabel = new QLabel(tr("Ready"), this);
    statusBar()->addWidget(m_statusLabel);
}

void SimpleWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void SimpleWindow::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    for (const auto &url : urls) {
        QString path = url.toLocalFile();
        if (!path.isEmpty())
            m_fileList->addItem(path);
    }
}

void SimpleWindow::onAddFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add Files"));
    for (const auto &file : files)
        m_fileList->addItem(file);
}

void SimpleWindow::onRemoveSelected()
{
    auto items = m_fileList->selectedItems();
    for (auto *item : items)
        delete item;
}

void SimpleWindow::onCompress()
{
    if (m_fileList->count() == 0) {
        QMessageBox::warning(this, tr("Warning"), tr("No files to compress."));
        return;
    }

    QString format = m_formatCombo->currentData().toString();
    QString filter = QString("%1 files (*.%1)").arg(format);
    QString outputPath = QFileDialog::getSaveFileName(this, tr("Save Archive"), QString(), filter);
    if (outputPath.isEmpty())
        return;

    // Password dialog
    PasswordDialog dlg(PasswordDialog::Encrypt, this);
    if (dlg.exec() == QDialog::Accepted) {
        // TODO: integrate with 7zip core compression
        m_progressBar->setVisible(true);
        m_statusLabel->setText(tr("Compressing..."));
    }
}

void SimpleWindow::onExtract()
{
    QString archivePath = QFileDialog::getOpenFileName(this, tr("Open Archive"), QString(),
        tr("Archives (*.zip *.7z *.tar *.gz *.bz2 *.xz *.rar *.cab *.iso *.wim);;All Files (*)"));
    if (archivePath.isEmpty())
        return;

    QString outputDir = QFileDialog::getExistingDirectory(this, tr("Extract To"));
    if (outputDir.isEmpty())
        return;

    // TODO: detect if archive is encrypted, show password dialog
    // TODO: integrate with 7zip core extraction
    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Extracting..."));
}

void SimpleWindow::onFormatChanged(int index)
{
    Q_UNUSED(index)
}

void SimpleWindow::onProgressUpdate(int percent, const QString &currentFile)
{
    m_progressBar->setValue(percent);
    m_statusLabel->setText(currentFile);
}

void SimpleWindow::onOperationFinished(bool success, const QString &message)
{
    m_progressBar->setVisible(false);
    m_progressBar->setValue(0);
    m_statusLabel->setText(success ? tr("Done") : tr("Failed"));
    if (!success)
        QMessageBox::critical(this, tr("Error"), message);
    else
        QMessageBox::information(this, tr("Success"), message);
}
