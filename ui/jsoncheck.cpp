#include "jsoncheck.h"
#include "ui_jsoncheck.h"

jsoncheck::jsoncheck(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::jsoncheck)
{
    ui->setupUi(this);
}

jsoncheck::~jsoncheck()
{
    delete ui;
}
