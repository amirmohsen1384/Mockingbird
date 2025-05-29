#include "include/pages/welcomepage.h"
#include "ui_welcomepage.h"
#include <QTimer>

WelcomePage::WelcomePage(QWidget *parent) : QMainWindow(parent)
{
    ui = std::make_unique<Ui::WelcomePage>();

    ui->setupUi(this);
    ui->continueButton->setVisible(false);

    QTimer::singleShot(2000, [&]()
    {
        ui->continueButton->setVisible(true);
    }
    );
}

WelcomePage::~WelcomePage()
{}

void WelcomePage::goToSigninPage()
{
    ui->pageContainer->setCurrentWidget(ui->signinPage);
}
