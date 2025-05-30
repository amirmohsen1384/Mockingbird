#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "include/models/songdelegate.h"
#include "ui_playlistplayer.h"

void PlaylistPlayer::updateModel()
{
    ui->player->setCurrentTrack(0);
    model->setData(QModelIndex(), Playlist::PlayingRole, 0);
    const QString name = model->data(QModelIndex()).toString();
    ui->titleLabel->setText(name.isEmpty() ? QString() : name);
}

void PlaylistPlayer::updateCurrentTrack()
{
    QModelIndex index = model->index(ui->player->getCurrentTrack(), 0);
    model->setData(QModelIndex(), Playlist::PlayingRole, ui->player->getCurrentTrack());
    setWindowTitle(QString("%1 - Media Player").arg(index.data(Qt::DisplayRole).toString()));
}

void PlaylistPlayer::playSong(const QModelIndex &index)
{
    ui->player->setCurrentTrack(index.row());
}

PlaylistPlayer::PlaylistPlayer(const IDContainer &id, QWidget *parent) : PlaylistPlayer(parent)
{
    setID(id);
}

PlaylistPlayer::PlaylistPlayer(QWidget *parent) : QDialog(parent), ui(new Ui::PlaylistPlayer)
{
    ui->setupUi(this);
    model = std::make_unique<PlaylistModel>();
    delegate = std::make_unique<SongDelegate>();
    ui->playlistView->setItemDelegate(delegate.get());
    connect(ui->player, &Player::currentTrackChanged, this, &PlaylistPlayer::updateCurrentTrack);
    ui->playlistView->setModel(model.get());
    ui->player->setModel(model.get());
    ui->player->stop();
}

void PlaylistPlayer::setID(const IDContainer &id)
{
    model->setID(id);
    updateModel();
}

IDContainer PlaylistPlayer::getID() const
{
    return model->getID();
}

PlaylistPlayer::~PlaylistPlayer() {}
