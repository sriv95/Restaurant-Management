#include "reserve.h"
#include "ui_reserve.h"

reserve::reserve(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::reserve)
{
    ui->setupUi(this);
}

reserve::~reserve()
{
    delete ui;
}

void reserve::on_buttonBox_accepted()
{

}


void reserve::on_TableNum_cursorPositionChanged(int arg1, int arg2)
{

}

