#include "include/dialogs/playlistplayer.h"
#include "include/models/playlistmodel.h"
#include "include/dialogs/artistview.h"
#include "ui_artistview.h"

void ArtistView::updateModel()
{
    const auto condition = mainModel != nullptr;
    ui->playlistView->setModel(mainModel);
    updateControl();
    if(condition)
    {
        Artist artist = Artist::loadFromRecord(mainModel->mainKey());
        if(artist.getName().isEmpty())
        {
            ui->nameLabel->setText("Unnamed artist");
            setWindowTitle("Untitled - Artist Page");
        }
        else
        {
            ui->nameLabel->setText(artist.getName());
            setWindowTitle(QString("%1 - Artist Page").arg(artist.getName()));
        }
        ui->photoView->setImage(artist.getPhoto());
        if(artist.getBiography().isEmpty())
        {
            ui->biographyBrowser->setText("No biography has been provided.");
        }
        else
        {
            ui->biographyBrowser->setText(artist.getBiography());
        }
    }
    const auto visible = condition && mainModel->rowCount() > 0;
    ui->notFoundLabel->setVisible(!visible);
    ui->playlistView->setVisible(visible);
}

void ArtistView::updateControl()
{
    const auto condition = mainModel != nullptr;
    ui->nameLabel->setEnabled(condition);
    ui->photoView->setEnabled(condition);
    ui->pageContainer->setEnabled(condition);
    ui->biographyBrowser->setEnabled(condition);
    if(!condition)
    {
        ui->nameLabel->setText("Unable to know about the artist's name");
        ui->biographyBrowser->setPlainText("Unable to know about biography");
    }
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
    delegate->setPrimary(Qt::darkCyan);
    delegate->setSecondary(Qt::magenta);
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
