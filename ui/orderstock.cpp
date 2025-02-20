#include "orderstock.h"
#include "ui_orderstock.h"

OrderStock::OrderStock(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderStock)
{
    ui->setupUi(this);
}

OrderStock::~OrderStock()
{
    delete ui;
}
