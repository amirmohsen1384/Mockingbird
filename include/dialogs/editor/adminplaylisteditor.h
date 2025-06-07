#ifndef ADMINPLAYLISTEDITOR_H
#define ADMINPLAYLISTEDITOR_H

#include "abstractplaylisteditor.h"

class AdminPlaylistEditor : public AbstractPlaylistEditor
{
public:
    Q_DISABLE_COPY_MOVE(AdminPlaylistEditor)
    AdminPlaylistEditor(QWidget *parent = nullptr);

public slots:
    virtual void addSong() override;
    virtual void editSong(const QModelIndex &index);
};

#endif // ADMINPLAYLISTEDITOR_H
