#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QMainWindow>
#include "include/core/admin.h"

namespace Ui {
class AdminPage;
}

class AdminPage : public QMainWindow
{
    Q_OBJECT
public:
    explicit AdminPage(const Admin &admin, QWidget *parent = nullptr);
    explicit AdminPage(QWidget *parent = nullptr);
    ~AdminPage();

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
    std::unique_ptr<Ui::AdminPage> ui;
};

#endif // ADMINPAGE_H
