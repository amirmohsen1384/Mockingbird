#include "include/models/genremodel.h"
#include <QPixmap>

QPixmap GenreModel::toPixmap(const Song::Genre &genre)
{
    switch(genre)
    {
        case Song::Genre::Classical: 
        {
            return QPixmap(":/images/genres/classical.png");
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
        case Song::Genre::Blues:
        {
            return QPixmap(":/images/genres/blues.png");
        }
        case Song::Genre::Reggae:
        {
            return QPixmap(":/images/genres/reggae.png");
        }
        case Song::Genre::Alternative:
        {
            return QPixmap(":/images/genres/alternative.png");
        }
        case Song::Genre::Cassette:
        {
            return QPixmap(":/images/genres/cassette.png");
        }
        case Song::Genre::Children:
        {
            return QPixmap(":/images/genres/children.png");
        }
        case Song::Genre::Christian:
        {
            return QPixmap(":/images/genres/christian.png");
        }
        case Song::Genre::Demon:
        {
            return QPixmap(":/images/genres/demon.png");
        }
        case Song::Genre::EasyListening:
        {
            return QPixmap(":/images/genres/easy-listening.png");
        }
        case Song::Genre::Enka:
        {
            return QPixmap(":/images/genres/enka.png");
        }
        case Song::Genre::FrenchMusic:
        {
            return QPixmap(":/images/genres/french.png");
        }
        case Song::Genre::Goth:
        {
            return QPixmap(":/images/genres/goth.png");
        }
        case Song::Genre::HipHop:
        {
            return QPixmap(":/images/genres/hip-hop.png");
        }
        case Song::Genre::International:
        {
            return QPixmap(":/images/genres/international.png");
        }
        case Song::Genre::IrishMusic:
        {
            return QPixmap(":/images/genres/irish.png");
        }
        case Song::Genre::Lullaby:
        {
            return QPixmap(":/images/genres/lullaby.png");
        }
        case Song::Genre::Metal:
        {
            return QPixmap(":/images/genres/metal.png");
        }
        case Song::Genre::Oud:
        {
            return QPixmap(":/images/genres/oud.png");
        }
        case Song::Genre::ProgressiveRock:
        {
            return QPixmap(":/images/genres/progressive-rock.png");
        }
        case Song::Genre::Punk:
        {
            return QPixmap(":/images/genres/punk.png");
        }
        case Song::Genre::RnB:
        {
            return QPixmap(":/images/genres/rnb.png");
        }
        case Song::Genre::Rhythm:
        {
            return QPixmap(":/images/genres/rhythm.png");
        }
        case Song::Genre::Ska:
        {
            return QPixmap(":/images/genres/ska.png");
        }
        case Song::Genre::TexMex:
        {
            return QPixmap(":/images/genres/tex-mex.png");
        }
        case Song::Genre::UndergroundMusic:
        {
            return QPixmap(":/images/genres/underground.png");
        }
        case Song::Genre::UsMusic:
        {
            return QPixmap(":/images/genres/us.png");
        }
        case Song::Genre::Vaporwave:
        {
            return QPixmap(":/images/genres/vaporwave.png");
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
        case Song::Genre::Classical:
        {
            return QString("Classical Music");
        }
        case Song::Genre::Pop:
        {
            return QString("Pop Music");
        }
        case Song::Genre::Rock:
        {
            return QString("Rock Music");
        }
        case Song::Genre::Jazz:
        {
            return QString("Jazz");
        }
        case Song::Genre::Rap:
        {
            return QString("Rap Music");
        }
        case Song::Genre::Electronic:
        {
            return QString("Electronic Music");
        }
        case Song::Genre::Country:
        {
            return QString("Country Music");
        }
        case Song::Genre::Folk:
        {
            return QString("Folk Music");
        }
        case Song::Genre::Blues:
        {
            return QString("Blues");
        }
        case Song::Genre::Reggae:
        {
            return QString("Reggae");
        }
        case Song::Genre::Alternative:
        {
            return QString("Alternative Music");
        }
        case Song::Genre::Cassette:
        {
            return QString("Cassette");
        }
        case Song::Genre::Children:
        {
            return QString("Children Music");
        }
        case Song::Genre::Christian:
        {
            return QString("Christian Music");
        }
        case Song::Genre::Demon:
        {
            return QString("Demon");
        }
        case Song::Genre::EasyListening:
        {
            return QString("Easy Listening");
        }
        case Song::Genre::Enka:
        {
            return QString("Enka");
        }
        case Song::Genre::FrenchMusic:
        {
            return QString("French Music");
        }
        case Song::Genre::Goth:
        {
            return QString("Goth");
        }
        case Song::Genre::HipHop:
        {
            return QString("Hip Hop");
        }
        case Song::Genre::International:
        {
            return QString("International Music");
        }
        case Song::Genre::IrishMusic:
        {
            return QString("Irish Music");
        }
        case Song::Genre::Lullaby:
        {
            return QString("Lullaby");
        }
        case Song::Genre::Metal:
        {
            return QString("Metal Music");
        }
        case Song::Genre::Oud:
        {
            return QString("Oud Music");
        }
        case Song::Genre::ProgressiveRock:
        {
            return QString("Progressive Rock");
        }
        case Song::Genre::Punk:
        {
            return QString("Punk Music");
        }
        case Song::Genre::RnB:
        {
            return QString("RnB Music");
        }
        case Song::Genre::Rhythm:
        {
            return QString("Rhythm");
        }
        case Song::Genre::Ska:
        {
            return QString("Ska");
        }
        case Song::Genre::TexMex:
        {
            return QString("Tex-Mex");
        }
        case Song::Genre::UndergroundMusic:
        {
            return QString("Underground Music");
        }
        case Song::Genre::UsMusic:
        {
            return QString("US Music");
        }
        case Song::Genre::Vaporwave:
        {
            return QString("Vaporwave");
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
    return 34;
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
