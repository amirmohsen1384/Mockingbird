#include "include/models/songproxymodel.h"

SongProxyModel::SongProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    connect(this, &SongProxyModel::sortRoleChanged, this, &SongProxyModel::arrange);
}

int SongProxyModel::getMinimumYear() const
{
    return minYear;
}

void SongProxyModel::setMinimumYear(int value)
{
    minYear = value;
    invalidateFilter();
}

int SongProxyModel::getMaximumYear() const
{
    return maxYear;
}

Playlist::Role SongProxyModel::getFindRole() const
{
    return findRole;
}

bool SongProxyModel::getGenreFilteringEnabled() const
{
    return filteringGenre;
}

Qt::SortOrder SongProxyModel::getSortOrder() const
{
    return order;
}

void SongProxyModel::setMaximumYear(int value)
{
    maxYear = value;
    invalidateFilter();
}

void SongProxyModel::setFindRole(Playlist::Role role)
{
    findRole = role;
    invalidateFilter();
}

void SongProxyModel::setGenreFilteringEnabled(bool value)
{
    filteringGenre = value;
    invalidateFilter();
}

void SongProxyModel::setSortOrder(Qt::SortOrder order)
{
    this->order = order;
    arrange();
}

Song::Genre SongProxyModel::getFilteringGenre() const
{
    return genre;
}

bool SongProxyModel::acceptsWith(const Song &data) const
{
    if(filteringGenre)
    {
        if(genre != data.getGenre())
        {
            return false;
        }
    }
    const auto year = data.getPublicationYear();
    return year >= minYear && year <= maxYear;
}

void SongProxyModel::arrange()
{
    this->sort(0, getSortOrder());
}

void SongProxyModel::setFilteringGenre(Song::Genre genre)
{
    this->genre = genre;
    invalidateFilter();
}

bool SongProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex &index = sourceModel()->index(source_row, 0, source_parent);
    const Song &song = index.data(Qt::UserRole).value<Song>();
    if(!acceptsWith(song))
    {
        return false;
    }
    else
    {
        switch(findRole)
        {
        case Playlist::NameRole:
        {
            return song.getName().contains(filterRegularExpression());
        }
        case Playlist::ArtistRole:
        {
            return song.getArtist().contains(filterRegularExpression());
        }
        default:
        {
            return false;
        }
        }
    }
    return false;
}
