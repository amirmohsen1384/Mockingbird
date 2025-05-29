#ifndef SONGDELEGATE_H
#define SONGDELEGATE_H

#include <QStyledItemDelegate>
#include "include/models/playlistmodel.h"

class SongDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    QMargins margins = QMargins(4, 4, 4, 4);
    QFont yearFont = QFont("Segoe UI", 10);
    QFont genreFont = QFont("Segoe UI", 14);
    QFont artistFont = QFont("Segoe UI", 12, QFont::Bold);
    QFont nameFont = QFont("Segoe UI Light", 14);

private:
    QRect nameRegion(const QModelIndex &index) const;
    void paintName(QPainter *painter, const QModelIndex &index) const;

    QRect coverRegion(const QModelIndex &index) const;
    void paintCover(QPainter *painter, const QModelIndex &index) const;

    QRect genreRegion(const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    SongDelegate(QObject *parent = nullptr);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
#endif // SONGDELEGATE_H
