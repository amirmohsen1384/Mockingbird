#include "include/models/delegates/playlistdelegate.h"
#include <QPainter>

PlaylistDelegate::PlaylistDelegate(QObject *parent) : MainDelegate{parent}
{}

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
        painter->fillRect(trackRect, QGradient::PoliteRumors);
    }
}
