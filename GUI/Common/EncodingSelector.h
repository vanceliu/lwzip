#ifndef LWZIP_ENCODING_SELECTOR_H
#define LWZIP_ENCODING_SELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>

class EncodingManager;

class EncodingSelector : public QWidget
{
    Q_OBJECT

public:
    explicit EncodingSelector(QWidget *parent = nullptr);

signals:
    void encodingChanged(const QString &encoding);

private slots:
    void onEncodingSelected(int index);

private:
    QComboBox *m_combo = nullptr;
};

#endif // LWZIP_ENCODING_SELECTOR_H
