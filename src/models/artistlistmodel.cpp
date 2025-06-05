#include "include/models/artistlistmodel.h"

ArtistListModel::ArtistListModel(QObject *parent) : QAbstractListModel(parent)
{
    auto entries = MainFolder::getArtists().entryInfoList({"*"}, QDir::AllDirs | QDir::NoDotAndDotDot);
    for(const QFileInfo &file : entries)
    {
        DataContainer value;
        value.key = file.baseName().toLongLong();

        const Artist &data = Artist::loadFromRecord(value.key);
        value.photo = data.getPhoto();
        value.name = data.getName();

        auto it = std::lower_bound(container.cbegin(), container.cend(), value,
            [&](const DataContainer &one, const DataContainer &two)
            {
                return one.key < two.key;
            }
        );

        container.insert(std::distance(container.cbegin(), it), value);
    }
}

namespace Sensitive
{
    bool removeSong(const IDContainer &key)
    {
        const QString &filename = MainFolder::getSongs().absoluteFilePath(QString("%1.sof").arg(QString::number(key)));
        if(!QFile::exists(filename))
        {
            return false;
        }

        auto playlists = MainFolder::getPlaylists().entryInfoList({"*"}, QDir::NoDotAndDotDot | QDir::AllDirs);
        for(const auto &target : playlists)
        {
            IDContainer key = target.baseName().toLongLong();

            IDs playlistKeys = Playlist::loadIDsFromRecord(key);

            auto it = std::lower_bound(playlistKeys.cbegin(), playlistKeys.cend(), key);
            if(*it == key && it != playlistKeys.cend())
            {
                int index = std::distance(playlistKeys.cbegin(), it);
                playlistKeys.remove(index);
                Playlist::saveIDsToRecord(playlistKeys, key);
            }
        }

        return QFile::remove(filename);
    }

    bool removePlaylist(const IDContainer &key)
    {
        QDir directory(MainFolder::getPlaylists().absoluteFilePath(QString::number(key)));
        if(!directory.exists())
        {
            return false;
        }

        IDs keys = Playlist::loadIDsFromRecord(key);
        for(const IDContainer &value : keys)
        {
            removeSong(value);
        }

        return directory.removeRecursively();
    }

    bool removeArtist(const IDContainer &key)
    {
        QDir directory(MainFolder::getArtists().absoluteFilePath(QString::number(key)));
        if(!directory.exists())
        {
            return false;
        }

        IDs keys = Artist::loadIDsFromRecord(key);
        for(const IDContainer &value : keys)
        {
            removePlaylist(value);
        }

        return directory.removeRecursively();
    }
}

int ArtistListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : container.size();
}

QVariant ArtistListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    const DataContainer &value = container.at(index.row());
    switch(role)
    {
    case Qt::DisplayRole:
    {
        return value.name;
    }
    case Qt::DecorationRole:
    {
        QImage photo = value.photo;
        if(photo.isNull())
        {
            photo = QImage(":/images/windows/person.png");
        }
        return photo.scaled(_cover_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    case Artist::KeyRole:
    {
        return value.key;
    }
    case Artist::DataRole:
    {
        return QVariant::fromValue(value);
    }
    default:
    {
        return {};
    }
    }
}

void ArtistListModel::insertArtist(const ArtistModel &model)
{
    if(!ID::isValid(model.mainKey()))
    {
        return;
    }

    DataContainer data;
    data.key = model.mainKey();
    data.name = model.headerData(0, Qt::Horizontal, Artist::NameRole).toString();
    data.photo = model.headerData(0, Qt::Horizontal, Artist::PhotoRole).value<QImage>();

    auto it = std::lower_bound(container.cbegin(), container.cend(), data,
        [&](const DataContainer &one, const DataContainer &two)
        {
            return one.key < two.key;
        }
    );

    const int index = std::distance(container.cbegin(), it);

    beginInsertRows(QModelIndex(), index, index);

    const auto &store = model.getContainer();
    for(const auto &target : store)
    {
        const auto &store = target->getStore();
        for(const auto &song : store)
        {
            song.second.saveToRecord(song.first);
        }
        target->saveToRecord();
    }
    model.saveToRecord();
    container.insert(index, data);

    endInsertRows();
}

bool ArtistListModel::removeArtist(const IDContainer &key)
{
    return removeRow(fromKey(key).row());
}

bool ArtistListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = 0; i < count; ++i)
    {
        const auto &target = container.takeAt(row);
        Sensitive::removeArtist(target.key);
    }
    endRemoveRows();
    return true;
}

QModelIndex ArtistListModel::fromKey(const IDContainer &key) const
{
    DataContainer data;
    data.key = key;
    auto it = std::lower_bound(container.cbegin(), container.cend(), data,
        [&](const DataContainer &one, const DataContainer &two)
        {
            return one.key < two.key;
        }
    );
    if(it->key == key && it != container.cend())
    {
        return index(std::distance(container.cbegin(), it));
    }
    return QModelIndex();
}

void ArtistListModel::editArtist(const ArtistModel &newModel)
{
    if(!ID::isValid(newModel.mainKey()))
    {
        return;
    }
    const QModelIndex position = fromKey(newModel.mainKey());
    if(!position.isValid())
    {
        return;
    }

    ArtistModel oldModel;
    oldModel.setMainKey(newModel.mainKey());

    const IDs oldKeys = oldModel.getKeys();
    const IDs newKeys = newModel.getKeys();
    for(const IDContainer &oldKey : oldKeys)
    {
        auto threat = std::lower_bound(newKeys.cbegin(), newKeys.cend(), oldKey);
        if(*threat == oldKey && threat != newKeys.cend())
        {
            IDs newSongs = newModel.fromKey(oldKey).data(Artist::KeyListRole).value<IDs>();
            IDs oldSongs = Playlist::loadIDsFromRecord(oldKey);
            for(const IDContainer &oldSong : oldSongs)
            {
                auto target = std::lower_bound(newSongs.cbegin(), newSongs.cend(), oldSong);
                if(*target != oldSong || target == newSongs.cend())
                {
                    Sensitive::removeSong(oldSong);
                }
            }
        }
        else
        {
            Sensitive::removePlaylist(oldKey);
        }
    }

    for(auto i = 0; i < newModel.rowCount(); ++i)
    {
        const QModelIndex &index = newModel.index(i);
        auto model = index.data(Artist::ModelRole).value<PlaylistModel*>();
        for(const SongInfo &info : model->getStore())
        {
            info.second.saveToRecord(info.first);
        }
        model->saveToRecord();
    }
    newModel.saveToRecord();
}
