#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class WelcomePage;
}
QT_END_NAMESPACE

class WelcomePage : public QMainWindow
{
    Q_OBJECT

public:
    WelcomePage(QWidget *parent = nullptr);
    ~WelcomePage();

private:
    Ui::WelcomePage *ui;
};
#endif // WELCOMEPAGE_H
