#ifndef USERPANEL_H
#define USERPANEL_H

#include <QMainWindow>

namespace Ui {
class UserPanel;
}

class UserPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserPanel(QWidget *parent = nullptr);
    ~UserPanel();

private:
    Ui::UserPanel *ui;
};

#endif // USERPANEL_H
