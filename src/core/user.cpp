#include "include/core/user.h"
#include "include/core/playlist.h"

QDir User::createRecord(const IDContainer &value)
{
    const QString target = QString::number(value);
    QDir result = MainFolder::getUsers();
    result.mkdir(target);
    result.cd(target);
    {
        const QString keyPath = result.absoluteFilePath("keys.dat");
        QFile keyFile(keyPath);
        if(!keyFile.exists())
        {
            keyFile.open(QFile::WriteOnly);
        }
    }
    {
        const QString metaPath = result.absoluteFilePath("metadata.dat");
        QFile metaFile(metaPath);
        if(!metaFile.exists())
        {
            metaFile.open(QFile::WriteOnly);
        }
    }
    return result;
}

User User::loadFromRecord(const IDContainer &value)
{
    if(!ID::isValid(value))
    {
        return User();
    }
    QDir result = MainFolder::getUsers();
    if(!result.cd(QString::number(value)))
    {
        return User();
    }
    QFile file(result.absoluteFilePath("metadata.dat"));
    if(!file.open(QFile::ReadOnly))
    {
        return User();
    }
    User data;
    QDataStream stream(&file);
    if((stream >> data).status() == QDataStream::Ok)
    {
        data.valid = true;
        return data;
    }
    else
    {
        return User();
    }
}

IDs User::loadIDsFromRecord(const IDContainer &value)
{
    if(!ID::isValid(value))
    {
        return IDs();
    }
    QDir folder = MainFolder::getUsers();
    if(!folder.cd(QString::number(value)))
    {
        return IDs();
    }
    QFile file(folder.absoluteFilePath("keys.dat"));
    if(!file.open(QFile::ReadOnly))
    {
        return IDs();
    }
    IDs idList;
    QDataStream stream(&file);
    if((stream >> idList).status() != QDataStream::Ok)
    {
        return IDs();
    }
    return idList;
}

void User::saveToRecord(const IDContainer &value) const
{
    if(!ID::isValid(value))
    {
        return;
    }
    const QString target = QString::number(value);
    QDir folder = MainFolder::getUsers();
    if(!folder.cd(target))
    {
        folder = createRecord(value);
    }
    QFile file(folder.absoluteFilePath("metadata.dat"));
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }
    QDataStream stream(&file);
    stream << *this;
}

void User::createAccount(User &user)
{
    // Save the meta information of the song
    const IDContainer userKey = ID::generateKey();

    // Create a playlist for saved songs
    Playlist saved;
    saved.setName("Saved Songs");
    const IDContainer savedKey = ID::generateKey();
    saved.saveToRecord(savedKey);

    // Create a playlist for liked songs
    Playlist liked;
    liked.setName("Liked Songs");
    const IDContainer likedKey = ID::generateKey();
    liked.saveToRecord(likedKey);

    // Save the created playlist for the user
    user.specialKeys.insert(LIKED_INDEX, likedKey);
    user.specialKeys.insert(SAVED_INDEX, savedKey);

    // Save the record into the database
    user.saveToRecord(userKey);

}

void User::saveIDsToRecord(const IDs &idList, const IDContainer &value)
{
    if(!ID::isValid(value))
    {
        return;
    }
    const QString target = QString::number(value);
    QDir folder = MainFolder::getUsers();
    if(!folder.cd(target))
    {
        folder = createRecord(value);
    }
    QFile file(folder.absoluteFilePath("keys.dat"));
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }
    QDataStream stream(&file);
    stream << idList;
}

const IDs &User::getSpecialKeys() const
{
    return specialKeys;
}

IDContainer User::getLikedPlaylist() const
{
    return specialKeys.at(LIKED_INDEX);
}

IDContainer User::getSavedPlaylist() const
{
    return specialKeys.at(SAVED_INDEX);
}

QDataStream& operator<<(QDataStream &stream, const User &data)
{
    stream << static_cast<const Person&>(data);
    stream << data.specialKeys;
    return stream;
}
QDataStream& operator>>(QDataStream &stream, User &data)
{
    stream >> static_cast<Person&>(data);
    stream >> data.specialKeys;
    return stream;
}
