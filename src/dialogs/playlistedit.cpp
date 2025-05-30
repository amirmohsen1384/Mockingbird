#include "include/dialogs/playlistplayer.h"
#include "include/dialogs/playlistedit.h"
#include "include/dialogs/songedit.h"
#include "ui_playlistedit.h"
#include <QMessageBox>

void PlaylistEdit::updateModel()
{
    const QString name = sourceModel->data(QModelIndex()).toString();
    setWindowTitle(QString("%1 - Playlist Editor").arg(name.isEmpty() ? "Untitled" : name));
    ui->nameEdit->setText(name);
    updateControl();
}

void PlaylistEdit::updateControl()
{
    ui->playButton->setVisible(sourceModel->rowCount() > 0);
}

void PlaylistEdit::updatePlaylistName()
{
    const QString value = ui->nameEdit->text();
    Playlist data = Playlist::loadFromRecord(sourceModel->getID());
    data.setName(value.isEmpty() ? "Untitled Playlist" : value);
    data.saveToRecord(sourceModel->getID());
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

    ui->playButton->setVisible(false);

    connect(ui->nameEdit, &QLineEdit::textEdited, this, &PlaylistEdit::updatePlaylistName);
    connect(sourceModel.get(), &PlaylistModel::rowsRemoved, this, &PlaylistEdit::updateControl);
    connect(sourceModel.get(), &PlaylistModel::rowsInserted, this, &PlaylistEdit::updateControl);
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
        const IDContainer id = ID::generateKey();
        const Song &song = editor.getSong();
        song.saveToRecord(id);
        sourceModel->insertID(id);
    }
    sourceModel->saveToRecord();
}

void PlaylistEdit::playSong()
{
    PlaylistPlayer player(sourceModel->getID());
    player.exec();
}

void PlaylistEdit::editSong(const QModelIndex &index)
{
    SongEdit editor(index.data(Qt::UserRole).value<Song>(), this);
    if(editor.exec() == QDialog::Accepted)
    {
        sourceModel->setData(index, QVariant::fromValue(editor.getSong()), Qt::UserRole);
    }
}

void PlaylistEdit::removeSong()
{
    auto indices = ui->songView->selectionModel()->selectedIndexes();

    QMessageBox message;
    message.setWindowTitle("Confirm");
    message.setIcon(QMessageBox::Warning);
    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message.setText(QString("Are you sure to remove %1 song(s) permanently?").arg(indices.size()));
    message.setInformativeText("The song(s) are also removed from the playlists of users. This cannot be reverted.");

    if(message.exec() == QMessageBox::Yes)
    {
        std::sort(indices.begin(), indices.end(),
            [&](const QModelIndex &one, const QModelIndex &two)
            {
                return one.row() >= two.row();
            }
        );

        PlaylistModel interface;
        auto playlists = MainFolder::getPlaylists().entryInfoList({"*"}, QDir::AllDirs | QDir::NoDotAndDotDot);
        for(const QModelIndex &index : indices)
        {
            const IDContainer key = index.data(Playlist::KeyRole).toLongLong();
            for(const QFileInfo &info : playlists)
            {
                IDContainer playlistId = info.baseName().toLongLong();
                interface.setID(playlistId);
                interface.removeID(key);
            }
            QFile::remove(MainFolder::getSongs().absoluteFilePath(QString("%1.sof").arg(key)));
            sourceModel->removeID(key);
        }
    }

    sourceModel->saveToRecord();
}
