#include "include/core/playlist.h"

QString Playlist::getName() const
{
    return name;
}

bool Playlist::operator==(const Playlist &another)
{
    return this->name == another.name;
}

bool Playlist::operator!=(const Playlist &another)
{
    return !(*this == another);
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
