#include "PasswordDialog.h"
#include <QProgressBar>
#include <QMessageBox>

PasswordDialog::PasswordDialog(Mode mode, QWidget *parent)
    : QDialog(parent)
    , m_mode(mode)
{
    if (mode == Encrypt)
        setupEncryptUI();
    else
        setupDecryptUI();
}

void PasswordDialog::setupEncryptUI()
{
    setWindowTitle(tr("Set Password"));
    setMinimumWidth(380);

    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel(tr("Password:")));
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_passwordEdit);

    layout->addWidget(new QLabel(tr("Confirm Password:")));
    m_confirmEdit = new QLineEdit(this);
    m_confirmEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_confirmEdit);

    m_strengthLabel = new QLabel(this);
    layout->addWidget(m_strengthLabel);

    m_showPasswordCheck = new QCheckBox(tr("Show Password"), this);
    layout->addWidget(m_showPasswordCheck);

    layout->addWidget(new QLabel(tr("Encryption Method:")));
    m_methodCombo = new QComboBox(this);
    m_methodCombo->addItem("AES-256", "AES256");
    m_methodCombo->addItem("ZipCrypto", "ZipCrypto");
    layout->addWidget(m_methodCombo);

    m_encryptNamesCheck = new QCheckBox(tr("Encrypt file names"), this);
    layout->addWidget(m_encryptNamesCheck);

    auto *buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton(tr("OK"), this);
    m_okButton->setEnabled(false);
    auto *cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(m_passwordEdit, &QLineEdit::textChanged, this, &PasswordDialog::onPasswordChanged);
    connect(m_confirmEdit, &QLineEdit::textChanged, this, &PasswordDialog::onPasswordChanged);
    connect(m_showPasswordCheck, &QCheckBox::toggled, this, &PasswordDialog::onShowPasswordToggled);
    connect(m_okButton, &QPushButton::clicked, this, &PasswordDialog::validateAndAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void PasswordDialog::setupDecryptUI()
{
    setWindowTitle(tr("Enter Password"));
    setMinimumWidth(320);

    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel(tr("Password:")));
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_passwordEdit);

    m_showPasswordCheck = new QCheckBox(tr("Show Password"), this);
    layout->addWidget(m_showPasswordCheck);

    auto *buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton(tr("OK"), this);
    auto *cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(m_passwordEdit, &QLineEdit::textChanged, [this](const QString &text) {
        m_okButton->setEnabled(!text.isEmpty());
    });
    connect(m_showPasswordCheck, &QCheckBox::toggled, this, &PasswordDialog::onShowPasswordToggled);
    connect(m_okButton, &QPushButton::clicked, this, &PasswordDialog::validateAndAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void PasswordDialog::onShowPasswordToggled(bool checked)
{
    auto mode = checked ? QLineEdit::Normal : QLineEdit::Password;
    m_passwordEdit->setEchoMode(mode);
    if (m_confirmEdit)
        m_confirmEdit->setEchoMode(mode);
}

void PasswordDialog::onPasswordChanged(const QString &text)
{
    Q_UNUSED(text)
    bool valid = !m_passwordEdit->text().isEmpty();
    if (m_mode == Encrypt) {
        valid = valid && (m_passwordEdit->text() == m_confirmEdit->text());
        updateStrengthIndicator(m_passwordEdit->text());
    }
    m_okButton->setEnabled(valid);
}

void PasswordDialog::validateAndAccept()
{
    if (m_mode == Encrypt && m_passwordEdit->text() != m_confirmEdit->text()) {
        QMessageBox::warning(this, tr("Error"), tr("Passwords do not match."));
        return;
    }
    accept();
}

void PasswordDialog::updateStrengthIndicator(const QString &password)
{
    int strength = 0;
    if (password.length() >= 8) strength++;
    if (password.length() >= 12) strength++;
    if (password.contains(QRegularExpression("[A-Z]"))) strength++;
    if (password.contains(QRegularExpression("[0-9]"))) strength++;
    if (password.contains(QRegularExpression("[^A-Za-z0-9]"))) strength++;

    QString text;
    QString color;
    if (strength <= 1) { text = tr("Weak"); color = "red"; }
    else if (strength <= 3) { text = tr("Medium"); color = "orange"; }
    else { text = tr("Strong"); color = "green"; }

    m_strengthLabel->setText(QString("<span style='color:%1'>%2</span>").arg(color, text));
}

QString PasswordDialog::password() const
{
    return m_passwordEdit->text();
}

QString PasswordDialog::encryptionMethod() const
{
    if (m_methodCombo)
        return m_methodCombo->currentData().toString();
    return "AES256";
}

bool PasswordDialog::encryptFileNames() const
{
    if (m_encryptNamesCheck)
        return m_encryptNamesCheck->isChecked();
    return false;
}
