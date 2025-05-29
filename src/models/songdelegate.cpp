#include "include/models/songdelegate.h"
#include <QPainter>

QRect SongDelegate::coverRegion(const QModelIndex &index) const
{
    QPixmap cover = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));
    QRect rectangle = cover.rect().marginsRemoved(margins);
    rectangle.setSize(QSize(72, 72));
    return rectangle;
}

void SongDelegate::paintCover(QPainter *painter, const QModelIndex &index) const
{
    const QPixmap cover = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));
    const QRect region = coverRegion(index);

    painter->save();
    painter->setClipRegion(QRegion(region, QRegion::Ellipse));
    painter->drawPixmap(region, cover);
    painter->restore();

    QLinearGradient gradient;
    gradient.setCoordinateMode(QLinearGradient::ObjectMode);
    gradient.setStops({{0.3, Qt::blue}, {0.8, Qt::darkGreen}});
    gradient.setFinalStop(QPointF(0.5, 1));
    gradient.setStart(QPointF(0.5, 0));

    const QPen initial = painter->pen();
    painter->setPen(QPen(gradient, 4));

    painter->drawEllipse(region);
    painter->setPen(initial);
}

QRect SongDelegate::nameRegion(const QModelIndex &index) const
{
    const QString name = index.data().toString();
    const QString artist = index.data(Playlist::ArtistRole).toString();

    const QRect nameRect = QFontMetrics(nameFont).boundingRect(name);
    const QRect artistRect = QFontMetrics(artistFont).boundingRect(artist);

    int width = std::max(nameRect.width(), artistRect.width()) + 80;
    return QRect(0, 0, width, nameRect.height());
}
void SongDelegate::paintName(QPainter *painter, const QModelIndex &index) const
{
    const QString text = index.data().toString();
    const QFont initial = painter->font();
    QRect region = nameRegion(index);

    painter->save();
    region.translate(20, 20);

    painter->setFont(nameFont);
    painter->drawText(region, text);
    painter->setFont(initial);

    painter->restore();
}

SongDelegate::SongDelegate(QObject *parent) : QStyledItemDelegate{parent}
{
    genreFont.setItalic(true);
}

QSize SongDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.widget != nullptr ? option.widget->width() : 1000 , 72 + margins.top() * 2 + margins.bottom());
}

void SongDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(Playlist::PlayingRole).toBool())
    {
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QColor>());
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(option.rect.topLeft() + QPoint(margins.top(), margins.left()));

    paintCover(painter, index);
    painter->translate(coverRegion(index).width(), 0);

    paintName(painter, index);

    painter->restore();
}
