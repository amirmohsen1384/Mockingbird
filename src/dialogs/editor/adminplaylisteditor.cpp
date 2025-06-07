#include "include/dialogs/editor/adminplaylisteditor.h"
#include "ui_abstractplaylisteditor.h"
#include "include/dialogs/songedit.h"

AdminPlaylistEditor::AdminPlaylistEditor(QWidget *parent) : AbstractPlaylistEditor(parent)
{
    connect(ui->songView, &QListView::activated, this, &AdminPlaylistEditor::editSong);
}

void AdminPlaylistEditor::addSong()
{
    SongEdit editor(this);
    if(editor.exec() == QDialog::Accepted)
    {
        const Song &song = editor.getSong();
        for(const SongInfo &info : sourceModel->getStore())
        {
            if(info.data.getAddress() == song.getAddress())
            {
                sourceModel->setData(sourceModel->fromKey(info.key), QVariant::fromValue(song), Qt::UserRole);
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
