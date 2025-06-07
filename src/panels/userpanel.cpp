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
    ui->accountNameLabel->setText(QString("%1, How's it going?").arg(name));
    setWindowTitle(QString("%1 - User Panel").arg(name));

    connect(ui->playlistView->selectionModel(), &QItemSelectionModel::selectionChanged,
        [&](const auto &one, const auto &two)
        {
            Q_UNUSED(one)
            Q_UNUSED(two)
            auto indices = ui->playlistView->selectionModel()->selectedIndexes();
            if(!indices.isEmpty())
            {
                ui->editButton->setVisible(true);
                if(!indices.contains(mainModel.index(SAVED_INDEX)) && !indices.contains(mainModel.index(LIKED_INDEX)))
                {
                    ui->deleteButton->setVisible(true);
                }
            }
        }
    );
}

UserPanel::~UserPanel() {}

void UserPanel::addPlaylist()
{
    auto model = std::make_shared<PlaylistModel>();
    model->setID(ID::generateKey());

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

}

void UserPanel::removePlaylist()
{

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
