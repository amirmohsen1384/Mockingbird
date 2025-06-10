#include "include/dialogs/editor/userplaylisteditor.h"
#include "include/dialogs/playlistplayer.h"
#include "include/dialogs/artistview.h"
#include "include/panels/userpanel.h"
#include "ui_userpanel.h"
#include <QMessageBox>

void UserPanel::goToInfoEditor()
{
    ui->pageContainer->setCurrentWidget(ui->infoEditorPage);
}

void UserPanel::toggleShowPassword(bool enabled)
{
    if(!enabled)
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

void UserPanel::rejectEditor()
{
    goToProfilePage();
    initializeEditor();
}

void UserPanel::acceptEditor()
{
    User temp = User::loadFromRecord(mainModel.getMainKey());
    try
    {
        const QString firstName = ui->accountFirstNameEdit->text();
        const QString lastName = ui->accountLastNameEdit->text();
        const QString userName = ui->accountUserNameEdit->text();
        const QString password = ui->accountPasswordEdit->text();
        if(!temp.setFirstName(firstName))
        {
            throw std::runtime_error("The first name should only contain non-letter characters and not be empty.");
        }

        if(!temp.setLastName(lastName))
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

    temp.saveToRecord(mainModel.getMainKey());
    QMessageBox::information(this, "Successful", "Your account has been modified successfully.");

    initializeEditor();
    goToProfilePage();
}

void UserPanel::initializeEditor()
{
    auto data = User::loadFromRecord(mainModel.getMainKey());
    if(data.isNull())
    {
        return;
    }

    ui->accountFirstNameEdit->setText(data.getFirstName());
    ui->accountLastNameEdit->setText(data.getLastName());
    ui->accountPasswordEdit->setText(data.getPassword());
    ui->accountUserNameEdit->setText(data.getUserName());

    const auto name = data.getFullName();
    ui->accountNameLabel->setText(name);
    setWindowTitle(QString("%1 - User Panel").arg(name));
}

void UserPanel::checkAvailability()
{
    const bool condition = artistModel.rowCount() > 0;
    ui->notFoundLabel->setVisible(!condition);
    ui->artistList->setVisible(condition);
}

UserPanel::UserPanel(const IDContainer &key, QWidget *parent) : QMainWindow(parent)
{
    if(User::loadFromRecord(key).isNull())
    {
        throw std::runtime_error("User panel cannot be opened with an invalid key.");
    }
    mainModel.setMainKey(key);

    ui = std::make_unique<Ui::UserPanel>();
    ui->setupUi(this);

    ui->deleteButton->setVisible(false);
    ui->editButton->setVisible(false);

    delegate = std::make_unique<MainDelegate>();

    ui->artistList->setModel(&artistModel);
    ui->artistList->setItemDelegate(delegate.get());

    delegate->setPrimary(Qt::magenta);
    delegate->setSecondary(Qt::darkYellow);
    ui->playlistView->setModel(&mainModel);
    ui->playlistView->setItemDelegate(delegate.get());

    connect(ui->playlistView->selectionModel(), &QItemSelectionModel::currentChanged,
        [&](const auto &current, const auto &previous)
        {
            Q_UNUSED(previous)
            if(current.isValid())
            {
                ui->editButton->setVisible(true);
                auto id = current.data(User::KeyRole).toLongLong();
                ui->deleteButton->setVisible(!mainModel.isSpecialKey(id));
            }
        }
    );

    checkAvailability();
    initializeEditor();
}

UserPanel::~UserPanel() {}

void UserPanel::addPlaylist()
{
    auto model = std::make_shared<PlaylistModel>(ID::generateKey());

    UserPlaylistEditor editor(this);
    editor.setSourceModel(model.get());
    editor.setHeaderDataEditable(true);

    if(editor.exec() == QDialog::Accepted)
    {
        mainModel.insert(model);
    }
}

void UserPanel::editPlaylist()
{
    UserPlaylistEditor editor(this);
    const auto &index = ui->playlistView->currentIndex();
    auto model = index.data(User::ModelRole).value<PlaylistModel*>();
    editor.setHeaderDataEditable(!mainModel.isSpecialKey(model->getID()));

    auto clone = model->clone();
    editor.setSourceModel(clone.get());
    if(editor.exec() == QDialog::Accepted)
    {
        mainModel.setData(index, QVariant::fromValue(clone), User::ModelRole);
    }
}

void UserPanel::removePlaylist()
{
    QMessageBox message(this);
    message.setWindowTitle("Caution");
    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message.setText("Are you sure to permanently remove the playlist?");
    if(message.exec() == QMessageBox::Yes)
    {
        const auto &index = ui->playlistView->currentIndex();
        if(index.isValid())
        {
            mainModel.removeRow(index.row());
        }
    }
}

void UserPanel::viewArtist(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    ArtistModel model;
    ArtistView view(this);
    model.setMainKey(index.data(Artist::KeyRole).value<IDContainer>());
    view.setSourceModel(&model);
    view.exec();
}

void UserPanel::goToExplorePage()
{
    ui->pageContainer->setCurrentWidget(ui->explorePage);
}

void UserPanel::goToProfilePage()
{
    ui->pageContainer->setCurrentWidget(ui->profilePage);
}

void UserPanel::viewPlaylist(const QModelIndex &index)
{
    auto model = index.data(User::ModelRole).value<PlaylistModel*>();
    PlaylistPlayer player;
    player.setSourceModel(model);
    player.exec();
}
