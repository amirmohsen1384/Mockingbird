#include "include/panels/welcomepanel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomePanel w;
    w.show();
    return a.exec();
}
