#ifndef USERPANEL_H
#define USERPANEL_H

#include <QMainWindow>
#include "include/models/usermodel.h"
#include "include/models/maindelegate.h"

namespace Ui {
class UserPanel;
}

class UserPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserPanel(const IDContainer &key, QWidget *parent = nullptr);
    ~UserPanel();

private:
    UserModel mainModel;
    std::unique_ptr<Ui::UserPanel> ui;
    std::unique_ptr<MainDelegate> delegate;
};

#endif // USERPANEL_H
