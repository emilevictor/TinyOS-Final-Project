#include <QtGui/QApplication>
#include "remotewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RemoteWidget *rw = new RemoteWidget();
    rw->setWindowTitle("CSSE4003 - robotic mote driver");
    rw->show();
    //MainWindow w;
    //w.show();

    return a.exec();
}
