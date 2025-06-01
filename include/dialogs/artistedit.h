#ifndef ARTISTEDIT_H
#define ARTISTEDIT_H

#include <QDialog>
#include <QAbstractItemModel>
#include "include/models/artistmodel.h"

namespace Ui {
class ArtistEdit;
}

class ArtistEdit : public QDialog
{
    Q_OBJECT
private slots:
    void updateMetaData();
    void updateControl();
    void updateModel();

public:
    Q_DISABLE_COPY_MOVE(ArtistEdit)
    explicit ArtistEdit(QWidget *parent = nullptr);
    explicit ArtistEdit(const Artist &value, QWidget *parent = nullptr);
    ~ArtistEdit();

    ArtistModel *sourceModel();
    ArtistModel *sourceModel() const;
    void setSourceModel(ArtistModel *model);

public slots:
    void addPlaylist();
    void removePlaylist();
    void editPlaylist(const QModelIndex &index);

private:
    ArtistModel *model = nullptr;
    std::unique_ptr<Ui::ArtistEdit> ui;
};

#endif // ARTISTEDIT_H
