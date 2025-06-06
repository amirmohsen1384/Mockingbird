#ifndef USERPANEL_H
#define USERPANEL_H

#include <QMainWindow>
#include "include/core/user.h"

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
    User user;
    std::unique_ptr<Ui::UserPanel> ui;
};

#endif // USERPANEL_H
