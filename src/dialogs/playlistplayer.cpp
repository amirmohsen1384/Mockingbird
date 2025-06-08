#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "ui_playlistplayer.h"

void PlaylistPlayer::updateModel()
{
    ui->playlistView->setModel(model);
    ui->player->setModel(model);

    if(model)
    {
        ui->player->setCurrentTrack(0);
        const QString name = model->headerData().toString();
        ui->titleLabel->setText(name.isEmpty() ? QString() : name);
        model->setHeaderData(0, Qt::Horizontal, 0, Playlist::PlayingRole);
        if(model->rowCount() == 1)
        {
            ui->playlistModeCheckBox->setVisible(false);
        }
    }
}

void PlaylistPlayer::updateCurrentTrack()
{
    if(model)
    {
        const int row = ui->player->getCurrentTrack();
        model->setHeaderData(0, Qt::Horizontal, row, Playlist::PlayingRole);

        QModelIndex index = model->index(row, 0);
        const int year = index.data(Playlist::YearRole).toInt();
        const QString name = index.data(Playlist::NameRole).toString();
        const QString artist = index.data(Playlist::ArtistRole).toString();
        const QString text = index.data(Playlist::GenreTextRole).toString();
        const QImage cover = index.data(Qt::UserRole).value<Song>().getCover();
        const QPixmap icon = index.data(Playlist::GenreIconRole).value<QPixmap>();        

        ui->nameLabel->setText(name);
        ui->coverView->setImage(cover);
        ui->genreTextLabel->setText(text);
        ui->genreIconLabel->setPixmap(icon);
        ui->yearLabel->setText(QString::number(year));
        ui->artistLabel->setText(QString("By %1").arg(artist));
        setWindowTitle(QString("%1 - Media Player").arg(name));
    }
}

void PlaylistPlayer::toggleSingleMode(bool enabled)
{
    if(enabled)
    {
        ui->pageContainer->setCurrentWidget(ui->playlistPage);
    }
    else
    {
        ui->pageContainer->setCurrentWidget(ui->singleModePage);
    }
}

void PlaylistPlayer::playSong(const QModelIndex &index)
{
    ui->player->setCurrentTrack(index.row());
}

PlaylistPlayer::PlaylistPlayer(QWidget *parent) : QDialog(parent), ui(new Ui::PlaylistPlayer)
{
    ui->setupUi(this);
    delegate = std::make_unique<PlaylistDelegate>();
    delegate->setSecondary(Qt::blue);
    delegate->setPrimary(Qt::darkGreen);
    ui->playlistView->setItemDelegate(delegate.get());
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
