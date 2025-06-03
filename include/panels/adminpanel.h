#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QMainWindow>
#include "include/core/admin.h"

namespace Ui
{
    class AdminPanel;
}

class AdminPanel : public QMainWindow
{
    Q_OBJECT
public:
    explicit AdminPanel(const Admin &admin, QWidget *parent = nullptr);
    explicit AdminPanel(QWidget *parent = nullptr);
    ~AdminPanel();

    Admin adminstrator() const;

private slots:
    void updateMetaData();
    void rejectChanging();
    void acceptChanging();
    void togglePasswordShow(bool toggle);

public slots:
    void setAdminstrator(const Admin &value);
    void goToInfoChangingMode();

signals:
    void adminstratorChanged(const Admin &value);

private:
    Admin admin;
    std::unique_ptr<Ui::AdminPanel> ui;
};

#endif // ADMINPANEL_H
