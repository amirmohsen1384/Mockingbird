#include "include/dialogs/editor/adminplaylisteditor.h"
#include "include/dialogs/songedit.h"

AdminPlaylistEditor::AdminPlaylistEditor(QWidget *parent) : AbstractPlaylistEditor(parent) {}

void AdminPlaylistEditor::addSong()
{
    SongEdit editor(this);
    if(editor.exec() == QDialog::Accepted)
    {
        const Song &song = editor.getSong();
        for(const SongInfo &info : sourceModel->getStore())
        {
            if(info.second.getAddress() == song.getAddress())
            {
                sourceModel->setData(sourceModel->fromKey(info.first), QVariant::fromValue(song), Qt::UserRole);
                return;
            }
        }
        IDContainer id = ID::generateKey();
        sourceModel->insertSong(id, song);
    }
}

void AdminPlaylistEditor::editSong(const QModelIndex &index)
{
    SongEdit editor(index.data(Qt::UserRole).value<Song>(), this);
    if(editor.exec() == QDialog::Accepted)
    {
        sourceModel->setData(index, QVariant::fromValue(editor.getSong()), Qt::UserRole);
    }
}
