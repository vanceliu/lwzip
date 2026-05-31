#ifndef LWZIP_ARCHIVE_BRIDGE_H
#define LWZIP_ARCHIVE_BRIDGE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDateTime>

struct ArchiveItem {
    QString name;
    quint64 size;
    quint64 compressedSize;
    QDateTime modified;
    quint32 crc;
    bool isDirectory;
    bool isEncrypted;
    QString path;
};

struct CompressOptions {
    QString format;
    QString password;
    QString encryptionMethod;
    bool encryptFileNames;
    int compressionLevel;
    int threadCount;
    QString codePage;
};

struct ExtractOptions {
    QString password;
    QString outputDir;
    QString codePage;
    bool overwrite;
};

class ArchiveBridge : public QObject
{
    Q_OBJECT

public:
    explicit ArchiveBridge(QObject *parent = nullptr);
    ~ArchiveBridge();

    bool openArchive(const QString &path, const QString &codePage = "UTF-8");
    void closeArchive();
    bool isOpen() const;
    bool isEncrypted() const;

    QList<ArchiveItem> listItems(const QString &internalPath = QString()) const;
    QStringList supportedFormats() const;

    void extractAll(const ExtractOptions &options);
    void extractFiles(const QStringList &files, const ExtractOptions &options);
    void compress(const QStringList &inputFiles, const QString &outputPath, const CompressOptions &options);

    void setCodePage(const QString &codePage);
    QString codePage() const;

signals:
    void progressChanged(int percent, const QString &currentFile);
    void operationFinished(bool success, const QString &message);
    void passwordRequired();

private:
    class Private;
    Private *d;
};

#endif // LWZIP_ARCHIVE_BRIDGE_H
