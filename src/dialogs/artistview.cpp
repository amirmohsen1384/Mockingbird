#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "include/dialogs/artistview.h"
#include "ui_artistview.h"

void ArtistView::updateMetaData()
{
    Artist artist = Artist::loadFromRecord(mainModel->getMainKey());
    ui->nameLabel->setText(artist.getName());
    ui->photoView->setImage(artist.getPhoto());
    ui->biographyBrowser->setText(artist.getBiography());
    setWindowTitle(QString("%1 - Artist Page").arg(artist.getName()));
}

void ArtistView::playPlaylist(const QModelIndex &index)
{
    if(index.isValid())
    {
        const IDContainer &value = index.data(Artist::KeyRole).toLongLong();
        PlaylistPlayer player(value);
        player.exec();
    }
}

ArtistView::ArtistView(const IDContainer &key, QWidget *parent) : ArtistView(parent)
{
    setMainKey(key);
}

ArtistView::ArtistView(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::ArtistView>();
    ui->setupUi(this);

    mainModel = std::make_unique<ArtistModel>();
    ui->playlistView->setModel(mainModel.get());
}

ArtistView::~ArtistView()
{}

IDContainer ArtistView::mainKey() const
{
    return mainModel->getMainKey();
}

void ArtistView::setMainKey(const IDContainer &value)
{
    mainModel->setMainKey(value);
}
