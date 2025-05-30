#ifndef PLAYLISTEDIT_H
#define PLAYLISTEDIT_H

#include <QDialog>
#include "include/models/playlistmodel.h"

namespace Ui
{
    class PlaylistEdit;
}

class PlaylistEdit : public QDialog
{
    Q_OBJECT
private slots:
    void updateModel();
    void updateControl();
    void updatePlaylistName();

public:
    Q_DISABLE_COPY_MOVE(PlaylistEdit)
    explicit PlaylistEdit(const IDContainer &value, QWidget *parent = nullptr);
    explicit PlaylistEdit(QWidget *parent = nullptr);
    ~PlaylistEdit();

    IDContainer mainId() const;
    void setMainId(const IDContainer &value);

public slots:
    void addSong();
    void playSong();
    void removeSong();
    void editSong(const QModelIndex &index);

private:
    std::unique_ptr<Ui::PlaylistEdit> ui;
    std::unique_ptr<PlaylistModel> sourceModel;
};

#endif // PLAYLISTEDIT_H
