#include "include/models/delegates/playlistdelegate.h"
#include <QPainter>

PlaylistDelegate::PlaylistDelegate(QObject *parent) : MainDelegate{parent}
{
    playing.setStops({{0.3, Qt::magenta}, {0.5, Qt::green}, {0.7, Qt::darkYellow}});
    playing.setCoordinateMode(QGradient::ObjectMode);
    playing.setFinalStop(1.0, 1.0);
    playing.setStart(0.0, 0.0);
}

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    MainDelegate::paint(painter, option, index);

    auto trackRect = QRect{};
    trackRect.setTopRight(option.rect.topRight());
    trackRect.setX(option.rect.topRight().x() - 30);
    trackRect.setHeight(option.rect.height());

    bool value = index.data(Playlist::PlayingRole).toBool();
    if(value)
    {
        qDebug() << index.data(Playlist::NameRole).toString();
        painter->fillRect(trackRect, Qt::yellow);
    }
    qDebug() << playing << value;
}
