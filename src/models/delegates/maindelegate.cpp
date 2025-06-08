#include <QPainter>
#include "include/models/maindelegate.h"

int MainDelegate::thickness = 4;
QMargins MainDelegate::margins = QMargins(4, 4, 4, 4);
QFont MainDelegate::nameFont = QFont("Segoe UI Light", 14);
QFontMetrics MainDelegate::nameMetrics = QFontMetrics(MainDelegate::nameFont);
QTextOption MainDelegate::textOptions = QTextOption(Qt::AlignLeft | Qt::AlignVCenter);

MainDelegate::MainDelegate(QObject *parent) : QStyledItemDelegate{parent}
{
    setSecondary(Qt::darkBlue);
    setPrimary(Qt::yellow);
    gradient.setCoordinateMode(QLinearGradient::ObjectMode);
    gradient.setFinalStop(QPointF(0.5, 1));
    gradient.setStart(QPointF(0.5, 0));
}

QSize MainDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    QSize size = index.data(Qt::DecorationRole).value<QPixmap>().size();
    return QSize(option.widget != nullptr ? option.widget->width() : 1000, size.grownBy(margins).height());
}

void MainDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto hints = painter->renderHints();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    if(!index.data(Qt::BackgroundRole).isNull())
    {
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QColor>());
    }
    else if(option.state.testFlag(QStyle::State_Selected))
    {
        painter->fillRect(option.rect, QColor(180, 255, 240));
    }


    painter->save();
    painter->translate(option.rect.topLeft() + QPoint(margins.left(), margins.top()));

    const QPixmap cover = index.data(Qt::DecorationRole).value<QPixmap>();
    {
        painter->save();
        painter->setClipRegion(QRegion(cover.rect(), QRegion::Ellipse));
        painter->drawPixmap(cover.rect(), cover);
        painter->restore();

        const QPen initial = painter->pen();
        painter->setPen(QPen(gradient, thickness));
        painter->drawEllipse(cover.rect());
        painter->setPen(initial);
    }

    painter->translate(cover.width() + margins.right() + margins.left(), 0);

    QRect nameRegion;
    nameRegion.setWidth(option.rect.width() - cover.width() - margins.right() - 2 * margins.left());
    nameRegion.setHeight(option.rect.height() - margins.top() - margins.bottom());

    QFont font = painter->font();
    painter->setFont(nameFont);
    painter->drawText(nameRegion, index.data(Qt::DisplayRole).toString(), textOptions);

    if(!index.data(Qt::BackgroundRole).isNull())
    {
        painter->setFont(QFont("Segoe UI", 9, QFont::Bold));
        painter->drawText(nameRegion, "Playing...", QTextOption(Qt::AlignRight | Qt::AlignBottom));
    }

    painter->setRenderHints(hints);
    painter->setFont(font);

    painter->restore();
}

void MainDelegate::setPrimary(const QColor &value)
{
    gradient.setColorAt(0.8, value);
    emit primaryChanged(value);
}

void MainDelegate::setSecondary(const QColor &value)
{
    gradient.setColorAt(0.5, value);
    emit secondaryChanged(value);
}
