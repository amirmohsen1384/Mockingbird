#ifndef USERPANEL_H
#define USERPANEL_H

#include <QMainWindow>
#include "include/models/usermodel.h"
#include "include/models/artistlistmodel.h"
#include "include/models/delegates/maindelegate.h"

namespace Ui {
class UserPanel;
}

class UserPanel : public QMainWindow
{
    Q_OBJECT
private slots:
    void initializeEditor();
    void checkAvailability();

public:
    explicit UserPanel(const IDContainer &key, QWidget *parent = nullptr);
    ~UserPanel();

public slots:
    void addPlaylist();
    void editPlaylist();
    void removePlaylist();

    void goToExplorePage();
    void goToProfilePage();

    void viewArtist(const QModelIndex &index);
    void viewPlaylist(const QModelIndex &index);

    void goToInfoEditor();
    void toggleShowPassword(bool enabled);

    void rejectEditor();
    void acceptEditor();

private:
    UserModel mainModel;
    ArtistListModel artistModel;
    std::unique_ptr<Ui::UserPanel> ui;
    std::unique_ptr<MainDelegate> delegate;
};

#endif // USERPANEL_H
