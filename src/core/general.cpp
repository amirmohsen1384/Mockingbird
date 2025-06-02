#include "include/core/general.h"
#include <QImageReader>
#include <QMediaFormat>
#include <QFileDialog>

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

QDir MainFolder::getUsers()
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

bool Entity::isNull() const
{
    return !valid;
}

bool ImageFile::isValid(const QUrl &filename)
{
    const auto formats = QImageReader::supportedImageFormats();
    QFileInfo info(filename.toLocalFile());
    if(!info.isFile())
    {
        return false;
    }
    QString extenstion = info.suffix().toLower();
    for(const auto &format : formats)
    {
        if(extension == QString::fromLatin1(format).toLower())
        {
            return true;
        }
    }
    return false;
}

std::shared_ptr<QFileDialog> ImageFile::initializeImageDialog(QWidget *parent)
{
    const auto formats = QImageReader::supportedImageFormats();
    std::shared_ptr<QFileDialog> dialog = std::make_shared<QFileDialog>(parent);
    dialog->setMimeTypeFilter(formats);
    dialog->setDirectory(QDir::home());
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setWindowTitle("Select an image to open");
    dialog->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    return dialog;
}

bool SongFile::isValid(const QUrl &filename)
{
    QFileInfo file(filename.toLocalFile());
    if(!file.isFile())
    {
        return false;
    }
    for(const QString &data : formats)
    {
        if(file.suffix().toLower() == data)
        {
            return true;
        }
    }
    return false;
}

std::shared_ptr<QFileDialog> SongFile::initializeSongDialog(QWidget *parent)
{
    std::shared_ptr<QFileDialog> dialog = std::make_shared<QFileDialog>(parent);
    dialog->setDirectory(QDir::home());
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setWindowTitle("Select a file to open");
    dialog->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QString nameFilter = "Songs (";
    for(const QString &extension : songs)
    {
        nameFilter.append(QString(" *.%1").arg(extension));
    }
    nameFilter.append(" )");
    dialog->setNameFilter(nameFilter);
    return dialog;
}
