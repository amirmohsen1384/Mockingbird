#include "include/panels/userpanel.h"
#include "ui_userpanel.h"

UserPanel::UserPanel(const IDContainer &key, QWidget *parent) : QMainWindow(parent)
{
    ui = std::make_unique<Ui::UserPanel>();
    ui->setupUi(this);

    if(User::loadFromRecord(key).isNull())
    {
        throw std::runtime_error("User panel cannot be opened with an invalid key.");
    }

    mainModel.setMainKey(key);
}

UserPanel::~UserPanel() {}
