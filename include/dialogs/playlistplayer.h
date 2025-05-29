#ifndef PLAYLISTPLAYER_H
#define PLAYLISTPLAYER_H

#include <QDialog>
#include "include/components/player.h"
#include "include/models/songdelegate.h"

namespace Ui { class PlaylistPlayer; }

class PlaylistPlayer : public QDialog
{
    Q_OBJECT
private slots:
    void updateModel();
    void updateCurrentTrack();
    void playSong(const QModelIndex &index);

public:
    explicit PlaylistPlayer(const IDContainer &id, QWidget *parent = nullptr);
    explicit PlaylistPlayer(QWidget *parent = nullptr);
    ~PlaylistPlayer();

    IDContainer getID() const;
    void setID(const IDContainer &id);

private:
    std::unique_ptr<PlaylistModel> model;
    std::unique_ptr<SongDelegate> delegate;
    std::unique_ptr<Ui::PlaylistPlayer> ui;
};

#endif // PLAYLISTPLAYER_H
