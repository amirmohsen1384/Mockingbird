#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDataStream>
#include <QString>

class Playlist
{
public:
    enum Role
    {
        GenreTextRole = Qt::UserRole + 3,
        GenreIconRole = Qt::UserRole + 4,
        PlayingRole = Qt::UserRole + 1,
        ArtistRole = Qt::UserRole + 2,
        GenreRole = Qt::UserRole + 6,
        YearRole = Qt::UserRole + 5
    };
    QString getName() const;

    bool operator==(const Playlist &another);
    bool operator!=(const Playlist &another);

    friend QDataStream& operator<<(QDataStream &stream, const Playlist &data);
    friend QDataStream& operator>>(QDataStream &stream, Playlist &data);

public:
    void setName(const QString &value);

private:
    QString name;
};

QDataStream& operator<<(QDataStream &stream, const Playlist &data);
QDataStream& operator>>(QDataStream &stream, Playlist &data);

#endif // PLAYLIST_H
