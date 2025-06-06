#include "include/panels/welcomepanel.h"
#include "include/panels/adminpanel.h"
#include "include/panels/userpanel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::unique_ptr<QMainWindow> window;

    auto mainPanel = std::make_unique<WelcomePanel>();
    mainPanel->open();

    QObject::connect(mainPanel.get(), &QDialog::finished, [&](int result)
    {
        if(result == QDialog::Accepted)
        {
            switch(mainPanel->getLoginMode())
            {
                case WelcomePanel::LoginAsAdmin:
                {
                    window = std::make_unique<AdminPanel>(mainPanel->getSignedkey());
                    window->show();
                    break;
                }
                case WelcomePanel::LoginAsUser:
                {
                    window = std::make_unique<UserPanel>(mainPanel->getSignedkey());
                    window->show();
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
    );

    return app.exec();
}
