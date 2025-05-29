#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QMainWindow>

namespace Ui
{
    class WelcomePage;
}

class WelcomePage : public QMainWindow
{
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(WelcomePage)
    explicit WelcomePage(QWidget *parent = nullptr);
    ~WelcomePage();

public slots:
    void goToSigninPage();

private:
    std::unique_ptr<Ui::WelcomePage> ui;
};

#endif // WELCOMEPAGE_H
