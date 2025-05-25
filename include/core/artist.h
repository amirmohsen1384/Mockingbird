#ifndef ARTIST_H
#define ARTIST_H

#include <QImage>

class Artist
{
public:
    enum Role
    {
        NameRole = Qt::DisplayRole,
        PhotoRole = Qt::UserRole + 2,
        BiographyRole = Qt::UserRole + 1,
    };

public:
    QString getName() const;
    void setName(const QString &value);

    QString getBiography() const;
    void setBiography(const QString &value);

    QImage getPhoto() const;
    void setPhoto(const QImage &value);

    bool operator==(const Artist &other) const;
    bool operator!=(const Artist &other) const;

    friend QDataStream& operator>>(QDataStream &stream, Artist &target);
    friend QDataStream& operator<<(QDataStream &stream, const Artist &target);

private:
    QString name;
    QImage photo;
    QString biography;
};

QDataStream& operator>>(QDataStream &stream, Artist &target);
QDataStream& operator<<(QDataStream &stream, const Artist &target);

#endif // ARTIST_H
