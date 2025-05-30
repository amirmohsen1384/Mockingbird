#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "include/core/playlist.h"
#include <QAbstractItemModel>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    static const QVector<int> roles;

public:
    Q_DISABLE_COPY_MOVE(PlaylistModel)
    explicit PlaylistModel(const IDContainer &value = 0, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void appendID(const IDContainer &value);
    void removeID(const IDContainer &value);
    void removeID(int row);

    IDContainer getID() const;
    void setID(const IDContainer &value);

    int getCurrentTrack() const;
    void setCurrentTrack(int index);

private:
    int current = -1;
    IDContainer mainId = 0;
};

#endif // PLAYLISTMODEL_H
