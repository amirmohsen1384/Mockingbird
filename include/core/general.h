#ifndef GENERAL_H
#define GENERAL_H

#include <QDir>
#include <QApplication>

namespace MainFolder
{
    QDir getRoot();
    QDir getUser();
    QDir getAdmin();
    QDir getSongs();
    QDir getArtists();
    QDir getPlaylists();
}

constexpr int _min_year = 1900;

#endif // GENERAL_H
