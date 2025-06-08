#ifndef MAINDELEGATE_H
#define MAINDELEGATE_H

#include <QStyledItemDelegate>
#include "include/models/playlistmodel.h"

class MainDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    static int thickness;
    static QFont nameFont;
    static QMargins margins;
    static QTextOption textOptions;
    static QFontMetrics nameMetrics;

public:
    MainDelegate(QObject *parent = nullptr);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public slots:
    void setPrimary(const QColor &value);
    void setSecondary(const QColor &value);

signals:
    void primaryChanged(const QColor &value);
    void secondaryChanged(const QColor &value);

private:
    QLinearGradient gradient;
};
#endif // MAINDELEGATE_H
