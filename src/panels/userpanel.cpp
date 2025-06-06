#include "include/panels/userpanel.h"
#include "ui_userpanel.h"

UserPanel::UserPanel(const IDContainer &key, QWidget *parent) : QMainWindow(parent)
{
    ui = std::make_unique<Ui::UserPanel>();
    ui->setupUi(this);

    delegate = std::make_unique<MainDelegate>();

    delegate->setPrimary(Qt::magenta);
    delegate->setSecondary(Qt::darkYellow);
    ui->playlistView->setModel(&mainModel);
    ui->playlistView->setItemDelegate(delegate.get());

    if(User::loadFromRecord(key).isNull())
    {
        throw std::runtime_error("User panel cannot be opened with an invalid key.");
    }

    mainModel.setMainKey(key);

    const QString name = mainModel.headerData(0, Qt::Horizontal, Qt::DisplayRole).toString();
    ui->accountNameLabel->setText(QString("%1, How's it going?").arg(name));
    setWindowTitle(QString("%1 - User Panel").arg(name));
}

UserPanel::~UserPanel() {}
