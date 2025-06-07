#ifndef SONGMODEL_H
#define SONGMODEL_H

#include "include/core/song.h"
#include <QAbstractListModel>

class SongModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SongModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    SongInfoList container;
};

#endif // SONGMODEL_H
