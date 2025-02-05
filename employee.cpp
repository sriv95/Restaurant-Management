#include "employee.h"
#include "ui_employee.h"
#include <QDebug>
#include <QDir>
#include <header/json.h>
#include <string>

employee::employee(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::employee)
{
    ui->setupUi(this);
    Refresh_TableData();
}

employee::~employee()
{
    delete ui;
}

void employee::Refresh_TableData()
{
    json employee;
    getData(employee , "Employee");
    int number_of_employee = lenData(employee);

    ui->Table->setRowCount(number_of_employee);

    for (int i = 0 ; i < number_of_employee ; i++)
    {
        QString name = QString::fromStdString(employee[i]["Name"]);
        QString Age = QString::number(int(employee[i]["Age"]));
        QString Gender = QString::fromStdString(employee[i]["Gender"]);
        QString Salary = QString::number(double(employee[i]["Salary"]) , 'f' , 2);
        QString Job = QString::fromStdString(employee[i]["Job"]);

        ui->Table->setItem(i , 0 , new QTableWidgetItem(name));
        ui->Table->setItem(i , 1 , new QTableWidgetItem(Age));
        ui->Table->setItem(i , 2 , new QTableWidgetItem(Gender));
        ui->Table->setItem(i , 3 , new QTableWidgetItem(Salary));
        ui->Table->setItem(i , 4 , new QTableWidgetItem(Job));
    }
}

void employee::on_Refresh_Button_clicked()
{
    Refresh_TableData();
}

