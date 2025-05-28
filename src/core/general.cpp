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


bool ID::isValid(const IDContainer &value)
{
    return value > 0;
}

IDContainer ID::generateKey()
{
    auto artists = MainFolder::getArtists().entryInfoList({"*"}, QDir::AllDirs | QDir::NoDotAndDotDot);
    auto songs = MainFolder::getSongs().entryInfoList({"*.sof"}, QDir::AllEntries | QDir::NoDotAndDotDot);
    auto playlists = MainFolder::getPlaylists().entryInfoList({"*"}, QDir::AllDirs | QDir::NoDotAndDotDot);

    IDs existingIDs;
    for(QFileInfo file : songs)
    {
        existingIDs.append(file.baseName().toLongLong());
    }
    for(QFileInfo file : artists)
    {
        existingIDs.append(file.baseName().toLongLong());
    }
    for(QFileInfo file : playlists)
    {
        existingIDs.append(file.baseName().toLongLong());
    }

    IDContainer result = 0;
    do
    {
        result = QRandomGenerator::global()->generate();
    }
    while(existingIDs.contains(result) || !ID::isValid(result));

    return result;
}
