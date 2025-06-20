#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDataStream>
#include "general.h"
#include <QString>

class Playlist : public Entity
{
public:
    enum Role
    {
        GenreTextRole = Qt::UserRole + 3,
        GenreIconRole = Qt::UserRole + 4,
        PlayingRole = Qt::UserRole + 1,
        CoverRole = Qt::DecorationRole,
        ArtistRole = Qt::UserRole + 2,
        GenreRole = Qt::UserRole + 6,
        YearRole = Qt::UserRole + 5,
        NameRole = Qt::DisplayRole,
        KeyRole = Qt::UserRole + 7,
        InfoRole = Qt::UserRole + 8
    };

    QString getName() const;

    virtual void saveToRecord(const IDContainer &value) const override;

    friend QDataStream& operator>>(QDataStream &stream, Playlist &data);
    friend QDataStream& operator<<(QDataStream &stream, const Playlist &data);

    static IDs loadIDsFromRecord(const IDContainer &value);
    static Playlist loadFromRecord(const IDContainer &value);
    static void saveIDsToRecord(const IDs &idList, const IDContainer &value);

public:
    void setName(const QString &value);

private:
    static QDir createRecord(const IDContainer &value);

private:
    QString name;
};

QDataStream& operator<<(QDataStream &stream, const Playlist &data);
QDataStream& operator>>(QDataStream &stream, Playlist &data);

#endif // PLAYLIST_H
