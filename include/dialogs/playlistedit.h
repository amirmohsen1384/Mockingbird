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

public:
    Q_DISABLE_COPY_MOVE(PlaylistEdit)
    explicit PlaylistEdit(QWidget *parent = nullptr);
    ~PlaylistEdit();

    PlaylistModel* getSourceModel();
    const PlaylistModel* getSourceModel() const;

    void setSourceModel(PlaylistModel *model);

public slots:
    void addSong();
    void removeSong();
    void editSong(const QModelIndex &index);

    virtual void accept() override;

private:
    PlaylistModel *sourceModel = nullptr;
    std::unique_ptr<Ui::PlaylistEdit> ui;
};

#endif // PLAYLISTEDIT_H
