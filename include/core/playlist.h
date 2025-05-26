#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDataStream>
#include "general.h"
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

    bool isNull() const;

    bool operator==(const Playlist &another);
    bool operator!=(const Playlist &another);

    friend QDataStream& operator<<(QDataStream &stream, const Playlist &data);
    friend QDataStream& operator>>(QDataStream &stream, Playlist &data);

    void saveToRecord(const IDContainer &value);

    static QDir absoluteRecord(const IDContainer &value);
    static IDs loadIDsFromRecord(const IDContainer &value);
    static Playlist loadFromRecord(const IDContainer &value);

public:
    void setName(const QString &value);

private:
    QString name;
};

QDataStream& operator<<(QDataStream &stream, const Playlist &data);
QDataStream& operator>>(QDataStream &stream, Playlist &data);

#endif // PLAYLIST_H
