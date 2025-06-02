#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "include/models/songdelegate.h"
#include "ui_playlistplayer.h"

void PlaylistPlayer::updateModel()
{
    ui->playlistView->setModel(model);
    ui->player->setModel(model);

    if(model)
    {
        const QString name = model->headerData().toString();
        ui->titleLabel->setText(name.isEmpty() ? QString() : name);
        model->setHeaderData(0, Qt::Horizontal, 0, Playlist::PlayingRole);
        ui->playlistView->setItemDelegate(delegate.get());
        ui->player->setCurrentTrack(0);
    }
}

void PlaylistPlayer::updateCurrentTrack()
{
    if(model)
    {
        QModelIndex index = model->index(ui->player->getCurrentTrack(), 0);
        model->setHeaderData(0, Qt::Horizontal, ui->player->getCurrentTrack(), Playlist::PlayingRole);
        setWindowTitle(QString("%1 - Media Player").arg(index.data(Qt::DisplayRole).toString()));
    }
}

void PlaylistPlayer::playSong(const QModelIndex &index)
{
    ui->player->setCurrentTrack(index.row());
}

PlaylistPlayer::PlaylistPlayer(QWidget *parent) : QDialog(parent), ui(new Ui::PlaylistPlayer)
{
    ui->setupUi(this);
    delegate = std::make_unique<SongDelegate>();
    connect(ui->player, &Player::currentTrackChanged, this, &PlaylistPlayer::updateCurrentTrack);
}

PlaylistPlayer::~PlaylistPlayer() {}

void PlaylistPlayer::setSourceModel(PlaylistModel *value)
{
    model = value;
    updateModel();
}

PlaylistModel* PlaylistPlayer::getSourceModel() const
{
    return model;
}

PlaylistModel *PlaylistPlayer::getSourceModel()
{
    return model;
}
