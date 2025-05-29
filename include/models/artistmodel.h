#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include <QAbstractListModel>
#include "include/core/artist.h"

class ArtistModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(ArtistModel)
    ArtistModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}
    ArtistModel(const IDContainer &value, QObject *parent = nullptr);
    ~ArtistModel() {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    const IDContainer& mainKey() const;
    void setMainKey(const IDContainer &value);

private:
    IDContainer key;
};

#endif // ARTISTMODEL_H
