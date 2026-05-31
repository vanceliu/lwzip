#include "EncodingManager.h"
#include <QStringConverter>

EncodingManager *EncodingManager::s_instance = nullptr;

EncodingManager::EncodingManager(QObject *parent)
    : QObject(parent)
    , m_currentEncoding("UTF-8")
{
    initEncodings();
    if (!s_instance)
        s_instance = this;
}

EncodingManager *EncodingManager::instance()
{
    return s_instance;
}

void EncodingManager::initEncodings()
{
    m_encodings = {
        {"UTF-8",        "UTF-8",                  "UTF-8"},
        {"Shift-JIS",    "Shift-JIS (Japanese)",   "Shift_JIS"},
        {"Big5",         "Big5 (Traditional Chinese)", "Big5"},
        {"GBK",          "GBK (Simplified Chinese)", "GBK"},
        {"EUC-KR",       "EUC-KR (Korean)",        "EUC-KR"},
        {"ISO-8859-1",   "ISO-8859-1 (Latin-1)",   "ISO-8859-1"},
        {"Windows-1252", "Windows-1252 (Western)", "Windows-1252"},
        {"EUC-JP",       "EUC-JP (Japanese)",      "EUC-JP"},
        {"ISO-2022-JP",  "ISO-2022-JP (Japanese)", "ISO-2022-JP"},
        {"GB2312",       "GB2312 (Simplified Chinese)", "GB2312"},
        {"KOI8-R",       "KOI8-R (Russian)",       "KOI8-R"},
        {"Windows-1251", "Windows-1251 (Cyrillic)","Windows-1251"},
    };
}

QList<EncodingManager::EncodingInfo> EncodingManager::availableEncodings() const
{
    return m_encodings;
}

QString EncodingManager::currentEncoding() const
{
    return m_currentEncoding;
}

void EncodingManager::setEncoding(const QString &encodingName)
{
    if (m_currentEncoding != encodingName) {
        m_currentEncoding = encodingName;
        emit encodingChanged(m_currentEncoding);
    }
}

QString EncodingManager::decode(const QByteArray &data) const
{
    auto decoder = QStringDecoder(m_currentEncoding.toUtf8().constData());
    if (decoder.isValid())
        return decoder(data);
    return QString::fromUtf8(data);
}

QByteArray EncodingManager::encode(const QString &text) const
{
    auto encoder = QStringEncoder(m_currentEncoding.toUtf8().constData());
    if (encoder.isValid())
        return encoder(text);
    return text.toUtf8();
}
