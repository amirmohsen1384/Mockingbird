#ifndef GENERAL_H
#define GENERAL_H

#include <QDir>
#include <QApplication>
#include <QRandomGenerator64>

namespace MainFolder
{
    QDir getRoot();
    QDir getUser();
    QDir getAdmin();
    QDir getSongs();
    QDir getArtists();
    QDir getPlaylists();
}

using IDContainer = qint64;
using IDs = QList<IDContainer>;

namespace ID
{
    IDContainer generateKey();
    bool isValid(const IDContainer &value);
}

constexpr int _min_year = 1900;

class Entity
{
public:
    bool isNull() const;
    virtual void saveToRecord(const IDContainer &value) const = 0;

protected:
    bool valid = false;
};

#endif // GENERAL_H
