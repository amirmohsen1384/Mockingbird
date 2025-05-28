#include "include/core/playlist.h"

QString Playlist::getName() const
{
    return name;
}

void Playlist::saveToRecord(const IDContainer &value) const
{
    if(!ID::isValid(value))
    {
        return;
    }
    QFile file(Playlist::absoluteRecord(value).absoluteFilePath("metadata.dat"));
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }
    QDataStream stream(&file);
    stream << *this;
}

QDir Playlist::absoluteRecord(const IDContainer &value)
{
    const QString target = QString::number(value);
    QDir result = MainFolder::getPlaylists();
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

void Playlist::saveIDsToRecord(const IDs &idList, const IDContainer &value)
{
    if(!ID::isValid(value))
    {
        return;
    }
    QFile file(Playlist::absoluteRecord(value).absoluteFilePath("keys.dat"));
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }
    QDataStream stream(&file);
    stream << idList;
}

IDs Playlist::loadIDsFromRecord(const IDContainer &value)
{
    QDir record = Playlist::absoluteRecord(value);
    QFile file(record.absoluteFilePath("keys.dat"));
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

Playlist Playlist::loadFromRecord(const IDContainer &value)
{
    QFile file(Playlist::absoluteRecord(value).absoluteFilePath("metadata.dat"));
    if(!file.open(QFile::ReadOnly))
    {
        return Playlist();
    }
    Playlist data;
    QDataStream stream(&file);
    if((stream >> data).status() == QDataStream::Ok)
    {
        data.valid = true;
    }
    return data;
}

void Playlist::setName(const QString &value)
{
    name = value;
}

QDataStream& operator<<(QDataStream &stream, const Playlist &data)
{
    stream << data.name;
    return stream;
}

QDataStream& operator>>(QDataStream &stream, Playlist &data)
{
    stream >> data.name;
    return stream;
}
