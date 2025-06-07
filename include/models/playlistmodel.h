#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "include/core/playlist.h"
#include "include/core/song.h"
#include <QAbstractItemModel>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    static const QVector<int> roles;

private slots:
    void updateModel();

public:
    Q_DISABLE_COPY_MOVE(PlaylistModel)
    explicit PlaylistModel(const IDContainer &value = 0, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;
    virtual QVariant headerData(int section = 0, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;

    void insertSong(const IDContainer &value, const Song &data);
    void removeSong(const IDContainer &value);
    void removeSong(int row);

    QModelIndex fromKey(const IDContainer &value) const;

    IDContainer getID() const;
    void setID(const IDContainer &value);

    int getCurrentTrack() const;
    void setCurrentTrack(int index);

    void saveToRecord() const;

    IDs getKeys() const;
    SongList getSongs() const;
    const QList<SongInfo> &getStore() const;
    
    std::shared_ptr<PlaylistModel> clone() const;

private:
    int current = -1;
    Playlist metadata;
    QList<SongInfo> store;
    IDContainer mainId = 0;
};

#endif // PLAYLISTMODEL_H
