#ifndef ARTISTVIEW_H
#define ARTISTVIEW_H

#include <QDialog>
#include "include/models/artistmodel.h"

namespace Ui {
class ArtistView;
}

class ArtistView : public QDialog
{
    Q_OBJECT
    void updateModel();

public slots:
    void playPlaylist(const QModelIndex &index);
    void goToPlaylistPage();
    void goToInfoPage();

public:
    explicit ArtistView(QWidget *parent = nullptr);
    ~ArtistView();

    ArtistModel* sourceModel() const;
    void setSourceModel(ArtistModel *value);

private:
    ArtistModel *mainModel = nullptr;
    std::unique_ptr<Ui::ArtistView> ui;
};

#endif // ARTISTVIEW_H
