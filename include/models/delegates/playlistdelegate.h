#ifndef PLAYLISTDELEGATE_H
#define PLAYLISTDELEGATE_H

#include "maindelegate.h"

class PlaylistDelegate : public MainDelegate
{
    Q_OBJECT
public:
    explicit PlaylistDelegate(QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // PLAYLISTDELEGATE_H
