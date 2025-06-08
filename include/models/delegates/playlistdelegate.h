#ifndef PLAYLISTDELEGATE_H
#define PLAYLISTDELEGATE_H

#include <maindelegate.h>

class PlaylistDelegate : public MainDelegate
{
    Q_OBJECT
public:
    explicit PlaylistDelegate(QObject *parent = nullptr);
};

#endif // PLAYLISTDELEGATE_H
