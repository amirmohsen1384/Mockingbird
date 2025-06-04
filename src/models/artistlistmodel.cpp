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


// This namespace includes functions to permenantly remove an entity from the whole database.
namespace SensitiveRemoval
{
    // This function removes a song from the database
    bool removeSong(const IDContainer &key)
    {
        // Retrieves the filename of the song
        const QString &filename = MainFolder::getSongs().absoluteFilePath(QString::number(key));

        // Checks if the song itself exists.
        if(!QFile::exists(filename))
        {
            return false;
        }

        // Removes the song from other playlists.
        auto playlists = MainFolder::getPlaylists().entryInfoList({"*"}, QDir::NoDotAndDotDot | QDir::AllDirs);
        for(const auto &entry : playlists)
        {
            const IDContainer &value = entry.baseName().toLongLong();
            IDs keys = Playlist::loadIDsFromRecord(value);
            auto it = std::lower_bound(keys.cbegin(), keys.cend(), value);
            if(*it == value && it != keys.cend())
            {
                keys.remove(std::distance(keys.cbegin(), it));
            }
            Playlist::saveIDsToRecord(keys, value);
        }

        // Removes the file at the end.
        return QFile::remove(filename);
    }

    // This function removes a playlist from the database
    bool removePlaylist(const IDContainer &key)
    {
        // Retrieves the name of the playlist
        QDir directory(MainFolder::getPlaylists().absoluteFilePath(QString::number(key)));
        if(!directory.exists())
        {
            return false;
        }

        // Loads IDs from the record
        IDs keys = Playlist::loadIDsFromRecord(key);
        for(const IDContainer &value : keys)
        {
            removeSong(value);
        }

        // Removes the directory at the end
        return directory.removeRecursively();
    }

    // This function removes an artist from the database
    bool removeArtist(const IDContainer &key)
    {
        // Retrieves the name of the artist
        QDir directory(MainFolder::getArtists().absoluteFilePath(QString::number(key)));
        if(!directory.exists())
        {
            return false;
        }

        // Loads IDs from the record
        IDs keys = Artist::loadIDsFromRecord(key);
        for(const IDContainer &value : keys)
        {
            removePlaylist(value);
        }

        // Removes the directory at the end
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
        SensitiveRemoval::removeArtist(target.key);
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

void ArtistListModel::modifyArtist(const ArtistModel &model)
{
    const QModelIndex position = fromKey(model.mainKey());
    if(!position.isValid())
    {
        return;
    }

    ArtistModel initial(model.mainKey());
    const auto &final = model.getKeys();
    for(const auto &value : initial.getContainer())
    {
        const IDContainer initial = value->getID();
        auto it = std::lower_bound(final.cbegin(), final.cend(), initial);
        if(*it == initial && it != final.cend())
        {
            const auto &finalPlaylist = model.getContainer().at(std::distance(final.cbegin(), it));
            const auto &finalSongs = finalPlaylist->getStore();
            const auto &initialSongs = value->getStore();
            for(const auto &pair : initialSongs)
            {
                auto it = std::lower_bound(finalSongs.cbegin(), finalSongs.cend(), pair,
                    [&] (const SongInfo &one, const SongInfo &two)
                    {
                        return one.first < two.first;
                    }
                );
                if(it->first != pair.first || it == finalSongs.cend())
                {
                    SensitiveRemoval::removeSong(pair.first);
                }
            }
            for(const auto &target : finalSongs)
            {
                target.second.saveToRecord(target.first);
            }
            finalPlaylist->saveToRecord();
        }
        else
        {
            SensitiveRemoval::removePlaylist(initial);
        }
    }
    model.saveToRecord();

    DataContainer data;
    data.name = model.headerData(0, Qt::Horizontal, Artist::NameRole).toString();
    data.photo = model.headerData(0, Qt::Horizontal, Artist::PhotoRole).value<QImage>();
    data.key = model.headerData(0, Qt::Horizontal, Artist::KeyRole).value<IDContainer>();

    container.replace(position.row(), data);

    emit dataChanged(position, position, {Qt::DisplayRole, Qt::DecorationRole, Artist::KeyRole});
}
