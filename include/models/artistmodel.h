#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include "include/models/playlistmodel.h"
#include "include/core/artist.h"
#include <QAbstractListModel>

using PlaylistModelContainer = std::shared_ptr<PlaylistModel>;

class ArtistModel : public QAbstractListModel
{
    Q_OBJECT
private slots:
    void updateModel();

public:
    explicit ArtistModel(const IDContainer &key = 0, QObject *parent = nullptr);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    
    virtual void insertPlaylist(PlaylistModelContainer value);

    void saveToRecord() const;

    IDs getKeys() const;
    QModelIndex fromKey(const IDContainer &value) const;
    const QList<PlaylistModelContainer>& getContainer() const;

public:
    IDContainer mainKey() const;
    void setMainKey(const IDContainer &value);

private:
    Artist metaData;
    IDContainer key = 0;
    QList<PlaylistModelContainer> container;
};

#endif // ARTISTMODEL_H
