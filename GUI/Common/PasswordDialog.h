#ifndef LWZIP_PASSWORD_DIALOG_H
#define LWZIP_PASSWORD_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        Encrypt,
        Decrypt
    };

    explicit PasswordDialog(Mode mode, QWidget *parent = nullptr);

    QString password() const;
    QString encryptionMethod() const;
    bool encryptFileNames() const;

private slots:
    void onShowPasswordToggled(bool checked);
    void onPasswordChanged(const QString &text);
    void validateAndAccept();

private:
    void setupEncryptUI();
    void setupDecryptUI();
    void updateStrengthIndicator(const QString &password);

    Mode m_mode;
    QLineEdit *m_passwordEdit = nullptr;
    QLineEdit *m_confirmEdit = nullptr;
    QLabel *m_strengthLabel = nullptr;
    QComboBox *m_methodCombo = nullptr;
    QCheckBox *m_encryptNamesCheck = nullptr;
    QCheckBox *m_showPasswordCheck = nullptr;
    QPushButton *m_okButton = nullptr;
};

#endif // LWZIP_PASSWORD_DIALOG_H
