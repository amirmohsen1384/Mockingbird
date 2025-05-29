#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDataStream>
#include "general.h"
#include <QString>
#include "song.h"

class Playlist : public Entity
{
public:
    enum Role
    {
        GenreTextRole = Qt::UserRole + 3,
        GenreIconRole = Qt::UserRole + 4,
        PlayingRole = Qt::UserRole + 1,
        ArtistRole = Qt::UserRole + 2,
        GenreRole = Qt::UserRole + 6,
        YearRole = Qt::UserRole + 5,
        KeyRole = Qt::UserRole + 7
    };

    QString getName() const;

    friend QDataStream& operator<<(QDataStream &stream, const Playlist &data);
    friend QDataStream& operator>>(QDataStream &stream, Playlist &data);

    static IDs loadIDsFromRecord(const IDContainer &value);
    static void saveIDsToRecord(const IDs &idList, const IDContainer &value);

    static Playlist loadFromRecord(const IDContainer &value);
    virtual void saveToRecord(const IDContainer &value) const override;

    static QDir absoluteRecord(const IDContainer &value);

public:
    void setName(const QString &value);

private:
    QString name;
};

QDataStream& operator<<(QDataStream &stream, const Playlist &data);
QDataStream& operator>>(QDataStream &stream, Playlist &data);

#endif // PLAYLIST_H
