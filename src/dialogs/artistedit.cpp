#include "include/dialogs/playlistedit.h"
#include "include/dialogs/artistedit.h"
#include "ui_artistedit.h"
#include <QMessageBox>

void ArtistEdit::updateMetaData()
{
    const Artist &artist = container->artist();
    ui->nameEdit->setText(artist.getName());
    ui->photoView->setImage(artist.getPhoto().toImage());
    ui->biographyEdit->setPlainText(artist.getBiography());
    setWindowTitle(QString("%1 - Artist Editor").arg(artist.getName()));
}

ArtistEdit::ArtistEdit(const Artist &value, QWidget *parent) : ArtistEdit(parent)
{
    setArtist(value);
}

ArtistEdit::ArtistEdit(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::ArtistEdit>();
    ui->setupUi(this);

    ui->removeButton->setVisible(false);

    container = std::make_unique<ArtistModel>();
    ui->playlistView->setModel(container.get());

    connect(ui->playlistView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
        [&](const QItemSelection &selected, const QItemSelection &deselected)
        {
            Q_UNUSED(selected)
            Q_UNUSED(deselected)
            auto indexes = ui->playlistView->selectionModel()->selectedIndexes();
            ui->removeButton->setVisible(!indexes.isEmpty());
        }
    );
}

ArtistEdit::~ArtistEdit() {}

Artist ArtistEdit::artist() const
{
    return container->artist();
}

void ArtistEdit::setArtist(const Artist &value)
{
    container->setArtist(value);
    updateMetaData();
}

void ArtistEdit::addPlaylist()
{
    PlaylistEdit editor;
    if(editor.exec() == QDialog::Accepted)
    {
        container->appendPlaylist(editor.playlist());
    }
}

void ArtistEdit::removePlaylist()
{
    auto indices = ui->playlistView->selectionModel()->selectedIndexes();

    QMessageBox message;
    message.setWindowTitle("Confirm");
    message.setIcon(QMessageBox::Warning);
    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message.setText(QString("Are you sure to remove %1 playlist(s) permanently?").arg(indices.size()));

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
            container->removePlaylist(index.row());
        }
    }
}

void ArtistEdit::editPlaylist(const QModelIndex &index)
{
    Playlist data = index.data(Qt::UserRole).value<Playlist>();
    PlaylistEdit editor(data);
    if(editor.exec() == QDialog::Accepted)
    {
        container->setData(index, QVariant::fromValue(editor.playlist()), Qt::UserRole);
        qDebug() << "Hello";
    }
}

void ArtistEdit::accept()
{
    try
    {
        if(ui->nameEdit->text().isEmpty())
        {
            throw std::runtime_error("You have not entered the name.");
        }
        else if(ui->biographyEdit->toPlainText().isEmpty())
        {
            throw std::runtime_error("You have not entered the biography.");
        }
        container->setName(ui->nameEdit->text());
        container->setBiography(ui->biographyEdit->toPlainText());
        container->setPhoto(QPixmap::fromImage(ui->photoView->getImage()));
    }
    catch(std::exception &e)
    {
        QMessageBox::critical(this, "Error", e.what());
        return;
    }
    QDialog::accept();
}
