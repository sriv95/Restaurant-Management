#include <QtWidgets/QApplication>
#include "ui/jsoncheck.h"
#include "header/json.h"
#include <QFile>

int main(int argc, char *argv[])
{    QApplication a(argc, argv);

    jsoncheck *jsonCheck = new jsoncheck();
    jsonCheck->setWindowTitle("File Configuration");
    jsonCheck->show();


    return a.exec();
}
