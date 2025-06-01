#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "include/dialogs/artistview.h"
#include "ui_artistview.h"

void ArtistView::updateModel()
{
    Artist artist = Artist::loadFromRecord(mainModel->mainKey());
    ui->nameLabel->setText(artist.getName());
    ui->photoView->setImage(artist.getPhoto());
    ui->biographyBrowser->setText(artist.getBiography());
    setWindowTitle(QString("%1 - Artist Page").arg(artist.getName()));
}

void ArtistView::playPlaylist(const QModelIndex &index)
{
    if(index.isValid())
    {
        const auto value = index.data(Artist::KeyRole).value<IDContainer>();
        PlaylistPlayer player(value);
        player.exec();
    }
}

void ArtistView::goToPlaylistPage()
{
    ui->pageContainer->setCurrentWidget(ui->playlistWidget);
}

void ArtistView::goToInfoPage()
{
    ui->pageContainer->setCurrentWidget(ui->infoWidget);
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
    return mainModel->mainKey();
}

void ArtistView::setMainKey(const IDContainer &value)
{
    mainModel->setMainKey(value);
    updateModel();
}
