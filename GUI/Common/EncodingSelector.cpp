#include "EncodingSelector.h"
#include "EncodingManager.h"

EncodingSelector::EncodingSelector(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *label = new QLabel(tr("Encoding:"), this);
    layout->addWidget(label);

    m_combo = new QComboBox(this);
    auto *mgr = EncodingManager::instance();
    if (mgr) {
        for (const auto &enc : mgr->availableEncodings())
            m_combo->addItem(enc.displayName, enc.name);

        int idx = m_combo->findData(mgr->currentEncoding());
        if (idx >= 0)
            m_combo->setCurrentIndex(idx);
    }
    layout->addWidget(m_combo);

    connect(m_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EncodingSelector::onEncodingSelected);
}

void EncodingSelector::onEncodingSelected(int index)
{
    QString encoding = m_combo->itemData(index).toString();
    auto *mgr = EncodingManager::instance();
    if (mgr)
        mgr->setEncoding(encoding);
    emit encodingChanged(encoding);
}
