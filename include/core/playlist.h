#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDataStream>

class Playlist
{
public:
    QString getName() const;

    bool operator==(const Playlist &another);
    bool operator!=(const Playlist &another);

    friend QDataStream& operator<<(QDataStream &stream, const Playlist &data);
    friend QDataStream& operator>>(QDataStream &stream, const Playlist &data);

public:
    void setName(const QString &value);

private:
    QString name;
};

QDataStream& operator<<(QDataStream &stream, const Playlist &data);
QDataStream& operator>>(QDataStream &stream, const Playlist &data);

#endif // PLAYLIST_H
