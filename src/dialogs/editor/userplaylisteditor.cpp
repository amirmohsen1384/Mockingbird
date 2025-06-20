#include "include/dialogs/editor/userplaylisteditor.h"
#include "include/dialogs/playlistplayer.h"
#include "include/dialogs/storedialog.h"
#include "ui_abstractplaylisteditor.h"

UserPlaylistEditor::UserPlaylistEditor(QWidget *parent) : AbstractPlaylistEditor(parent)
{
    connect(this, &UserPlaylistEditor::headerDataEditableChanged, this, [&](bool value)
        {
            ui->nameEdit->setVisible(value);
            ui->nameLabel->setVisible(value);
        }
    );
    connect(ui->songView, &QListView::activated, this, &UserPlaylistEditor::playSong);
}

bool UserPlaylistEditor::isHeaderDataEditable() const
{
    return state;
}

void UserPlaylistEditor::addSong()
{
    StoreDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)
    {
        const SongInfo &info = dialog.getSelectedSong();
        sourceModel->insertSong(info);
        AbstractPlaylistEditor::addSong();
    }
}

void UserPlaylistEditor::playSong(const QModelIndex &index)
{
    PlaylistModel model;
    model.insertSong(index.data(Playlist::InfoRole).value<SongInfo>());

    PlaylistPlayer player;
    player.setSourceModel(&model);

    player.exec();
}

void UserPlaylistEditor::setHeaderDataEditable(bool value)
{
    state = value;
    emit headerDataEditableChanged(state);
}
