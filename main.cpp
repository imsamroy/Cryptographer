#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Cryptographer.svg"));
    MainWindow w;
    w.show();
    return a.exec();    
}
