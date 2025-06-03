#ifndef ARTISTLISTMODEL_H
#define ARTISTLISTMODEL_H

#include <QAbstractListModel>
#include "include/core/song.h"
#include "include/core/artist.h"
#include "include/core/playlist.h"
#include "include/models/artistmodel.h"

class ArtistListModel : public QAbstractListModel
{
    Q_OBJECT
    struct DataContainer
    {
        QString name;
        QImage photo;
        IDContainer key;
    };
    using DataContainerList = QList<DataContainer>;

public:
    explicit ArtistListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QModelIndex fromKey(const IDContainer &key) const;

    void insertArtist(const ArtistModel &model);
    void modifyArtist(const ArtistModel &model);
    bool removeArtist(const IDContainer &key);

private:
    DataContainerList container;
};

#endif // ARTISTLISTMODEL_H
