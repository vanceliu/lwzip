#ifndef LWZIP_FILE_MANAGER_WINDOW_H
#define LWZIP_FILE_MANAGER_WINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QStandardItemModel>

class EncodingSelector;
class LanguageSelector;

class FileManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileManagerWindow(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onOpenArchive();
    void onExtract();
    void onExtractHere();
    void onCompress();
    void onNavigateUp();
    void onPathChanged();
    void onItemDoubleClicked(const QModelIndex &index);
    void onEncodingChanged(const QString &encoding);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void loadArchive(const QString &path);
    void navigateToFolder(const QString &internalPath);
    void refreshFileList();

    QSplitter *m_splitter = nullptr;
    QTreeView *m_folderTree = nullptr;
    QTableView *m_fileTable = nullptr;
    QLineEdit *m_pathEdit = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QLabel *m_statusLabel = nullptr;
    QLabel *m_fileCountLabel = nullptr;
    EncodingSelector *m_encodingSelector = nullptr;
    LanguageSelector *m_languageSelector = nullptr;

    QStandardItemModel *m_folderModel = nullptr;
    QStandardItemModel *m_fileModel = nullptr;

    QString m_currentArchivePath;
    QString m_currentInternalPath;
};

#endif // LWZIP_FILE_MANAGER_WINDOW_H
