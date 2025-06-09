#ifndef MAINDELEGATE_H
#define MAINDELEGATE_H

#include <QStyledItemDelegate>
#include "include/models/playlistmodel.h"

class MainDelegate : public QStyledItemDelegate
{
    Q_OBJECT

protected:
    static int thickness;
    static QFont nameFont;
    static QMargins margins;
    static QFontMetrics nameMetrics;

public:
    MainDelegate(QObject *parent = nullptr);

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public slots:
    void setPrimary(const QColor &value);
    void setSecondary(const QColor &value);

protected:
    QLinearGradient gradient;
};

#endif // MAINDELEGATE_H
