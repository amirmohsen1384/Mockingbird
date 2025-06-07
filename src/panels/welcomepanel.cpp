#include "include/panels/welcomepanel.h"
#include "include/core/playlist.h"
#include "include/core/general.h"
#include "include/core/admin.h"
#include "include/core/user.h"
#include "ui_welcomepanel.h"
#include <QMessageBox>
#include <QTimer>

WelcomePanel::WelcomePanel(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::WelcomePanel>();
    ui->setupUi(this);

    QTimer::singleShot(2000, this, [&]()
    {
        ui->pageContainer->setCurrentWidget(ui->signinPage);
    }
    );
}

WelcomePanel::~WelcomePanel()
{}

IDContainer WelcomePanel::getSignedkey() const
{
    return signedKey;
}

WelcomePanel::LoginMode WelcomePanel::getLoginMode() const
{
    return mode;
}

void WelcomePanel::goToSigninPage()
{
    ui->pageContainer->setCurrentWidget(ui->signinPage);
}

void WelcomePanel::toggleShowLoginPassword(bool value)
{
    if(!value)
    {
        ui->loginPasswordEdit->setEchoMode(QLineEdit::Password);
        ui->loginPasswordShowButton->setIcon(QIcon(":/images/windows/show-password.png"));
    }
    else
    {
        ui->loginPasswordEdit->setEchoMode(QLineEdit::Normal);
        ui->loginPasswordShowButton->setIcon(QIcon(":/images/windows/hide-password.png"));
    }
}

void WelcomePanel::toggleShowAccountPassword(bool value)
{
    if(!value)
    {
        ui->accountPasswordEdit->setEchoMode(QLineEdit::Password);
        ui->accountPasswordShowButton->setIcon(QIcon(":/images/windows/show-password.png"));
    }
    else
    {
        ui->accountPasswordEdit->setEchoMode(QLineEdit::Normal);
        ui->accountPasswordShowButton->setIcon(QIcon(":/images/windows/hide-password.png"));
    }
}

void WelcomePanel::rejectCreation()
{
    goToSigninPage();
    ui->accountFirstNameEdit->setText(QString());
    ui->accountUserNameEdit->setText(QString());
    ui->accountPasswordEdit->setText(QString());
    ui->accountLastNameEdit->setText(QString());
}

void WelcomePanel::acceptCreation()
{
    User temp;
    try
    {
        const QString lastName = ui->accountLastNameEdit->text();
        const QString userName = ui->accountUserNameEdit->text();
        const QString password = ui->accountPasswordEdit->text();
        const QString firstName = ui->accountFirstNameEdit->text();
        if(!temp.setFirstName(firstName))
        {
            throw std::runtime_error("The first name should only contain non-letter characters and not be empty.");
        }
        else if(!temp.setLastName(lastName))
        {
            throw std::runtime_error("The last name should only contain non-letter characters and not be empty.");
        }

        if(!temp.setUserName(userName))
        {
            throw std::runtime_error("The user name should have at least 6 characters.");
        }

        if(!temp.setPassword(password))
        {
            throw std::runtime_error("A strong password is at least 8 character, and has all kinds of characters.");
        }
    }
    catch(std::exception &e)
    {
        QMessageBox::critical(this, "Error", e.what());
        return;
    }

    User::createAccount(temp);

    QMessageBox::information(this, "Successful", "Your account has been created successfully.");
    rejectCreation();
}

void WelcomePanel::goToAccountPage()
{
    ui->pageContainer->setCurrentWidget(ui->accountPage);
}

void WelcomePanel::accept()
{
    const QString &name = ui->loginUserNameEdit->text();
    const QString &password = ui->loginPasswordEdit->text();
    auto users = MainFolder::getUsers().entryInfoList({"*"}, QDir::AllDirs | QDir::NoDotAndDotDot);
    auto admins = MainFolder::getAdmin().entryInfoList({"*.adf"}, QDir::AllEntries | QDir::NoDotAndDotDot);

    for(const auto &file : users)
    {
        IDContainer number = file.baseName().toLongLong();
        User user = User::loadFromRecord(number);
        if(user.getUserName() == name && user.getPassword() == password)
        {
            mode = LoginMode::LoginAsUser;
            signedKey = number;
            QDialog::accept();
            return;
        }
    }

    for(const auto &file : admins)
    {
        IDContainer number = file.baseName().toLongLong();
        Admin admin = Admin::loadFromRecord(number);
        if(admin.getUserName() == name && admin.getPassword() == password)
        {
            mode = LoginMode::LoginAsAdmin;
            signedKey = number;
            QDialog::accept();
            return;
        }
    }

    QMessageBox::critical(this, "Error", "The entered user name or password is incorrect.");
}
