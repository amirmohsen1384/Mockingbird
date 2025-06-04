#include "include/dialogs/artistedit.h"
#include "include/dialogs/artistview.h"
#include "include/panels/adminpanel.h"
#include "ui_adminpanel.h"
#include <QMessageBox>
#include <exception>

AdminPanel::AdminPanel(const IDContainer &key, QWidget *parent) : QMainWindow(parent)
{
    ui = std::make_unique<Ui::AdminPanel>();
    ui->setupUi(this);
    ui->editButton->setVisible(false);
    ui->removeButton->setVisible(false);
    ui->artistView->setModel(&mainModel);
    connect(ui->artistView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [&]
        (const QItemSelection &selected, const QItemSelection &deselected)
        {
            Q_UNUSED(selected)
            Q_UNUSED(deselected)
            auto indices = ui->artistView->selectionModel()->selectedIndexes();
            ui->editButton->setVisible(!indices.isEmpty());
            ui->removeButton->setVisible(!indices.isEmpty());
        }
    );
    connect(this, &AdminPanel::mainKeyChanged, this, &AdminPanel::updateMetaData);
    setMainKey(key);
}

AdminPanel::~AdminPanel() {}

IDContainer AdminPanel::getMainKey() const
{
    return mainKey;
}

void AdminPanel::updateMetaData()
{
    const Admin admin = Admin::loadFromRecord(mainKey);
    ui->adminNameLabel->setText(admin.getFullName());
    ui->firstNameEdit->setText(admin.getFirstName());
    ui->lastNameEdit->setText(admin.getLastName());
    ui->userNameEdit->setText(admin.getUserName());
    ui->passwordEdit->setText(admin.getPassword());
}

void AdminPanel::setMainKey(const IDContainer &value)
{
    mainKey = value;
    if(Admin::loadFromRecord(value).isNull() || !ID::isValid(value))
    {
        this->close();
        throw std::runtime_error("The given adminstrator is not available or not valid.");
    }
    emit mainKeyChanged(mainKey);
}

void AdminPanel::rejectChanging()
{
    ui->pageContainer->setCurrentWidget(ui->mainPage);
    updateMetaData();
}

void AdminPanel::acceptChanging()
{
    Admin temp;
    try
    {
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

    temp.saveToRecord(mainKey);
    updateMetaData();
    goToNormalMode();
}

void AdminPanel::togglePasswordShow(bool toggle)
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

void AdminPanel::goToInfoChangingMode()
{
    ui->pageContainer->setCurrentWidget(ui->informationPage);
}

void AdminPanel::goToNormalMode()
{
    ui->pageContainer->setCurrentWidget(ui->mainPage);
}

void AdminPanel::addArtist()
{
    ArtistModel model(ID::generateKey());
    ArtistEdit editor;
    editor.setSourceModel(&model);
    if(editor.exec() == QDialog::Accepted)
    {
        mainModel.insertArtist(model);
    }
}

void AdminPanel::viewArtist(const QModelIndex &index)
{
    if(index.isValid())
    {
        ArtistModel model(index.data(Artist::KeyRole).value<IDContainer>());
        ArtistView view;
        view.setSourceModel(&model);
        view.exec();
    }
}

void AdminPanel::removeArtist()
{
    const QModelIndex index = ui->artistView->currentIndex();
    if(index.isValid())
    {
        QMessageBox message;
        message.setWindowTitle("Caution");
        message.setIcon(QMessageBox::Warning);
        message.setText("Are you sure to remove the artist");
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setInformativeText("This will delete all of the associated playlists along with their songs in the whole system!");
        message.adjustSize();
        if(message.exec() == QMessageBox::Yes)
        {
            mainModel.removeArtist(index.data(Artist::KeyRole).value<IDContainer>());
        }
    }
}

void AdminPanel::editArtist()
{
    const QModelIndex &index = ui->artistView->currentIndex();
    if(index.isValid())
    {
        ArtistEdit editor;
        ArtistModel model(index.data(Artist::KeyRole).value<IDContainer>());
        editor.setSourceModel(&model);
        if(editor.exec() == QDialog::Accepted)
        {
            mainModel.modifyArtist(model);
        }
    }
}
