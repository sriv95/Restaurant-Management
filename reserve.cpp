#include "reserve.h"
#include "ui_reserve.h"
#include "header/json.h"
#include <QMessageBox>

extern void setData(json &Data, std::string key);
extern void getAllData(json &Data);

reserve::reserve(QWidget *parent)
    : QDialog(parent), ui(new Ui::reserve)
{
    ui->setupUi(this);
}

reserve::~reserve()
{
    delete ui;
}

void reserve::on_buttonBox_accepted()
{
    bool ok;
    int tableNum = ui->TableNum->text().trimmed().toInt(&ok);
    if (!ok || tableNum <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid table number.");
        return;
    }

    QString reservationTime = ui->ReservationTime->text().trimmed();
    QString customerName = ui->CustomerName->text().trimmed();
    QString phoneNum = ui->PhoneNum->text().trimmed();

    if (reservationTime.isEmpty() || customerName.isEmpty() || phoneNum.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all fields correctly.");
        return;
    }

    json newData;
    newData["TableNum"] = tableNum;
    newData["ReservationTime"] = reservationTime.toStdString();
    newData["CustomerName"] = customerName.toStdString();
    newData["PhoneNum"] = phoneNum.toStdString();

    json allData;
    getAllData(allData);
    std::string key = "Reservation_" + std::to_string(tableNum);
    setData(newData, key);

    emit tableReserved(tableNum, customerName);
    accept();
}
