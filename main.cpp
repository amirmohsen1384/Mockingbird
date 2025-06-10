#include "include/panels/welcomepanel.h"
#include "include/panels/adminpanel.h"
#include "include/panels/userpanel.h"
#include <QApplication>

void initializeAdmin()
{
    Admin info;
    info.setFirstName("Mahsa");
    info.setLastName("Ghandahari");
    info.setUserName("mahsa1234");
    info.setPassword("Mahsa@1384");
    info.saveToRecord(498213342);

    Admin another;
    another.setFirstName("Amir Mohsen");
    another.setLastName("Ghasemi");
    another.setUserName("amir1234");
    another.setPassword("amir@1384");
    info.saveToRecord(683192219);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    initializeAdmin();

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
