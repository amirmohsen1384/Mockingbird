#ifndef ARTISTEDIT_H
#define ARTISTEDIT_H

#include <QDialog>
#include <QAbstractItemModel>
#include "include/models/artistmodel.h"
#include "include/models/maindelegate.h"

namespace Ui {
class ArtistEdit;
}

class ArtistEdit : public QDialog
{
    Q_OBJECT
private slots:
    void updateAvailability();
    void updateControl();
    void updateModel();

public:
    Q_DISABLE_COPY_MOVE(ArtistEdit)
    explicit ArtistEdit(QWidget *parent = nullptr);
    ~ArtistEdit();

    ArtistModel* sourceModel();
    ArtistModel* sourceModel() const;
    void setSourceModel(ArtistModel *value);

    virtual void accept() override;

public slots:
    void removeCover();
    void addPlaylist();
    void removePlaylist();
    void editPlaylist(const QModelIndex &index);

private:
    ArtistModel *model = nullptr;
    std::unique_ptr<Ui::ArtistEdit> ui;
    std::unique_ptr<MainDelegate> delegate;
};

#endif // ARTISTEDIT_H
