#include "include/models/artistmodel.h"
#include "include/core/playlist.h"
#include "include/core/song.h"

int ArtistModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return Artist::loadIDsFromRecord(key).size();
}

QVariant ArtistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        Artist data = Artist::loadFromRecord(key);
        switch(role)
        {
            case Artist::NameRole:
            {
                return data.getName();
            }
            case Artist::BiographyRole:
            {
                return data.getBiography();
            }
            case Artist::PhotoRole:
            {
                return data.getPhoto();
            }
            case Artist::KeyRole:
            {
                return key;
            }
            default:
            {
                return QVariant();
            }
        }
    }
    IDs keys = Artist::loadIDsFromRecord(key);
    if(keys.isEmpty())
    {
        return QVariant();
    }

    const IDContainer &targetKey = keys.at(index.row());
    switch(role)
    {
        case Qt::DisplayRole:
        {
            Playlist data = Playlist::loadFromRecord(targetKey);
            return data.getName();
        }
        case Artist::KeyRole:
        {
            return targetKey;
        }
        case Qt::DecorationRole:
        {
            QImage cover;
            IDs songs = Playlist::loadIDsFromRecord(targetKey);
            if(!songs.isEmpty())
            {
                cover = Song::loadFromRecord(songs.constFirst()).getCover();
            }
            if(cover.isNull())
            {
                cover = QImage(":/images/playlist/playlist.png");
            }
            return cover.scaled(QSize(64, 64), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        default:
        {
            return {};
        }
    }
}

const IDContainer &ArtistModel::mainKey() const
{
    return key;
}

void ArtistModel::setMainKey(const IDContainer &value)
{
    beginResetModel();
    key = value;
    endResetModel();
}
