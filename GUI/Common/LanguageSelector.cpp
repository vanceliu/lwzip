#include "LanguageSelector.h"
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QDir>

LanguageSelector::LanguageSelector(QWidget *parent)
    : QWidget(parent)
    , m_translator(new QTranslator(this))
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *label = new QLabel(tr("Language:"), this);
    layout->addWidget(label);

    m_combo = new QComboBox(this);
    m_combo->addItem("English", "en");
    m_combo->addItem("繁體中文", "zh_TW");
    m_combo->addItem("简体中文", "zh_CN");
    m_combo->addItem("日本語", "ja");
    layout->addWidget(m_combo);

    connect(m_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LanguageSelector::onLanguageSelected);
}

void LanguageSelector::onLanguageSelected(int index)
{
    QString locale = m_combo->itemData(index).toString();

    qApp->removeTranslator(m_translator);

    QString qmFile = QString("lwzip_%1.qm").arg(locale);
    QString qmPath = QApplication::applicationDirPath() + "/i18n/" + qmFile;

    if (m_translator->load(qmPath)) {
        qApp->installTranslator(m_translator);
    }

    emit languageChanged(locale);
}
