#include "include/models/songmodel.h"
#include "include/core/playlist.h"

SongModel::SongModel(QObject *parent) : QAbstractListModel(parent)
{
    auto songFiles = MainFolder::getSongs().entryInfoList({"*.sof"}, QDir::AllEntries | QDir::NoDotAndDotDot);
    for(const auto &item : songFiles)
    {
        const IDContainer key = item.baseName().toLongLong();
        Song song = Song::loadFromRecord(key);
        if(!song.isNull())
        {
            SongInfo target;
            target.key = key;
            target.data = song;
            container.append(target);
        }
    }
}

int SongModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return container.size();
}


QVariant SongModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }
    const auto &songInfo = container.at(index.row());
    switch(role)
    {
    case Qt::DisplayRole:
    {
        return songInfo.data.getName();
    }
    case Qt::DecorationRole:
    {
        const auto &image = songInfo.data.getCover();
        if(!image.isNull())
        {
            return image.scaled(_cover_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
        else
        {
            return {};
        }

    }
    case Playlist::KeyRole:
    {
        return songInfo.key;
    }
    case Qt::UserRole:
    {
        return QVariant::fromValue(songInfo.data);
    }
    case Playlist::InfoRole:
    {
        return QVariant::fromValue(songInfo);
    }
    }
    return {};
}
