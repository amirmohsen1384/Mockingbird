#include "include/models/usermodel.h"

void UserModel::updateModel()
{
    container.clear();

    if(!ID::isValid(mainKey))
    {
        return;
    }

    metaData = User::loadFromRecord(mainKey);
    if(metaData.isNull())
    {
        return;
    }

    IDs keys = User::loadIDsFromRecord(mainKey);
    for(const IDContainer &key : keys)
    {
        auto model = std::make_shared<PlaylistModel>(key);
        container.append(model);
    }

    likedPlaylist = std::make_shared<PlaylistModel>(metaData.getLikedPlaylist());
    savedPlaylist = std::make_shared<PlaylistModel>(metaData.getSavedPlaylist());
}

UserModel::UserModel(const IDContainer &key, QObject *parent) : QAbstractListModel(parent)
{
    setMainKey(key);
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    if(section != 0)
    {
        return {};
    }
    switch(role)
    {
    case User::NameRole:
    {
        return metaData.getFullName();
    }
    case User::FirstNameRole:
    {
        return metaData.getFirstName();
    }
    case User::LastNameRole:
    {
        return metaData.getLastName();
    }
    case User::UserNameRole:
    {
        return metaData.getUserName();
    }
    case User::PasswordRole:
    {
        return metaData.getPassword();
    }
    case User::LikedRole:
    {
        return metaData.getLikedPlaylist();
    }
    case User::SavedRole:
    {
        return metaData.getSavedPlaylist();
    }
    default:
    {
        return {};
    }
    }
}

void UserModel::insert(std::shared_ptr<PlaylistModel> value)
{
    const IDContainer target = value->getID();
    if(!ID::isValid(target))
    {
        qDebug() << "The given ID is not valid.";
        return;
    }

    IDs keys = getKeys();
    auto it = std::lower_bound(keys.cbegin(), keys.cend(), target);
    if(it != keys.cend())
    {
        if(*it == target)
        {
            qDebug() << "The ID is already in the list.";
            return;
        }
    }

    int index = std::distance(keys.cbegin(), it);
    beginInsertRows(QModelIndex(), index, index);

    value->saveToRecord();
    container.insert(index, value);
    keys.insert(index, value->getID());
    User::saveIDsToRecord(keys, mainKey);

    endInsertRows();
}

bool UserModel::remove(const IDContainer &key)
{
    if(metaData.getSpecialKeys().contains(key))
    {
        qDebug() << "Cannot delete special playlists.";
        return false;
    }

    auto keys = getKeys();
    auto it = std::lower_bound(keys.cbegin(), keys.cend(), key);
    if(it != keys.cend() && *it == key)
    {
        int index = std::distance(keys.cbegin(), it);
        return removeRow(index);
    }
    else
    {
        qDebug() << "The ID was not found in the list.";
        return false;
    }
}

void UserModel::setMainKey(const IDContainer &key)
{
    beginResetModel();
    mainKey = key;
    updateModel();
    endResetModel();;
}

IDContainer UserModel::getMainKey() const
{
    return mainKey;
}

IDs UserModel::getKeys() const
{
    IDs result;
    for(const auto &model : container)
    {
        result.append(model->getID());
    }
    return result;
}

QModelIndex UserModel::fromKey(const IDContainer &key)
{
    if(key == savedPlaylist->getID())
    {
        return index(SAVED_INDEX);
    }
    else if(key == likedPlaylist->getID())
    {
        return index(LIKED_INDEX);
    }

    IDs keys = getKeys();
    auto it = std::lower_bound(keys.cbegin(), keys.cend(), key);
    if(it != keys.cend() && *it == key)
    {
        return index(std::distance(keys.cbegin(), it));
    }
    else
    {
        qDebug() << "The given key was not found";
    }

    return QModelIndex();
}

int UserModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return container.size() + metaData.getSpecialKeys().size();
}

