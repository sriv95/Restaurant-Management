#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "header/json.h"

int main(int argc, char *argv[])
{    QApplication a(argc, argv);

    RestuarantManagement w;
    w.show();

    return a.exec();
}
