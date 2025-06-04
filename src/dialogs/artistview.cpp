#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "include/dialogs/artistview.h"
#include "ui_artistview.h"

void ArtistView::updateModel()
{
    ui->playlistView->setModel(mainModel);
    updateControl();
    if(mainModel != nullptr)
    {
        Artist artist = Artist::loadFromRecord(mainModel->mainKey());
        ui->nameLabel->setText(artist.getName());
        ui->photoView->setImage(artist.getPhoto());
        ui->biographyBrowser->setText(artist.getBiography());
        setWindowTitle(QString("%1 - Artist Page").arg(artist.getName()));
    }
}

void ArtistView::updateControl()
{
    ui->nameLabel->setEnabled(mainModel != nullptr);
    ui->photoView->setEnabled(mainModel != nullptr);
    ui->pageContainer->setEnabled(mainModel != nullptr);
    ui->biographyBrowser->setEnabled(mainModel != nullptr);
    ui->nameLabel->setText(mainModel != nullptr ? "" : "Unable to know about the artist's name");
    ui->biographyBrowser->setPlainText(mainModel != nullptr ? "" : "Unable to know about biography");
}

void ArtistView::playPlaylist(const QModelIndex &index)
{
    if(index.isValid())
    {
        PlaylistModel *model = index.data(Artist::ModelRole).value<PlaylistModel*>();
        if(model != nullptr)
        {
            PlaylistPlayer player;
            player.setSourceModel(model);
            player.exec();
        }
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
    updateControl();

    delegate = std::make_unique<MainDelegate>();
    ui->playlistView->setItemDelegate(delegate.get());
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
