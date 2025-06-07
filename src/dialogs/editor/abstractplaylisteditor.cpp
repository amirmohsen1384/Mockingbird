#include "include/dialogs/editor/abstractplaylisteditor.h"
#include "ui_abstractplaylisteditor.h"
#include <QMessageBox>

void AbstractPlaylistEditor::updateModel()
{
    ui->songView->setModel(sourceModel);
    updateControl();
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
                ui->removeButton->setVisible(!indexes.isEmpty());
            }
        );
    }
}

void AbstractPlaylistEditor::updateControl()
{
    ui->nameEdit->setEnabled(sourceModel != nullptr);
    ui->songView->setEnabled(sourceModel != nullptr);
    ui->nameLabel->setEnabled(sourceModel != nullptr);
    ui->addButton->setVisible(sourceModel != nullptr);
}

AbstractPlaylistEditor::AbstractPlaylistEditor(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::AbstractPlaylistEditor>();
    ui->setupUi(this);

    ui->addButton->setVisible(false);
    ui->removeButton->setVisible(false);

    delegate = std::make_unique<MainDelegate>();
    delegate->setPrimary(Qt::darkRed);
    delegate->setSecondary(Qt::blue);
    ui->songView->setItemDelegate(delegate.get());
}

AbstractPlaylistEditor::~AbstractPlaylistEditor() {}

PlaylistModel *AbstractPlaylistEditor::getSourceModel()
{
    return sourceModel;
}

void AbstractPlaylistEditor::setSourceModel(PlaylistModel *model)
{
    sourceModel = model;
    updateModel();
}

void AbstractPlaylistEditor::accept()
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

void AbstractPlaylistEditor::removeSong()
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
