#ifndef STOREDIALOG_H
#define STOREDIALOG_H

#include <QDialog>
#include "include/models/songmodel.h"
#include "include/models/songproxymodel.h"
#include "include/models/delegates/maindelegate.h"

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

    SongInfo getSelectedSong() const;

public slots:
    void playSong();
    void playSong(const QModelIndex &index);

    void setFindPanelVisible(bool toggle);
    void setFilterPanelVisible(bool toggle);
    void setArrangePanelVisible(bool toggle);

    virtual void accept() override;

private slots:
    void updateFilter();
    void updateFindCriteria();
    void updateArrangeCriteria();

private:
    SongModel sourceModel;
    SongProxyModel model;
    std::unique_ptr<Ui::StoreDialog> ui;
    std::unique_ptr<MainDelegate> delegate;
};

#endif // STOREDIALOG_H
