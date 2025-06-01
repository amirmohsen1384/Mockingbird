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
    explicit ArtistView(const IDContainer &key, QWidget *parent = nullptr);
    explicit ArtistView(QWidget *parent = nullptr);
    ~ArtistView();

    IDContainer mainKey() const;
    void setMainKey(const IDContainer &value);

private:
    std::unique_ptr<Ui::ArtistView> ui;
    std::unique_ptr<ArtistModel> mainModel;
};

#endif // ARTISTVIEW_H
