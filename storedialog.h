#ifndef STOREDIALOG_H
#define STOREDIALOG_H

#include <QDialog>
#include "include/models/songmodel.h"

namespace Ui {
class StoreDialog;
}

class StoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StoreDialog(QWidget *parent = nullptr);
    ~StoreDialog();

private:
    SongModel model;
    Ui::StoreDialog *ui;
};

#endif // STOREDIALOG_H
