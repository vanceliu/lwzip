#ifndef LWZIP_ENCODING_MANAGER_H
#define LWZIP_ENCODING_MANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QMap>

class EncodingManager : public QObject
{
    Q_OBJECT

public:
    struct EncodingInfo {
        QString name;
        QString displayName;
        QByteArray codecName;
    };

    explicit EncodingManager(QObject *parent = nullptr);

    QList<EncodingInfo> availableEncodings() const;
    QString currentEncoding() const;
    void setEncoding(const QString &encodingName);

    QString decode(const QByteArray &data) const;
    QByteArray encode(const QString &text) const;

    static EncodingManager *instance();

signals:
    void encodingChanged(const QString &newEncoding);

private:
    void initEncodings();

    QString m_currentEncoding;
    QList<EncodingInfo> m_encodings;
    static EncodingManager *s_instance;
};

#endif // LWZIP_ENCODING_MANAGER_H
