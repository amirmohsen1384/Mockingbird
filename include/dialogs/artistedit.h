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

public:
    Q_DISABLE_COPY_MOVE(ArtistEdit)
    explicit ArtistEdit(const Artist &value, QWidget *parent = nullptr);
    explicit ArtistEdit(QWidget *parent = nullptr);
    ~ArtistEdit();

    Artist artist() const;
    void setArtist(const Artist &value);

public slots:
    void addPlaylist();
    void removePlaylist();
    void editPlaylist(const QModelIndex &index);

    virtual void accept() override;

private:
    std::unique_ptr<Ui::ArtistEdit> ui;
    std::unique_ptr<ArtistModel> container;
};

#endif // ARTISTEDIT_H
