#include "include/core/artist.h"

QString Artist::getName() const
{
    return name;
}

void Artist::setName(const QString &value)
{
    name = value;
}

QString Artist::getBiography() const
{
    return biography;
}

void Artist::setBiography(const QString &value)
{
    biography = value;
}

QImage Artist::getPhoto() const
{
    return photo;
}

void Artist::setPhoto(const QImage &value)
{
    photo = value;
}

bool Artist::operator==(const Artist &other) const
{
    if(this->name != other.name)
    {
        return false;
    }
    else if(this->photo != other.photo)
    {
        return false;
    }
    else if(this->biography != other.biography)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Artist::operator!=(const Artist &other) const
{
    return !(*this == other);
}

void Artist::saveToRecord(const IDContainer &value) const
{
    const QString &target = QString::number(value);
    QDir folder = MainFolder::getArtists();
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

void Artist::saveIDsToRecord(const IDs &idList, const IDContainer &value)
{
    if(!ID::isValid(value))
    {
        return;
    }
    const QString target = QString::number(value);
    QDir folder = MainFolder::getArtists();
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

Artist Artist::loadFromRecord(const IDContainer &value)
{
    if(!ID::isValid(value))
    {
        return Artist();
    }
    QDir result = MainFolder::getArtists();
    if(!result.cd(QString::number(value)))
    {
        return Artist();
    }
    QFile file(result.absoluteFilePath("metadata.dat"));
    if(!file.open(QFile::ReadOnly))
    {
        return Artist();
    }
    Artist data;
    QDataStream stream(&file);
    if((stream >> data).status() == QDataStream::Ok)
    {
        data.valid = true;
        return data;
    }
    else
    {
        return Artist();
    }
}

IDs Artist::loadIDsFromRecord(const IDContainer &value)
{
    if(!ID::isValid(value))
    {
        return IDs();
    }
    QDir folder = MainFolder::getArtists();
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

QDir Artist::createRecord(const IDContainer &value)
{
    const QString &target = QString::number(value);
    QDir result = MainFolder::getArtists();
    result.mkdir(target);
    result.cd(target);
    {
        QFile metaFile(result.absoluteFilePath("metadata.dat"));
        if(!metaFile.exists())
        {
            metaFile.open(QFile::WriteOnly);
        }
    }
    {
        QFile keyFile(result.absoluteFilePath("keys.dat"));
        if(!keyFile.exists())
        {
            keyFile.open(QFile::WriteOnly);
        }
    }
    return result;
}

QDataStream& operator>>(QDataStream &stream, Artist &target)
{
    stream >> target.name;
    stream >> target.photo;
    stream >> target.biography;
    return stream;
}

QDataStream& operator<<(QDataStream &stream, const Artist &target)
{
    stream << target.name;
    stream << target.photo;
    stream << target.biography;
    return stream;
}
