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
            container.append({song, key});
        }
    }
}

int SongModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
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
        return songInfo.data.getCover();
    }
    case Playlist::KeyRole:
    {
        return songInfo.key;
    }
    }
    return {};
}
