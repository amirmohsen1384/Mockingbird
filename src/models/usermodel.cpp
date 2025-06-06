#include "include/models/usermodel.h"

void UserModel::updateModel()
{
    container.clear();

    if(!ID::isValid(mainKey))
    {
        return;
    }

    // Loads the meta-information into the model
    metaData = User::loadFromRecord(mainKey);
    if(metaData.isNull())
    {
        return;
    }

    // Loads keys into the model
    IDs keys = User::loadIDsFromRecord(mainKey);
    for(const IDContainer &key : keys)
    {
        auto model = std::make_shared<PlaylistModel>(key);
        if(model->rowCount() > 0)
        {
            container.append(model);
        }
    }
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
        return;
    }

    IDs keys = getKeys();
    auto it = std::lower_bound(keys.cbegin(), keys.cend(), target);
    if(*it == target && it != keys.cend())
    {
        return;
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
    if(key == container.at(LIKED_INDEX)->getID() && key == container.at(SAVED_INDEX)->getID())
    {
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
    IDs keys = getKeys();
    auto it = std::lower_bound(keys.cbegin(), keys.cend(), key);
    if(it != keys.cend() && *it == key)
    {
        return index(std::distance(keys.cbegin(), it));
    }
    return QModelIndex();
}

int UserModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return container.size();
}

bool UserModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid())
    {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = row; i < (row + count); ++i)
    {
        if(i == LIKED_INDEX || i == SAVED_INDEX)
        {
            return false;
        }
        auto model = container.takeAt(i);
        QDir target(MainFolder::getPlaylists().absolutePath());
        bool result = target.cd(QString("%1").arg(model->getID()));
        if(result)
        {
            target.removeRecursively();
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
        return false;
    }
    else if(index.row() == LIKED_INDEX || index.row() == SAVED_INDEX)
    {
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
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::DecorationRole, User::KeyRole, User::ModelRole});
    }
    return true;
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }
    auto model = container.at(index.row());
    switch(role)
    {
    case Qt::DisplayRole:
    {
        return model->headerData(Playlist::NameRole);
    }
    case Qt::DecorationRole:
    {
        if(index.row() == LIKED_INDEX)
        {
            return QPixmap(":/images/playlist/liked-songs.png");
        }
        else if(index.row() == SAVED_INDEX)
        {
            return QPixmap(":/images/playlist/saved-songs.png");
        }
        else
        {
            return model->data(model->index(0), Playlist::CoverRole);
        }
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
    case Qt::BackgroundRole:
    {
        if(index.row() == LIKED_INDEX)
        {
            return QColor(250, 210, 210);
        }
        else if(index.row() == SAVED_INDEX)
        {
            return QColor(220, 220, 220);
        }
        else
        {
            return {};
        }
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
