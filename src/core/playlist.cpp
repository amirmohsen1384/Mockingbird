#include "include/core/playlist.h"

QString Playlist::getName() const
{
    return name;
}

bool Playlist::isNull() const
{
    return name.isEmpty();
}

bool Playlist::operator==(const Playlist &another)
{
    return this->name == another.name;
}

bool Playlist::operator!=(const Playlist &another)
{
    return !(*this == another);
}

void Playlist::saveToRecord(const IDContainer &value)
{
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
    result.mkpath(target);
    result.cd(target);
    return result;
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
    stream >> data;
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
