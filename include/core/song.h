#ifndef SONG_H
#define SONG_H

#include <QUrl>
#include <QImage>
#include <QVariant>
#include "general.h"

class Entity
{
public:
    bool isNull() const;
    virtual void saveToRecord(const IDContainer &value) const = 0;

protected:
    bool valid = false;
};

class Song : public Entity
{
public:
    enum Genre
    {
        Rock,
        Rap,
        Pop,
        Jazz,
        Electronic,
        Classic,
        Folk,
        Country
    };

public:
    Genre getGenre() const;
    QUrl getAddress() const;
    QString getName() const;
    QImage getCover() const;
    QString getArtist() const;
    int getPublicationYear() const;
    
    bool operator==(const Song &other) const;
    bool operator!=(const Song &other) const;

    friend QDataStream& operator<<(QDataStream &stream, const Song &another);
    friend QDataStream& operator>>(QDataStream &stream, Song &another);

    static Song loadFromRecord(const IDContainer &value);
    virtual void saveToRecord(const IDContainer &value) const override;

    friend class SongEdit;

public:
    void setArtist(const QString &value);
    void setCover(const QImage &value);
    void setName(const QString &value);
    void setPublicationYear(int value);
    void setAddress(const QUrl &value);
    void setGenre(Genre value);

private:
    QUrl address;
    QString name;
    QImage cover;
    QString artist;
    int publicationYear;
    Genre genre = Genre::Classic;
};

QDataStream& operator<<(QDataStream &stream, const Song &another);
QDataStream& operator>>(QDataStream &stream, Song &another);

Q_DECLARE_METATYPE(Song)

using SongList = QList<Song>;

#endif // SONG_H
