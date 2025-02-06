#include "employee.h"
#include "ui_employee.h"
#include <QDebug>
#include <QDir>
#include <header/json.h>
#include <string>
#include <cstdlib>

using namespace std;

const string Employee_Keys[] = {"Name" , "Age" , "Gender" , "Salary" , "Job"};
const int len_Employee_Keys = 5;

employee::employee(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::employee)
{
    ui->setupUi(this);
    this->setWindowTitle("Employee");
    Refresh_TableData();
    ui->Table->clearSelection();
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
    ui->Status_Block->setText("");
}

void employee::on_Add_Button_clicked()
{
    ui->Table->insertRow(ui->Table->rowCount());
}

void employee::on_Delete_Button_clicked()
{
    QModelIndexList selected_row_list = ui->Table->selectionModel()->selectedRows();
    int selected_size = selected_row_list.size();

    int deleted_row = -1;
    int j = 0;

    for (int i = 0 ; i < selected_size ; i++)
    {
        int row = selected_row_list[i].row();
        if (row >= deleted_row) row = row - j;

        ui->Table->removeRow(row);

        deleted_row = row;
        j++;
    }
}

void employee::on_Test_Button_clicked()
{
    int Number_of_employee_OnTable = ui->Table->rowCount();

    for (int i = 0 ; i < Number_of_employee_OnTable ; i++)
    {
        for (int j = 0 ; j < 5 ; j++)
        {
            qDebug() << ui->Table->item(i,j);
        }
        qDebug() << "\n";
    }
}

bool employee::Check_empty_cell_in_table() // return True ถ้ามีช่องไหนว่าง | return false ถ้าไม่มีช่องไหนว่าง
{
    int Number_of_employee_OnTable = ui->Table->rowCount();
    for (int i = 0 ; i < Number_of_employee_OnTable ; i++)
    {
        for (int j = 0 ; j < 5 ; j++)
        {
            if (ui->Table->item(i,j) == nullptr or ui->Table->item(i,j)->text().isEmpty())
            {
                QString text = "❗Save failed.❗ Row " + QString::number(i+1) + " have empty cell.";
                ui->Status_Block->setText(text);
                return true;
            }
        }
    }
    return false;
}

bool employee::Check_Correct_DataType_in_cell() // return True ถ้า DataType ทุกช่องสามารถใช้งานได้ | return false ถ้ามี cell ที่ DataType ไม่เหมาะสม
{
    bool test_value;

    int Number_of_employee_OnTable = ui->Table->rowCount();
    for (int i = 0 ; i < Number_of_employee_OnTable ; i++)
    {
        for (int j = 1 ; j <=3 ; j = j+2)
        {
            switch (j) {
            case 1:
                ui->Table->item(i,j)->text().toInt(&test_value);
                if (test_value == false)
                {
                    QString text = "❗Save failed.❗ Data type of Item (" + QString::number(i+1) + "," + QString::fromStdString(Employee_Keys[j]) + ") is incorrect.";
                    ui->Status_Block->setText(text);
                    return false;
                }
                break;
            case 3:
                ui->Table->item(i,j)->text().toDouble(&test_value);
                if (test_value == false)
                {
                    QString text = "❗Save failed.❗ Data type of Item (" + QString::number(i+1) + "," + QString::fromStdString(Employee_Keys[j]) + ") is incorrect.";
                    ui->Status_Block->setText(text);
                    return false;
                }
                break;
            }
        }
    }

    return true;
}

void employee::on_Save_Button_clicked()
{
    int Number_of_employee_OnTable = ui->Table->rowCount();

    if (Check_empty_cell_in_table() == false)
    {
        if (Check_Correct_DataType_in_cell() == true)
        {
            json employee;
            getData(employee , "Employee");
            employee.clear();
            setData(employee , "Employee");

            for (int i = 0 ; i < Number_of_employee_OnTable ; i++)
            {

                string Name = ui->Table->item(i , 0)->text().toStdString();
                int Age = ui->Table->item(i , 1)->text().toInt();
                string Gender = ui->Table->item(i , 2)->text().toStdString();
                double Salary = ui->Table->item(i , 3)->text().toDouble();
                string Job = ui->Table->item(i , 4)->text().toStdString();

                employee[i]["Name"] = Name;
                employee[i]["Age"] = Age;
                employee[i]["Gender"] = Gender;
                employee[i]["Salary"] = Salary;
                employee[i]["Job"] = Job;
            }

            setData(employee , "Employee");
            ui->Status_Block->setText("Save successful.");
        }
    }
}
