#include "include/models/genremodel.h"
#include <QPixmap>

QPixmap GenreModel::toPixmap(const Song::Genre &genre)
{
    switch(genre)
    {
        case Song::Genre::Classic: 
        {
            return QPixmap(":/images/genres/classic.png");
        }
        case Song::Genre::Pop: 
        {
            return QPixmap(":/images/genres/pop.png");
        }
        case Song::Genre::Rock: 
        {
            return QPixmap(":/images/genres/rock.png");
        }
        case Song::Genre::Jazz: 
        {
            return QPixmap(":/images/genres/jazz.png");
        }
        case Song::Genre::Rap: 
        {
            return QPixmap(":/images/genres/rap.png");
        }
        case Song::Genre::Electronic: 
        {
            return QPixmap(":/images/genres/electronic.png");
        }
        case Song::Genre::Country: 
        {
            return QPixmap(":/images/genres/country.png");
        }
        case Song::Genre::Folk: 
        {
            return QPixmap(":/images/genres/folk.png");
        }
        default: 
        {
            return QPixmap();
        }
    }
}

QString GenreModel::toString(const Song::Genre &genre)
{
    switch(genre)
    {
        case Song::Genre::Classic:
        {
            return QString("Classic");
        }
        case Song::Genre::Pop:
        {
            return QString("Pop");
        }
        case Song::Genre::Rock:
        {
            return QString("Rock");
        }
        case Song::Genre::Jazz:
        {
            return QString("Jazz");
        }
        case Song::Genre::Rap:
        {
            return QString("Rap");
        }
        case Song::Genre::Electronic:
        {
            return QString("Electronic");
        }
        case Song::Genre::Country:
        {
            return QString("Country");
        }
        case Song::Genre::Folk:
        {
            return QString("Folk");
        }
        default:
        {
            return QString();
        }
    }
}

QPair<QString, QPixmap> GenreModel::toMetaData(const Song::Genre &genre)
{
    return QPair<QString, QPixmap>(GenreModel::toString(genre), GenreModel::toPixmap(genre));
}

GenreModel::GenreModel(QObject *parent) : QAbstractListModel(parent)
{}

int GenreModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    // The count of available enumeration values
    return 8;
}

QVariant GenreModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    Song::Genre genre = static_cast<Song::Genre>(index.row());
    switch(role) {
        case Qt::DisplayRole: 
        {
            return GenreModel::toString(genre);
        }
        case Qt::DecorationRole: 
        {
            return GenreModel::toPixmap(genre);
        }
        case Qt::UserRole:
        {
            return QVariant::fromValue(genre);
        }
        default:
        {
            return {};
        }
    }
}
