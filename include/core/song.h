#ifndef SONG_H
#define SONG_H

#include <QUrl>
#include <QImage>
#include <QVariant>
#include "general.h"

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
        Classical,
        Folk,
        Country,
        Blues,
        Reggae,
        Alternative,
        Cassette,
        Children,
        Christian,
        Demon,
        EasyListening,
        Enka,
        FrenchMusic,
        Goth,
        HipHop,
        International,
        IrishMusic,
        Lullaby,
        Metal,
        Oud,
        ProgressiveRock,
        Punk,
        RnB,
        Rhythm,
        Ska,
        TexMex,
        UndergroundMusic,
        UsMusic,
        Vaporwave
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
    Genre genre = Genre::Classical;
};

QDataStream& operator<<(QDataStream &stream, const Song &another);
QDataStream& operator>>(QDataStream &stream, Song &another);

Q_DECLARE_METATYPE(Song)

using SongList = QList<Song>;

struct SongInfo
{
    Song data;
    IDContainer key;
};

using SongInfoList = QList<SongInfo>;

#endif // SONG_H
