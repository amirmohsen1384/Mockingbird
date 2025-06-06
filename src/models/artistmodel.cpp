#include "include/models/artistmodel.h"

void ArtistModel::updateModel()
{
    beginResetModel();

    // Resets the store
    container.clear();

    // Checks if the given key is valid.
    if(ID::isValid(key))
    {
        // Loads the meta data into the model.
        metaData = Artist::loadFromRecord(key);

        // Loads the playlists and songs into model
        IDs keys = Artist::loadIDsFromRecord(key);
        for(const IDContainer &key : keys)
        {
            PlaylistModelContainer model;
            model = std::make_shared<PlaylistModel>(key);
            if(model->rowCount() > 0)
            {
                container.append(model);
            }
        }
    }

    endResetModel();
}

ArtistModel::ArtistModel(const IDContainer &key, QObject *parent) : QAbstractListModel(parent)
{
    setMainKey(key);
}

QVariant ArtistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    if(section != 0)
    {
        return {};
    }
    switch(role)
    {
    case Artist::NameRole:
    {
        return metaData.getName();
    }
    case Artist::BiographyRole:
    {
        return metaData.getBiography();
    }
    case Artist::PhotoRole:
    {
        return metaData.getPhoto();
    }
    case Artist::KeyRole:
    {
        return key;
    }
    case Artist::DataRole:
    {
        return QVariant::fromValue(metaData);
    }
    default:
    {
        return {};
    }
    }
}

bool ArtistModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(section != 0)
    {
        return false;
    }
    switch(role)
    {
    case Artist::NameRole:
    {
        metaData.setName(value.toString());
        break;
    }
    case Artist::BiographyRole:
    {
        metaData.setBiography(value.toString());
        break;
    }
    case Artist::PhotoRole:
    {
        metaData.setPhoto(value.value<QImage>());
        break;
    }
    default:
    {
        return false;
    }
    }
    emit headerDataChanged(orientation, section, section);
    return true;
}

int ArtistModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() || !ID::isValid(key) ? 0 : container.size();
}

QVariant ArtistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    auto value = container[index.row()];

    switch(role)
    {
    case Artist::KeyRole:
    {
        return value->getID();
    }
    case Qt::DisplayRole:
    {
        return value->headerData(Playlist::NameRole);
    }
    case Artist::KeyListRole:
    {
        return QVariant::fromValue(value->getKeys());
    }
    case Artist::ModelRole:
    {
        return QVariant::fromValue<PlaylistModel*>(value.get());
    }
    case Qt::DecorationRole:
    {
        return value->data(value->index(0), Playlist::CoverRole);
    }
    default:
    {
        return {};
    }
    }
}

bool ArtistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return false;
    }
    if(role != Artist::ModelRole)
    {
        container[index.row()]->setHeaderData(0, Qt::Horizontal, value, role);
        emit dataChanged(index, index, {role, Qt::DecorationRole});
    }
    else
    {
        container.replace(index.row(), value.value<PlaylistModelContainer>());
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::DecorationRole, Artist::KeyRole, Artist::ModelRole});
    }
    return true;
}

bool ArtistModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    container.remove(row, count);
    endRemoveRows();
    return true;
}

void ArtistModel::insertPlaylist(PlaylistModelContainer value)
{
    auto it = std::lower_bound(container.cbegin(), container.cend(), value,
    [&](const PlaylistModelContainer &one, const PlaylistModelContainer &two)
    {
        return one->getID() < two->getID();
    });
    int index = std::distance(container.cbegin(), it);
    beginInsertRows(QModelIndex(), index, index);
    container.insert(index, value);
    endInsertRows();
}

void ArtistModel::saveToRecord() const
{
    if(ID::isValid(key))
    {
        metaData.saveToRecord(key);
        Artist::saveIDsToRecord(getKeys(), key);
    }
}

IDs ArtistModel::getKeys() const
{
    IDs keys;
    for(auto iterator = container.cbegin(); iterator != container.cend(); ++iterator)
    {
        keys.append((*iterator)->getID());
    }
    return keys;
}

QModelIndex ArtistModel::fromKey(const IDContainer &value) const
{
    IDs keys = getKeys();
    auto it = std::lower_bound(keys.cbegin(), keys.cend(), value);
    if(*it == value && it != keys.cend())
    {
        return index(std::distance(keys.cbegin(), it));
    }
    return QModelIndex();
}

const QList<PlaylistModelContainer> &ArtistModel::getContainer() const
{
    return container;
}

IDContainer ArtistModel::mainKey() const
{
    return key;
}

void ArtistModel::setMainKey(const IDContainer &value)
{
    key = value;
    updateModel();
}