bool UserModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid())
    {
        qDebug() << "Cannot delete a valid parent";
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    for(int i = row; i < (row + count); ++i)
    {
        if(i == LIKED_INDEX || i == SAVED_INDEX)
        {
            qDebug() << "Cannot delete special playlists.";
            return false;
        }

        auto model = container.takeAt(i);
        QDir target(MainFolder::getPlaylists().absolutePath());
        bool result = target.cd(QString("%1").arg(model->getID()));
        if(result)
        {
            target.removeRecursively();
        }
        else
        {
            qDebug() << QString("The directory with %1 was not found.").arg(model->getID());
        }
    }
    User::saveIDsToRecord(getKeys(), mainKey);
    endRemoveRows();
    return true;
}

bool UserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        qDebug() << "Index is not valid.";
        return false;
    }
    else if(index.row() == LIKED_INDEX || index.row() == SAVED_INDEX)
    {
        qDebug() << "Liked or saved playlist cannot be modified.";
        return false;
    }
    else if(role != User::ModelRole)
    {
        container[index.row()]->setHeaderData(0, Qt::Horizontal, value, role);
        emit dataChanged(index, index, {role, Qt::DecorationRole});
    }
    else
    {
        container.replace(index.row(), value.value<std::shared_ptr<PlaylistModel>>());
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::DecorationRole, User::KeyRole, User::ModelRole, User::KeyListRole});
    }
    return true;
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }
    if(index.row() == SAVED_INDEX)
    {
        switch(role)
        {
        case Qt::DisplayRole:
        {
            return savedPlaylist->headerData(0, Qt::Horizontal, Playlist::NameRole);
        }
        case Qt::DecorationRole:
        {
            return QPixmap(":/images/playlist/saved-songs.png");
        }
        case User::ModelRole:
        {
            return QVariant::fromValue<PlaylistModel*>(savedPlaylist.get());
        }
        case User::KeyRole:
        {
            return savedPlaylist->getID();
        }
        case User::KeyListRole:
        {
            return QVariant::fromValue(savedPlaylist->getKeys());
        }
        default:
        {
            return {};
        }
        }
    }
    else if(index.row() == LIKED_INDEX)
    {
        switch(role)
        {
        case Qt::DisplayRole:
        {
            return likedPlaylist->headerData(0, Qt::Horizontal, Playlist::NameRole);
        }
        case Qt::DecorationRole:
        {
            return QPixmap(":/images/playlist/liked-songs.png");
        }
        case User::ModelRole:
        {
            return QVariant::fromValue<PlaylistModel*>(likedPlaylist.get());
        }
        case User::KeyRole:
        {
            return likedPlaylist->getID();
        }
        case User::KeyListRole:
        {
            return QVariant::fromValue(likedPlaylist->getKeys());
        }
        default:
        {
            return {};
        }
        }
    }

    auto model = container.at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
    {
        return model->headerData(0, Qt::Horizontal, Playlist::NameRole);
    }
    case Qt::DecorationRole:
    {
        return model->data(model->index(0), Playlist::CoverRole);
    }
    case User::KeyListRole:
    {
        return QVariant::fromValue<IDs>(model->getKeys());
    }
    case User::ModelRole:
    {
        return QVariant::fromValue<PlaylistModel*>(model.get());
    }
    case User::KeyRole:
    {
        return model.get()->getID();
    }
    default:
    {
        return {};
    }
    }
}

bool UserModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(section != 0)
    {
        return false;
    }
    switch(role)
    {
    case User::FirstNameRole:
    {
        metaData.setFirstName(value.toString());
        break;
    }
    case User::LastNameRole:
    {
        metaData.setLastName(value.toString());
        break;
    }
    case User::UserNameRole:
    {
        metaData.setUserName(value.toString());
        break;
    }
    case User::PasswordRole:
    {
        metaData.setPassword(value.toString());
        break;
    }
    default:
    {
        return false;
    }
    }
    metaData.saveToRecord(mainKey);
    emit headerDataChanged(orientation, section, section);
    return true;
}
