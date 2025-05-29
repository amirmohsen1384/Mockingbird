#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "include/core/playlist.h"
#include <QAbstractItemModel>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    static const QVector<int> roles;

public:
    explicit PlaylistModel(const IDContainer &value = 0, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    IDContainer getID() const;
    void setID(const IDContainer &value);

    int getCurrentTrack() const;
    void setCurrentTrack(int index);

private:
    int current = -1;
    IDContainer mainId = 0;
};

#endif // PLAYLISTMODEL_H
