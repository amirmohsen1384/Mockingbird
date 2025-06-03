#include "include/panels/welcomepanel.h"
#include "ui_welcomepanel.h"
#include <QTimer>

WelcomePanel::WelcomePanel(QWidget *parent) : QMainWindow(parent)
{
    ui = std::make_unique<Ui::WelcomePanel>();

    ui->setupUi(this);
    ui->continueButton->setVisible(false);

    QTimer::singleShot(2000, [&]()
    {
        ui->continueButton->setVisible(true);
    }
    );
}

WelcomePanel::~WelcomePanel()
{}

void WelcomePanel::goToSigninPage()
{
    ui->pageContainer->setCurrentWidget(ui->signinPage);
}
