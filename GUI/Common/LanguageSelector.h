#ifndef LWZIP_LANGUAGE_SELECTOR_H
#define LWZIP_LANGUAGE_SELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QTranslator>

class LanguageSelector : public QWidget
{
    Q_OBJECT

public:
    explicit LanguageSelector(QWidget *parent = nullptr);

signals:
    void languageChanged(const QString &locale);

private slots:
    void onLanguageSelected(int index);

private:
    QComboBox *m_combo = nullptr;
    QTranslator *m_translator = nullptr;
};

#endif // LWZIP_LANGUAGE_SELECTOR_H
