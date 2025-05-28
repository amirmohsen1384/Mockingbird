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
            if(data.isNull())
            {
                return {};
            }
            return data.getName();
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
    const IDContainer key = songs.at(index.row());
    const Song target = Song::loadFromRecord(key);

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
        case Qt::DisplayRole:
        {
            Playlist p = Playlist::loadFromRecord(mainId);
            if(p.isNull())
            {
                return false;
            }
            p.setName(value.toString());
            p.saveToRecord(mainId);
            emit dataChanged(index, index, {Qt::DisplayRole});
            return true;
        }
        case Playlist::PlayingRole:
        {
            current = value.toInt();
            emit dataChanged(index, index, {Playlist::PlayingRole});
            return true;
        }
        default:
        {
            return false;
        }
        }
    }

    IDs keys = Playlist::loadIDsFromRecord(mainId);
    const IDContainer key = keys[index.row()];
    Song target = Song::loadFromRecord(key);

    switch(role)
    {
    case Qt::DisplayRole:
    {
        target.setName(value.toString());
        target.saveToRecord(key);
        emit dataChanged(index, index, {Qt::DisplayRole});
        return true;
    }
    case Qt::DecorationRole:
    {
        target.setCover(value.value<QImage>());
        target.saveToRecord(key);
        emit dataChanged(index, index, {Qt::DecorationRole});
        return true;
    }
    case Playlist::ArtistRole:
    {
        target.setArtist(value.toString());
        target.saveToRecord(key);
        emit dataChanged(index, index, {Playlist::ArtistRole});
        return true;
    }
    case Playlist::GenreRole:
    {
        target.setGenre(qvariant_cast<Song::Genre>(value));
        target.saveToRecord(key);
        emit dataChanged(index, index, {Playlist::GenreRole});
        return true;
    }
    case Playlist::YearRole:
    {
        target.setPublicationYear(value.toInt());
        target.saveToRecord(key);
        emit dataChanged(index, index, {Playlist::YearRole});
        return true;
    }
    case Qt::UserRole:
    {
        target = qvariant_cast<Song>(value);
        target.saveToRecord(key);
        emit dataChanged(index, index, roles);
        return true;
    }
    default:
    {
        return false;
    }
    }
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


bool PlaylistModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid())
    {
        return false;
    }

    IDs idList = Playlist::loadIDsFromRecord(mainId);
    if(idList.isEmpty())
    {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);

    idList.remove(row, count);
    Playlist::saveIDsToRecord(idList, mainId);

    endRemoveRows();
    return true;
}



bool PlaylistModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid())
    {
        return false;
    }

    IDs idList = Playlist::loadIDsFromRecord(mainId);

    beginInsertRows(parent, row, row + count - 1);

    idList.insert(row, count, 0);
    Playlist::saveIDsToRecord(idList, mainId);

    endRemoveRows();
    return true;
}

