#include "ui_storedialog.h"
#include "include/models/genremodel.h"
#include "include/dialogs/storedialog.h"
#include "include/dialogs/playlistplayer.h"

void StoreDialog::updateArrangeCriteria()
{
    if(model.sourceModel() != nullptr)
    {
        ui->songView->selectionModel()->reset();
        model.setSortOrder(ui->arrangePanel->getSortOrder());
        model.setSortRole(ui->arrangePanel->getArrangeBase());
    }
}

void StoreDialog::updateFindCriteria()
{
    if(model.sourceModel() != nullptr)
    {
        QRegularExpression target;
        ui->songView->selectionModel()->reset();
        Qt::MatchFlag flag = ui->findPanel->getFlag();
        switch(flag)
        {
        case Qt::MatchStartsWith:
        {
            target.setPattern(QString("^%1").arg(ui->findPanel->getText()));
            break;
        }
        case Qt::MatchEndsWith:
        {
            target.setPattern(QString("%1$").arg(ui->findPanel->getText()));
            break;
        }
        case Qt::MatchContains:
        {
            target.setPattern(QString("%1").arg(ui->findPanel->getText()));
            break;
        }
        default:
        {
            return;
        }
        }
        model.setFilterRegularExpression(target);
        model.setFindRole(ui->findPanel->getFindRole());
        model.setFilterCaseSensitivity(ui->findPanel->getCaseSensitivityMode());
    }
}

void StoreDialog::updateFilter()
{
    if(model.sourceModel() != nullptr)
    {
        ui->songView->selectionModel()->reset();
        model.setFilteringGenre(ui->filterPanel->getGenre());
        model.setGenreFilteringEnabled(ui->filterPanel->isGenreFilteringEnabled());
        model.setMinimumYear(ui->filterPanel->isYearFilteringEnabled() ? ui->filterPanel->getMinimumYear() : _min_year);
        model.setMaximumYear(ui->filterPanel->isYearFilteringEnabled() ? ui->filterPanel->getMaximumYear() : QDate::currentDate().year());
    }
}

StoreDialog::StoreDialog(QWidget *parent) : QDialog(parent)
{
    ui = std::make_unique<Ui::StoreDialog>();
    ui->setupUi(this);

    ui->playButton->setVisible(false);

    ui->arrangePanel->setVisible(false);
    ui->filterPanel->setVisible(false);
    ui->findPanel->setVisible(false);

    delegate = std::make_unique<MainDelegate>();
    model.setSourceModel(&sourcModel);

    ui->songView->setModel(&model);
    ui->songView->setItemDelegate(delegate.get());

    connect(ui->songView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
        [&](const QItemSelection &selected, const QItemSelection &deselected)
        {
            Q_UNUSED(selected)
            Q_UNUSED(deselected)
            auto indices = ui->songView->selectionModel()->selectedIndexes();
            ui->playButton->setVisible(indices.size() == 1);
        }
    );

    connect(ui->filterPanel, &FilterEdit::filterChanged, this, &StoreDialog::updateFilter);
    connect(ui->findPanel, &FindWidget::findPropertyChanged, this, &StoreDialog::updateFindCriteria);
    connect(ui->arrangePanel, &ArrangeWidget::sortCriteriaChanged, this, &StoreDialog::updateArrangeCriteria);

    updateFilter();
    updateFindCriteria();
    updateArrangeCriteria();
}

StoreDialog::~StoreDialog() {}

void StoreDialog::playSong()
{
    const QModelIndex &index = ui->songView->currentIndex();
    if(index.isValid())
    {
        playSong(index);
    }
}

void StoreDialog::playSong(const QModelIndex &index)
{
    if(index.isValid())
    {
        PlaylistModel model(ID::generateKey());
        model.insertSong(index.data(Playlist::KeyRole).toLongLong(), index.data(Qt::UserRole).value<Song>());

        PlaylistPlayer player;
        player.setSourceModel(&model);
        player.exec();
    }
}

void StoreDialog::setFindPanelVisible(bool toggle)
{
    ui->findPanel->setVisible(toggle);

    ui->filterPanel->setVisible(false);
    ui->arrangePanel->setVisible(false);

    ui->filterButton->setChecked(false);
    ui->arrangeButton->setChecked(false);
}

void StoreDialog::setFilterPanelVisible(bool toggle)
{
    ui->filterPanel->setVisible(toggle);

    ui->arrangePanel->setVisible(false);
    ui->findPanel->setVisible(false);

    ui->arrangeButton->setChecked(false);
    ui->findButton->setChecked(false);
}

void StoreDialog::setArrangePanelVisible(bool toggle)
{
    ui->arrangePanel->setVisible(toggle);

    ui->filterPanel->setVisible(false);
    ui->findPanel->setVisible(false);

    ui->filterButton->setChecked(false);
    ui->findButton->setChecked(false);
}
