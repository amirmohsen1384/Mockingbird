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
    bool isValid(const IDContainer &value);
}

constexpr int _min_year = 1900;

#endif // GENERAL_H
