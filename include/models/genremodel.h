#ifndef GENREMODEL_H
#define GENREMODEL_H

#include "include/core/song.h"
#include <QAbstractListModel>

class GenreModel : public QAbstractListModel
{
    Q_OBJECT
public:
    static QPixmap toPixmap(const Song::Genre &genre);
    static QString toString(const Song::Genre &genre);
    static QPair<QString, QPixmap> toMetaData(const Song::Genre &genre);

public:
    explicit GenreModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // GENREMODEL_H
