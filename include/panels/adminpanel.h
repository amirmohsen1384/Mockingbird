#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QMainWindow>
#include "include/core/admin.h"
#include "include/models/artistlistmodel.h"

namespace Ui
{
    class AdminPanel;
}

class AdminPanel : public QMainWindow
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(AdminPanel)
    explicit AdminPanel(const IDContainer &key, QWidget *parent = nullptr);

    IDContainer getMainKey() const;
    ~AdminPanel();

private slots:
    void updateMetaData();
    void rejectChanging();
    void acceptChanging();
    void togglePasswordShow(bool toggle);

public slots:
    void setMainKey(const IDContainer &value);
    void goToInfoChangingMode();
    void goToNormalMode();

    void addArtist();
    void viewArtist(const QModelIndex &index);
    void removeArtist();
    void editArtist();

signals:
    void mainKeyChanged(const IDContainer &value);

private:
    IDContainer mainKey;
    ArtistListModel mainModel;
    std::unique_ptr<Ui::AdminPanel> ui;
};

#endif // ADMINPANEL_H
