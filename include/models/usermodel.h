#ifndef USERMODEL_H
#define USERMODEL_H

#include <QAbstractListModel>
#include "include/core/user.h"
#include "include/models/playlistmodel.h"

class UserModel : public QAbstractListModel
{
    Q_OBJECT
private slots:
    void updateModel();

public:
    explicit UserModel(const IDContainer &key = 0, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void insert(std::shared_ptr<PlaylistModel> value);
    bool remove(const IDContainer &key);

    void setMainKey(const IDContainer &key);
    IDContainer getMainKey() const;

    IDs getKeys() const;
    QModelIndex fromKey(const IDContainer &key);


private:
    User metaData;
    IDContainer mainKey = 0;
    QList<std::shared_ptr<PlaylistModel>> container;
};

#endif // USERMODEL_H
