#include "include/dialogs/editor/adminplaylisteditor.h"
#include "include/dialogs/artistedit.h"
#include "ui_artistedit.h"
#include <QMessageBox>

void ArtistEdit::updateControl()
{
    ui->addButton->setVisible(model != nullptr);
    ui->removeButton->setVisible(model != nullptr);
}

void ArtistEdit::updateModel()
{
    ui->playlistView->setModel(model);
    updateControl();
    if(model)
    {
        const auto artist = model->headerData(0, Qt::Horizontal, Artist::DataRole).value<Artist>();
        connect(ui->playlistView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            [&](const QItemSelection &selected, const QItemSelection &deselected)
            {
                Q_UNUSED(selected)
                Q_UNUSED(deselected)
                auto indexes = ui->playlistView->selectionModel()->selectedIndexes();
                ui->removeButton->setVisible(!indexes.isEmpty());
            }
        );
        ui->nameEdit->setText(artist.getName());
        ui->photoView->setImage(artist.getPhoto());
        ui->biographyEdit->setPlainText(artist.getBiography());
        setWindowTitle(QString("%1 - Artist Editor").arg(artist.getName()));
    }
}

ArtistEdit::ArtistEdit(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::ArtistEdit>();
    ui->setupUi(this);

    ui->addButton->setVisible(false);
    ui->removeButton->setVisible(false);
    ui->removeCoverButton->setVisible(false);

    delegate = std::make_unique<MainDelegate>();
    delegate->setPrimary(Qt::green);
    delegate->setSecondary(Qt::darkBlue);
    ui->playlistView->setItemDelegate(delegate.get());

    connect(ui->photoView, &ImageView::imageChanged, this,
        [&](const QImage &value)
        {
            ui->removeCoverButton->setVisible(!value.isNull());
        }
    );
}

ArtistEdit::~ArtistEdit() {}

ArtistModel *ArtistEdit::sourceModel()
{
    return model;
}

ArtistModel *ArtistEdit::sourceModel() const
{
    return model;
}

void ArtistEdit::setSourceModel(ArtistModel *value)
{
    model = value;
    updateModel();
}

void ArtistEdit::accept()
{
    try
    {
        if(model)
        {
            const QString name = ui->nameEdit->text();
            if(!name.isEmpty())
            {
                model->setHeaderData(0, Qt::Horizontal, name, Artist::NameRole);
            }
            else
            {
                throw std::runtime_error("You have not entered the name.");
            }

            const QImage image = ui->photoView->getImage();
            model->setHeaderData(0, Qt::Horizontal, image, Artist::PhotoRole);

            const QString text = ui->biographyEdit->toPlainText();
            model->setHeaderData(0, Qt::Horizontal, text, Artist::BiographyRole);
        }
        else
        {
            throw std::runtime_error("You have not provided any sources to work with.");
        }
        QDialog::accept();
    }
    catch(std::exception &e)
    {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void ArtistEdit::removeCover()
{
    ui->photoView->setImage(QImage());
}

void ArtistEdit::addPlaylist()
{
    AdminPlaylistEditor editor;
    PlaylistModelContainer playlist = std::make_shared<PlaylistModel>(ID::generateKey());
    editor.setSourceModel(playlist.get());
    if(editor.exec() == QDialog::Accepted)
    {
        model->insertPlaylist(playlist);
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
            model->removeRow(index.row());
        }
    }
}

void ArtistEdit::editPlaylist(const QModelIndex &index)
{
    auto data = index.data(Artist::ModelRole).value<PlaylistModel*>();
    if(data != nullptr)
    {
        // Cloning from the original model
        std::shared_ptr<PlaylistModel> target = data->clone();

        // Performing on the clone
        AdminPlaylistEditor editor;
        editor.setSourceModel(target.get());
        if(editor.exec() == QDialog::Accepted)
        {
            model->setData(index, QVariant::fromValue(target), Artist::ModelRole);
        }
    }
}
