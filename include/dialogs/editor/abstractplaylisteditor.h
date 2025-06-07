#ifndef ABSTRACTPLAYLISTEDITOR_H
#define ABSTRACTPLAYLISTEDITOR_H

#include <QDialog>
#include "include/models/playlistmodel.h"
#include "include/models/maindelegate.h"

namespace Ui
{
    class AbstractPlaylistEditor;
}

class AbstractPlaylistEditor : public QDialog
{
    Q_OBJECT
private slots:
    void updateModel();
    void updateControl();

public:
    Q_DISABLE_COPY_MOVE(AbstractPlaylistEditor)
    explicit AbstractPlaylistEditor(QWidget *parent = nullptr);

    ~AbstractPlaylistEditor();

    PlaylistModel* getSourceModel();
    virtual void setSourceModel(PlaylistModel *model);

public slots:
    virtual void removeSong();
    virtual void addSong() = 0;
    virtual void handleSong(const QModelIndex &index);

    virtual void accept() override;

protected:
    PlaylistModel *sourceModel = nullptr;
    std::unique_ptr<MainDelegate> delegate;
    std::unique_ptr<Ui::AbstractPlaylistEditor> ui;
};

#endif // ABSTRACTPLAYLISTEDITOR_H
