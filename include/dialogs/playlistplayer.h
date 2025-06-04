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
    void toggleSingleMode(bool enabled);
    void playSong(const QModelIndex &index);

public:
    explicit PlaylistPlayer(QWidget *parent = nullptr);
    ~PlaylistPlayer();

    void setSourceModel(PlaylistModel *value);
    PlaylistModel* getSourceModel() const;
    PlaylistModel* getSourceModel();

private:
    PlaylistModel *model = nullptr;
    std::unique_ptr<SongDelegate> delegate;
    std::unique_ptr<Ui::PlaylistPlayer> ui;
};

#endif // PLAYLISTPLAYER_H
