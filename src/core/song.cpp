#include "include/core/song.h"
#include <QDataStream>

QString Song::getName() const
{
    return name;
}

QImage Song::getCover() const
{
    return cover;
}

Song::Genre Song::getGenre() const
{
    return genre;
}

QString Song::getArtist() const
{
    return artist;
}

int Song::getPublicationYear() const
{
    return publicationYear;
}

bool Song::operator==(const Song &other) const
{
    if(this->name != other.name)
    {
        return false;
    }
    else if(this->genre != other.genre)
    {
        return false;
    }
    else if(this->artist != other.artist)
    {
        return false;
    }
    else if(this->publicationYear != other.publicationYear)
    {
        return false;
    }
    else if(this->address != other.address)
    {
        return false;
    }
    else if(this->cover != other.cover)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Song::operator!=(const Song &other) const
{
    return !(*this == other);
}

void Song::saveToRecord(const IDContainer &value) const
{
    QFile file(absoluteRecord(value));
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }
    QDataStream stream(&file);
    stream << *this;
    return;
}

IDContainer Song::generateKey()
{
    IDs keys;
    IDContainer result = 0;
    QFileInfoList entries = MainFolder::getSongs().entryInfoList({"*.sof"}, QDir::AllEntries | QDir::NoDotAndDotDot);
    for(QFileInfo entry : entries)
    {
        keys.append(entry.baseName().toLongLong());
    }
    do
    {
        result = QRandomGenerator::global()->generate();
    }
    while(keys.contains(result));
    return result;
}

Song Song::loadFromRecord(const IDContainer &value)
{
    QFile file(absoluteRecord(value));
    if(!file.open(QFile::ReadOnly))
    {
        return Song();
    }
    Song song;
    QDataStream stream(&file);
    stream >> song;
    return song;
}

QString Song::absoluteRecord(const IDContainer &value)
{
    return MainFolder::getSongs().absoluteFilePath(QString("%1.sof").arg(value));
}

bool Song::isNull() const
{
    return address.isEmpty() || !address.isValid();
}

QUrl Song::getAddress() const
{
    return address;
}

void Song::setName(const QString &value)
{
    name = value;
}

void Song::setGenre(Genre value)
{
    genre = value;
}
void Song::setArtist(const QString &value)
{
    artist = value;
}

void Song::setCover(const QImage &value)
{
    cover = value;
}

void Song::setPublicationYear(int value)
{
    publicationYear = value;
}
void Song::setAddress(const QUrl &value)
{
    address = value;
}

QDataStream& operator<<(QDataStream &stream, const Song &another)
{
    stream << another.name;
    stream << another.genre;
    stream << another.cover;
    stream << another.artist;
    stream << another.address;
    stream << another.publicationYear;
    return stream;
}

QDataStream& operator>>(QDataStream &stream, Song &another)
{
    stream >> another.name;
    stream >> another.genre;
    stream >> another.cover;
    stream >> another.artist;
    stream >> another.address;
    stream >> another.publicationYear;
    return stream;
}
