#ifndef STOREDIALOG_H
#define STOREDIALOG_H

#include <QDialog>
#include "include/models/songmodel.h"
#include "include/models/maindelegate.h"
#include "include/models/songproxymodel.h"

namespace Ui {
class StoreDialog;
}

class StoreDialog : public QDialog
{
    Q_OBJECT
private:
    void updateCriteria();

public:
    explicit StoreDialog(QWidget *parent = nullptr);
    ~StoreDialog();

public slots:
    void playSong();
    void playSong(const QModelIndex &index);

    void setFindPanelVisible(bool toggle);
    void setFilterPanelVisible(bool toggle);
    void setArrangePanelVisible(bool toggle);

private slots:
    void updateFilter();
    void updateFindCriteria();
    void updateArrangeCriteria();

private:
    SongModel sourcModel;
    SongProxyModel model;
    std::unique_ptr<Ui::StoreDialog> ui;
    std::unique_ptr<MainDelegate> delegate;
};

#endif // STOREDIALOG_H
