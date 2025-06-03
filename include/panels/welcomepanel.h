#ifndef WELCOMEPANEL_H
#define WELCOMEPANEL_H

#include <QMainWindow>

namespace Ui
{
    class WelcomePanel;
}

class WelcomePanel : public QMainWindow
{
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(WelcomePanel)
    explicit WelcomePanel(QWidget *parent = nullptr);
    ~WelcomePanel();

public slots:
    void goToSigninPage();

private:
    std::unique_ptr<Ui::WelcomePanel> ui;
};

#endif // WELCOMEPANEL_H
