#include "include/models/playlistmodel.h"
#include "include/models/genremodel.h"
#include "include/core/song.h"
#include <QFont>

const QVector<int> PlaylistModel::roles  =
{
    Qt::UserRole,
    Qt::DisplayRole,
    Qt::DecorationRole,
    Playlist::ArtistRole,
    Playlist::GenreRole,
    Playlist::YearRole
};

PlaylistModel::PlaylistModel(const IDContainer &value, QObject *parent) : QAbstractListModel(parent)
{
    setID(value);
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return Playlist::loadIDsFromRecord(mainId).size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        switch(role)
        {
        case Qt::DisplayRole:
        {
            Playlist data = Playlist::loadFromRecord(mainId);
            return data.isNull() ? QVariant() : data.getName();
        }
        case Playlist::KeyRole:
        {
            return mainId;
        }
        case Playlist::PlayingRole:
        {
            return current;
        }
        default:
        {
            return {};
        }
        }
    }

    const IDs songs = Playlist::loadIDsFromRecord(mainId);
    if(songs.isEmpty())
    {
        return {};
    }

    const IDContainer key = songs.at(index.row());
    const Song target = Song::loadFromRecord(key);
    if(target.isNull())
    {
        return {};
    }

    switch(role)
    {
    case Qt::DisplayRole:
    {
        return target.getName();
    }
    case Qt::BackgroundRole:
    {
        return index.row() == current ? QColor(217, 255, 161) : QVariant();
    }
    case Qt::FontRole:
    {
        return index.row() == current ? QFont("Segoe UI", 10, QFont::Bold) : QVariant();
    }
    case Qt::DecorationRole:
    {
        if(target.getCover().isNull())
        {
            return QPixmap(":/images/playlist/song.png").scaled(64, 64);
        }
        else
        {
            return target.getCover().scaled(QSize(64, 64), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
    case Qt::UserRole:
    {
        return QVariant::fromValue<Song>(target);
    }
    case Playlist::PlayingRole:
    {
        return index.row() == current;
    }
    case Playlist::ArtistRole:
    {
        return target.getArtist();
    }
    case Playlist::GenreRole:
    {
        return QVariant::fromValue(target.getGenre());
    }
    case Playlist::GenreTextRole:
    {
        return GenreModel::toString(target.getGenre());
    }
    case Playlist::GenreIconRole:
    {
        return GenreModel::toPixmap(target.getGenre());
    }
    case Playlist::YearRole:
    {
        return target.getPublicationYear();
    }
    case Playlist::KeyRole:
    {
        return key;
    }
    default:
    {
        return {};
    }
    }
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        switch(role)
        {
        case Playlist::PlayingRole:
        {
            current = value.toInt();
            emit dataChanged(QModelIndex(), QModelIndex(), {Playlist::PlayingRole});
            return true;
        }
        }
    }
    return false;
}

IDContainer PlaylistModel::getID() const
{
    return mainId;
}

void PlaylistModel::setID(const IDContainer &value)
{
    beginResetModel();
    mainId = value;
    endResetModel();
}

int PlaylistModel::getCurrentTrack() const
{
    return current;
}

void PlaylistModel::setCurrentTrack(int index)
{
    setData(QModelIndex(), qBound(0, index, rowCount()), Playlist::PlayingRole);
}
