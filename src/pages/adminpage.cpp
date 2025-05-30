#include "include/pages/adminpage.h"
#include "ui_adminpage.h"
#include <QMessageBox>
#include <exception>

AdminPage::AdminPage(const Admin &admin, QWidget *parent) : AdminPage(parent)
{
    setAdminstrator(admin);
}

AdminPage::AdminPage(QWidget *parent) : QMainWindow(parent)
{
    ui = std::make_unique<Ui::AdminPage>();
    ui->setupUi(this);
}

AdminPage::~AdminPage() {}

Admin AdminPage::adminstrator() const
{
    return admin;
}

void AdminPage::updateMetaData()
{
    ui->adminNameLabel->setText(admin.getFullName());
    ui->firstNameEdit->setText(admin.getFirstName());
    ui->lastNameEdit->setText(admin.getLastName());
    ui->userNameEdit->setText(admin.getUserName());
    ui->passwordEdit->setText(admin.getPassword());
}

void AdminPage::setAdminstrator(const Admin &value)
{
    admin = value;
    updateMetaData();
    emit adminstratorChanged(value);
}

void AdminPage::rejectChanging()
{
    ui->pageContainer->setCurrentWidget(ui->mainPage);
    updateMetaData();
}

void AdminPage::acceptChanging()
{
    try
    {
        Admin temp;
        const QString lastName = ui->lastNameEdit->text();
        const QString userName = ui->userNameEdit->text();
        const QString password = ui->passwordEdit->text();
        const QString firstName = ui->firstNameEdit->text();
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

    // if(admin.saveToRecord())
    // {
    //     updateMetaData();
    // }
    ui->pageContainer->setCurrentWidget(ui->mainPage);
}

void AdminPage::togglePasswordShow(bool toggle)
{
    if(!toggle)
    {
        ui->passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        ui->showPasswordButton->setIcon(QIcon(":/images/windows/show-password.png"));
    }
    else
    {
        ui->passwordEdit->setEchoMode(QLineEdit::Normal);
        ui->showPasswordButton->setIcon(QIcon(":/images/windows/hide-password.png"));
    }
}

void AdminPage::goToInfoChangingMode()
{
    ui->pageContainer->setCurrentWidget(ui->informationPage);
}
