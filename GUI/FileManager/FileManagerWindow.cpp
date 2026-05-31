#include "FileManagerWindow.h"
#include "EncodingSelector.h"
#include "LanguageSelector.h"
#include "EncodingManager.h"
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
#include <QHeaderView>
#include <QPushButton>
#include <QAction>

FileManagerWindow::FileManagerWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();

    setWindowTitle(tr("LWZip File Manager"));
    setMinimumSize(900, 600);
    setAcceptDrops(true);
}

void FileManagerWindow::setupUI()
{
    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    // Navigation bar
    auto *navBar = new QHBoxLayout();
    auto *upBtn = new QPushButton(tr("Up"), this);
    m_pathEdit = new QLineEdit(this);
    m_pathEdit->setPlaceholderText(tr("Open an archive or drop file here..."));
    navBar->addWidget(upBtn);
    navBar->addWidget(m_pathEdit);
    mainLayout->addLayout(navBar);

    connect(upBtn, &QPushButton::clicked, this, &FileManagerWindow::onNavigateUp);
    connect(m_pathEdit, &QLineEdit::returnPressed, this, &FileManagerWindow::onPathChanged);

    // Encoding + Language bar
    auto *toolBar = new QHBoxLayout();
    m_encodingSelector = new EncodingSelector(this);
    m_languageSelector = new LanguageSelector(this);
    toolBar->addWidget(m_encodingSelector);
    toolBar->addStretch();
    toolBar->addWidget(m_languageSelector);
    mainLayout->addLayout(toolBar);

    connect(m_encodingSelector, &EncodingSelector::encodingChanged,
            this, &FileManagerWindow::onEncodingChanged);

    // Splitter: folder tree + file table
    m_splitter = new QSplitter(Qt::Horizontal, this);

    m_folderTree = new QTreeView(this);
    m_folderModel = new QStandardItemModel(this);
    m_folderModel->setHorizontalHeaderLabels({tr("Folders")});
    m_folderTree->setModel(m_folderModel);
    m_folderTree->setMaximumWidth(250);
    m_splitter->addWidget(m_folderTree);

    m_fileTable = new QTableView(this);
    m_fileModel = new QStandardItemModel(this);
    m_fileModel->setHorizontalHeaderLabels({
        tr("Name"), tr("Size"), tr("Compressed"), tr("Modified"), tr("CRC")
    });
    m_fileTable->setModel(m_fileModel);
    m_fileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileTable->horizontalHeader()->setStretchLastSection(true);
    m_fileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_splitter->addWidget(m_fileTable);

    m_splitter->setSizes({200, 700});
    mainLayout->addWidget(m_splitter);

    connect(m_fileTable, &QTableView::doubleClicked,
            this, &FileManagerWindow::onItemDoubleClicked);

    // Progress
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    setCentralWidget(central);
}

void FileManagerWindow::setupMenuBar()
{
    auto *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open Archive..."), this, &FileManagerWindow::onOpenArchive, QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("E&xit"), this, &QWidget::close, QKeySequence::Quit);

    auto *actionMenu = menuBar()->addMenu(tr("&Actions"));
    actionMenu->addAction(tr("&Extract..."), this, &FileManagerWindow::onExtract);
    actionMenu->addAction(tr("Extract &Here"), this, &FileManagerWindow::onExtractHere);
    actionMenu->addSeparator();
    actionMenu->addAction(tr("&Compress..."), this, &FileManagerWindow::onCompress);

    auto *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About LWZip"), [this]() {
        QMessageBox::about(this, tr("About LWZip File Manager"),
            tr("LWZip File Manager - Cross-Platform Archive Manager\n"
               "Based on 7-Zip by Igor Pavlov\n"
               "Version 0.1.0"));
    });
}

void FileManagerWindow::setupToolBar()
{
    auto *toolbar = addToolBar(tr("Main"));
    toolbar->addAction(tr("Open"), this, &FileManagerWindow::onOpenArchive);
    toolbar->addAction(tr("Extract"), this, &FileManagerWindow::onExtract);
    toolbar->addAction(tr("Compress"), this, &FileManagerWindow::onCompress);
    toolbar->addSeparator();
}

void FileManagerWindow::setupStatusBar()
{
    m_statusLabel = new QLabel(tr("Ready"), this);
    m_fileCountLabel = new QLabel(this);
    statusBar()->addWidget(m_statusLabel);
    statusBar()->addPermanentWidget(m_fileCountLabel);
}

void FileManagerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void FileManagerWindow::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (!urls.isEmpty()) {
        QString path = urls.first().toLocalFile();
        if (!path.isEmpty())
            loadArchive(path);
    }
}

void FileManagerWindow::onOpenArchive()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Archive"), QString(),
        tr("Archives (*.zip *.7z *.tar *.gz *.bz2 *.xz *.rar *.cab *.iso *.wim);;All Files (*)"));
    if (!path.isEmpty())
        loadArchive(path);
}

void FileManagerWindow::onExtract()
{
    if (m_currentArchivePath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No archive is open."));
        return;
    }

    QString outputDir = QFileDialog::getExistingDirectory(this, tr("Extract To"));
    if (outputDir.isEmpty())
        return;

    // TODO: integrate with 7zip core extraction
    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Extracting..."));
}

void FileManagerWindow::onExtractHere()
{
    if (m_currentArchivePath.isEmpty())
        return;

    QFileInfo fi(m_currentArchivePath);
    QString outputDir = fi.absolutePath();

    // TODO: integrate with 7zip core extraction
    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Extracting..."));
}

void FileManagerWindow::onCompress()
{
    // TODO: compress selected files
}

void FileManagerWindow::onNavigateUp()
{
    if (m_currentInternalPath.isEmpty())
        return;

    int lastSlash = m_currentInternalPath.lastIndexOf('/');
    if (lastSlash > 0)
        m_currentInternalPath = m_currentInternalPath.left(lastSlash);
    else
        m_currentInternalPath.clear();

    refreshFileList();
}

void FileManagerWindow::onPathChanged()
{
    QString path = m_pathEdit->text();
    if (QFileInfo::exists(path))
        loadArchive(path);
}

void FileManagerWindow::onItemDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    // TODO: navigate into folder or extract file
    QString itemName = m_fileModel->item(index.row(), 0)->text();
    navigateToFolder(m_currentInternalPath + "/" + itemName);
}

void FileManagerWindow::onEncodingChanged(const QString &encoding)
{
    Q_UNUSED(encoding)
    refreshFileList();
}

void FileManagerWindow::loadArchive(const QString &path)
{
    m_currentArchivePath = path;
    m_currentInternalPath.clear();
    m_pathEdit->setText(path);
    setWindowTitle(tr("LWZip File Manager - %1").arg(QFileInfo(path).fileName()));

    // TODO: open archive with 7zip core, populate tree and table
    m_statusLabel->setText(tr("Opened: %1").arg(path));
    refreshFileList();
}

void FileManagerWindow::navigateToFolder(const QString &internalPath)
{
    m_currentInternalPath = internalPath;
    refreshFileList();
}

void FileManagerWindow::refreshFileList()
{
    m_fileModel->removeRows(0, m_fileModel->rowCount());
    m_folderModel->removeRows(0, m_folderModel->rowCount());

    // TODO: read archive contents using 7zip core with current encoding
    // and populate m_fileModel and m_folderModel
}
