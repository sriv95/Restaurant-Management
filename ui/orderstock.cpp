#include "orderstock.h"
#include "ui_orderstock.h"

json Data;

OrderStock::OrderStock(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderStock)
{
    ui->setupUi(this);

    getAllData(Data);

    //Add items to comboBox
    for(auto stock : Data["Stocks"]){
        ui->SelectStock->addItem(QString::fromStdString(stock[0]));
    }
}

OrderStock::~OrderStock()
{
    setData(Data["Statement"],"Statement");
    delete ui;
}

