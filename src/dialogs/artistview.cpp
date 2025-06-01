#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "include/dialogs/artistview.h"
#include "ui_artistview.h"

void ArtistView::updateModel()
{
    ui->playlistView->setModel(mainModel);
    if(mainModel != nullptr)
    {
        Artist artist = Artist::loadFromRecord(mainModel->mainKey());
        ui->nameLabel->setText(artist.getName());
        ui->photoView->setImage(artist.getPhoto());
        ui->biographyBrowser->setText(artist.getBiography());
        setWindowTitle(QString("%1 - Artist Page").arg(artist.getName()));
    }
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

ArtistView::ArtistView(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::ArtistView>();
    ui->setupUi(this);
}

ArtistView::~ArtistView()
{}

ArtistModel *ArtistView::sourceModel() const
{
    return mainModel;
}

void ArtistView::setSourceModel(ArtistModel *value)
{
    mainModel = value;
    updateModel();
}
