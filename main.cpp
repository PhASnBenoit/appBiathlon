//#include "mainui.h"
#include "capp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CApp app;
    //MainUI w;
    //w.show();
    return a.exec();
}
