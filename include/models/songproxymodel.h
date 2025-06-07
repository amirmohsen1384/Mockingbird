#ifndef SONGPROXYMODEL_H
#define SONGPROXYMODEL_H

#include "include/models/playlistmodel.h"
#include <QSortFilterProxyModel>

class SongProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum Sort
    {
        ByName = Qt::DisplayRole,
        ByArtist = Playlist::ArtistRole,
        ByReleasedYear = Playlist::YearRole,
        ByGenre = Playlist::GenreTextRole
    };

public:
    Q_DISABLE_COPY_MOVE(SongProxyModel)
    explicit SongProxyModel(QObject *parent = nullptr);

    int getMinimumYear() const;
    int getMaximumYear() const;
    Playlist::Role getFindRole() const;
    Qt::SortOrder getSortOrder() const;
    bool getGenreFilteringEnabled() const;
    Song::Genre getFilteringGenre() const;

    bool acceptsWith(const Song &data) const;

public slots:
    void arrange();

    void setMinimumYear(int value);
    void setMaximumYear(int value);
    void setFindRole(Playlist::Role role);
    void setSortOrder(Qt::SortOrder order);
    void setGenreFilteringEnabled(bool value);
    void setFilteringGenre(Song::Genre genre);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Qt::SortOrder order;
    int minYear = _min_year;
    bool filteringGenre = true;
    int maxYear = QDate::currentDate().year();
    Song::Genre genre = Song::Genre::Classical;
    Playlist::Role findRole = Playlist::NameRole;
};

#endif // SONGPROXYMODEL_H
