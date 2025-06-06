#include "storedialog.h"
#include "ui_storedialog.h"

StoreDialog::StoreDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StoreDialog)
{
    ui->setupUi(this);
}

StoreDialog::~StoreDialog()
{
    delete ui;
}
