#include "include/dialogs/editor/userplaylisteditor.h"
#include "include/dialogs/playlistplayer.h"
#include "ui_abstractplaylisteditor.h"
#include "storedialog.h"

UserPlaylistEditor::UserPlaylistEditor(QWidget *parent) : AbstractPlaylistEditor(parent)
{
    connect(this, &UserPlaylistEditor::headerDataEditableChanged, this, [&](bool value)
        {
            ui->nameEdit->setVisible(value);
            ui->nameLabel->setVisible(value);
        }
    );
}

bool UserPlaylistEditor::isHeaderDataEditable() const
{
    return state;
}

void UserPlaylistEditor::addSong()
{
    StoreDialog dialog(this);
}

void UserPlaylistEditor::performSong(const QModelIndex &index)
{
    PlaylistModel model;
    model.insertSong(index.data(Playlist::KeyRole).toLongLong(), index.data(Qt::UserRole).value<Song>());

    PlaylistPlayer player;
    player.setSourceModel(&model);

    player.exec();
}

void UserPlaylistEditor::setHeaderDataEditable(bool value)
{
    state = value;
    emit headerDataEditableChanged(state);
}
