#include "include/core/general.h"

QDir MainFolder::getRoot()
{
    const QString name = QApplication::applicationName();
    QDir result = QDir::temp();
    result.mkdir(name);
    result.cd(name);
    return result;
}

QDir MainFolder::getAdmin()
{
    const QString name = "Admins";
    QDir result = getRoot();
    result.mkdir(name);
    result.cd(name);
    return result;
}

QDir MainFolder::getUser()
{
    const QString name = "Users";
    QDir result = getRoot();
    result.mkdir(name);
    result.cd(name);
    return result;
}

QDir MainFolder::getSongs()
{
    const QString name = "Songs";
    QDir result = getRoot();
    result.mkdir(name);
    result.cd(name);
    return result;
}

QDir MainFolder::getPlaylists()
{
    const QString name = "Playlists";
    QDir result = getRoot();
    result.mkdir(name);
    result.cd(name);
    return result;
}

QDir MainFolder::getArtists()
{
    const QString name = "Artists";
    QDir result = getRoot();
    result.mkdir(name);
    result.cd(name);
    return result;
}

