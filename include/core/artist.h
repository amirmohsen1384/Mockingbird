#ifndef ARTIST_H
#define ARTIST_H

#include <QImage>
#include "general.h"

class Artist : public Entity
{
public:
    enum Role
    {
        KeyRole = Qt::UserRole,
        NameRole = Qt::DisplayRole,
        DataRole = Qt::UserRole + 4,
        PhotoRole = Qt::UserRole + 2,
        ModelRole = Qt::UserRole + 3,
        BiographyRole = Qt::UserRole + 1
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

    virtual void saveToRecord(const IDContainer &value) const override;

    friend QDataStream& operator>>(QDataStream &stream, Artist &target);
    friend QDataStream& operator<<(QDataStream &stream, const Artist &target);

    static Artist loadFromRecord(const IDContainer &value);
    static IDs loadIDsFromRecord(const IDContainer &value);
    static void saveIDsToRecord(const IDs &idList, const IDContainer &value);

private:
    static QDir createRecord(const IDContainer &value);

private:
    QString name;
    QImage photo;
    QString biography;
};

QDataStream& operator>>(QDataStream &stream, Artist &target);
QDataStream& operator<<(QDataStream &stream, const Artist &target);

#endif // ARTIST_H
