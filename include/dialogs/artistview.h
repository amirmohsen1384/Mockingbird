#ifndef ARTISTVIEW_H
#define ARTISTVIEW_H

#include <QDialog>
#include "include/models/artistmodel.h"
#include "include/models/maindelegate.h"

namespace Ui {
class ArtistView;
}

class ArtistView : public QDialog
{
    Q_OBJECT
    void updateModel();
    void updateControl();

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
    std::unique_ptr<MainDelegate> delegate;
};

#endif // ARTISTVIEW_H
