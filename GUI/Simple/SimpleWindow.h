#ifndef LWZIP_SIMPLE_WINDOW_H
#define LWZIP_SIMPLE_WINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>

class EncodingSelector;
class LanguageSelector;

class SimpleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleWindow(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onCompress();
    void onExtract();
    void onAddFiles();
    void onRemoveSelected();
    void onFormatChanged(int index);
    void onProgressUpdate(int percent, const QString &currentFile);
    void onOperationFinished(bool success, const QString &message);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();

    QListWidget *m_fileList = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QLabel *m_statusLabel = nullptr;
    QComboBox *m_formatCombo = nullptr;
    EncodingSelector *m_encodingSelector = nullptr;
    LanguageSelector *m_languageSelector = nullptr;
};

#endif // LWZIP_SIMPLE_WINDOW_H
