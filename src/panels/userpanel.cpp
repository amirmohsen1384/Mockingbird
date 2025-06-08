#include "include/dialogs/editor/userplaylisteditor.h"
#include "include/dialogs/playlistplayer.h"
#include "include/dialogs/artistview.h"
#include "include/panels/userpanel.h"
#include "ui_userpanel.h"
#include <QMessageBox>

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

    const auto& name = mainModel.headerData(0, Qt::Horizontal, Qt::DisplayRole).toString();
    ui->accountNameLabel->setText(name);
    setWindowTitle(QString("%1 - User Panel").arg(name));

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
    if(model->rowCount() <= 0)
    {
        QMessageBox::warning(this, "Error", "You have no song to be played in this playlist.");
        return;
    }
    PlaylistPlayer player;
    player.setSourceModel(model);
    player.exec();
}
