#include "include/dialogs/playlistplayer.h"
#include "include/dialogs/playlistedit.h"
#include "include/dialogs/songedit.h"
#include "ui_playlistedit.h"
#include <QMessageBox>

void PlaylistEdit::updateModel()
{
    const QString name = sourceModel->headerData().toString();
    setWindowTitle(QString("%1 - Playlist Editor").arg(name.isEmpty() ? "Untitled" : name));
    ui->nameEdit->setText(name);
}

void PlaylistEdit::deleteForever(const IDContainer &id)
{
    auto entries = MainFolder::getPlaylists().entryInfoList({"*"}, QDir::AllDirs | QDir::NoDotAndDotDot);
    for(auto info : entries)
    {
        // Obtaining the key of the playlist
        const IDContainer &key = info.baseName().toLongLong();
        if(id == key)
        {
            continue;
        }

        // Loading Song IDs from the playlist
        IDs keys = Playlist::loadIDsFromRecord(key);

        // Finding for the target key
        auto it = std::lower_bound(keys.cbegin(), keys.cend(), id);
        if(it != keys.cend() && *it == id)
        {
            int index = std::distance(keys.cbegin(), it);
            keys.remove(index);
        }

        // Saving the result
        Playlist::saveIDsToRecord(keys, key);
    }
}

PlaylistEdit::PlaylistEdit(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::PlaylistEdit>();
    ui->setupUi(this);

    sourceModel = std::make_unique<PlaylistModel>();
    ui->songView->setModel(sourceModel.get());

    connect(ui->songView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
        [&](const QItemSelection &selected, const QItemSelection &deselected)
        {
            Q_UNUSED(selected)
            Q_UNUSED(deselected)
            auto indexes = ui->songView->selectionModel()->selectedIndexes();
            ui->removeButton->setDisabled(indexes.isEmpty());
        }
    );
}

PlaylistEdit::PlaylistEdit(const IDContainer &value, QWidget *parent) : PlaylistEdit(parent)
{
    setMainId(value);
}


PlaylistEdit::~PlaylistEdit() {}

IDContainer PlaylistEdit::mainId() const
{
    return sourceModel->getID();
}

void PlaylistEdit::setMainId(const IDContainer &value)
{
    sourceModel->setID(value);
    updateModel();
}

void PlaylistEdit::addSong()
{
    SongEdit editor(this);
    if(editor.exec() == QDialog::Accepted)
    {
        const Song &song = editor.getSong();
        for(const SongInfo &info : store)
        {
            if(info.second.getAddress() == song.getAddress())
            {
                sourceModel->setData(sourceModel->fromKey(info.first), song, Qt::UserRole);
                return;
            }
        }
        IDContainer id = ID::generateKey();
        sourceModel->insertSong(id, song);
    }
}

void PlaylistEdit::editSong(const QModelIndex &index)
{
    SongEdit editor(index.data(Qt::UserRole).value<Song>(), this);
    if(editor.exec() == QDialog::Accepted)
    {
        sourceModel->setData(index, QVariant::fromValue(editor.getSong()), Qt::UserRole);
    }
}

void PlaylistEdit::accept()
{
    // Updating with the new keys.
    const IDs final = sourceModel->getKeys();
    const IDs initial = Playlist::loadIDsFromRecord(sourceModel->getID());

    // Deleting the old songs that are not present in the list.
    for(const IDContainer &key : initial)
    {
        auto it = std::lower_bound(final.cbegin(), final.cend(), key);
        if(it == final.cend() || *it != key)
        {
            deleteForever(key);
        }
    }

    // Updating the whole songs.
    const QList<SongInfo> &store = sourceModel->getStore();
    for(const SongInfo &info : store)
    {
        info.second.saveToRecord(info.first);
    }

    // Updating the meta data
    sourceModel->setHeaderData(0, Qt::Horizontal, ui->nameEdit->text(), Qt::DisplayRole);
    sourceModel->saveToRecord();
    QDialog::accept();
}

void PlaylistEdit::removeSong()
{
    auto indices = ui->songView->selectionModel()->selectedIndexes();

    QMessageBox message;
    message.setWindowTitle("Confirm");
    message.setIcon(QMessageBox::Information);
    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message.setText(QString("Are you sure to remove %1 song(s)?").arg(indices.size()));

    if(message.exec() == QMessageBox::Yes)
    {
        std::sort(indices.begin(), indices.end(),
            [&](const QModelIndex &one, const QModelIndex &two)
            {
                return one.row() >= two.row();
            }
        );

        for(const QModelIndex &index : indices)
        {
            sourceModel->removeSong(index.row());
        }
    }
}
