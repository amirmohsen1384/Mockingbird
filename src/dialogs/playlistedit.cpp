#include "include/dialogs/playlistplayer.h"
#include "include/dialogs/playlistedit.h"
#include "include/dialogs/songedit.h"
#include "ui_playlistedit.h"
#include <QMessageBox>

void PlaylistEdit::updateModel()
{
    updateControl();
    ui->songView->setModel(sourceModel);
    if(sourceModel)
    {
        const QString name = sourceModel->headerData().toString();
        setWindowTitle(QString("%1 - Playlist Editor").arg(name.isEmpty() ? "Untitled" : name));
        ui->nameEdit->setText(name);
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
}

void PlaylistEdit::updateControl()
{
    ui->nameEdit->setEnabled(sourceModel != nullptr);
    ui->songView->setEnabled(sourceModel != nullptr);
    ui->nameLabel->setEnabled(sourceModel != nullptr);
    ui->addButton->setVisible(sourceModel != nullptr);
    ui->removeButton->setVisible(sourceModel != nullptr);
}

PlaylistEdit::PlaylistEdit(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::PlaylistEdit>();
    ui->setupUi(this);
    ui->addButton->setVisible(false);
    ui->removeButton->setVisible(false);
}

PlaylistEdit::~PlaylistEdit() {}

PlaylistModel *PlaylistEdit::getSourceModel()
{
    return sourceModel;
}

const PlaylistModel *PlaylistEdit::getSourceModel() const
{
    return sourceModel;
}

void PlaylistEdit::setSourceModel(PlaylistModel *model)
{
    sourceModel = model;
    updateModel();
}

void PlaylistEdit::addSong()
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
    try
    {
        if(sourceModel)
        {
            const QString name = ui->nameEdit->text();
            if(!name.isEmpty())
            {
                sourceModel->setHeaderData(0, Qt::Horizontal, ui->nameEdit->text(), Qt::DisplayRole);
            }
            else
            {
                throw std::runtime_error("You have not entered the playlist's name.");
            }
            if(sourceModel->rowCount() <= 0)
            {
                throw std::runtime_error("You have not entered added any songs to the playlist.");
            }
        }
        else
        {
            throw std::runtime_error("You have not supplied any sources to work with.");
        }
        QDialog::accept();
    }
    catch(std::exception &e)
    {
        QMessageBox::critical(this, "Error", e.what());
    }
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
