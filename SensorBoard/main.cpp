#include "mainwindow.h"
#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login(nullptr, true);
    login.show();
//    MainWindow w;
//    w.show();
    return a.exec();
}
