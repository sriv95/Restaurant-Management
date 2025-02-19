#include "employee.h"
#include "ui_employee.h"
#include <QDebug>
#include <QDir>
#include <header/json.h>
#include <string>
#include <cstdlib>
#include <QMessageBox>
#include <algorithm>
#include <QResizeEvent>

using namespace std;

const string Employee_Keys[] = {"Name" , "Age" , "Gender" , "Salary" , "Job"};
const int len_Employee_Keys = 5;

employee::employee(QWidget *parent,int index)
    : QDialog(parent)
    , ui(new Ui::employee)
{
    ui->setupUi(this);
    OpenScreen(index);
    this->setWindowTitle("Employee");
    Refresh_TableData();
    ui->Table->clearSelection();

    ui->Table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->Table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->Table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->Table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->Table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
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

void employee::on_Add_Button_clicked()
{
    ui->Table->insertRow(ui->Table->rowCount());
}

void employee::on_Delete_Button_clicked()
{
    QModelIndexList selected_row_list = ui->Table->selectionModel()->selectedRows();
    int selected_size = selected_row_list.size();

    if (selected_size <= 0)
    {
        QMessageBox::about(this , "❗warning❗" , "❗Delete failed.❗Please select the row you want to delete by clicking on the first column of the table (the column with row numbers).");
    }
    else
    {
        vector<int> seleted_row_vector;
        for (int i = 0 ; i < selected_size ; i++)
        {
            seleted_row_vector.push_back(int(selected_row_list[i].row()));
        }

        sort(seleted_row_vector.begin(),seleted_row_vector.end());

        for (int i = seleted_row_vector.size() - 1 ; i >= 0  ; i--)
        {
            ui->Table->removeRow(seleted_row_vector[i]);
        }
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
                QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗There is an empty cell in row " + QString::number(i+1) + ".");
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
                    QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗ Invalid data type in row " + QString::number(i+1) + ", column " + QString::number(j+1) + "(Age). It should be an integer." );
                    return false;
                }
                break;
            case 3:
                ui->Table->item(i,j)->text().toDouble(&test_value);
                if (test_value == false)
                {
                    QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗ Invalid data type in row " + QString::number(i+1) + ", column " + QString::number(j+1) + "(Salary). It should be a number." );
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

            QMessageBox Save_successful_Message;
            Save_successful_Message.setWindowTitle("🎉🥳✅✅🥳🎉");
            Save_successful_Message.setText("✅🟢🥳🎉-Save successful-🎉🥳🟢✅");
            Save_successful_Message.setWindowFlags(Qt::Popup);
            Save_successful_Message.exec();

            // QMessageBox::about(this , "🎉🥳✅✅🥳🎉" , "✅🟢🥳🎉-Save successful-🎉🥳🟢✅");
        }
    }
}

void employee::resizeEvent(QResizeEvent *event){
    QSize newSize = event->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();
    qDebug() << "-------------------employee-------------------";
    qDebug() << "New Width:" << newWidth << ", New Height:" << newHeight;

    int defaultWidth = 1024;
    int perScale = newWidth*100/defaultWidth;
    double Scale = perScale/100.0;
    qDebug() <<"perScale: " << perScale <<" Scale: "<< Scale;

    // setFrontTable -----------------------------------------------------------------------------------------------------------------------

    int defaultTableFont = 12;
    int intTableFont = defaultTableFont*perScale/100;
    QFont FontTable("Segoe UI", max(intTableFont, 3));
    ui->Table->setFont(FontTable);
    qDebug() <<"FrontTable: "  << ui->Table->font();


    // int defaultTableFont = 12;
    // int intFrontTable = defaultTableFont*100/defaultWidth;
    // QString FrontTableSize = QString::number(intFrontTable);
    // ui->Table->setStyleSheet("font: 400 "+FrontTableSize+"pt Segoe UI;");

    // setStyleSheetWidgetButton -----------------------------------------------------------------------------------------------------------------------

    int defaultButtonFont = 12;
    int newFontbuttonSize = defaultButtonFont*perScale/100;
    QString FontButton = QString::number(newFontbuttonSize);
    qDebug() <<"newFontbuttonSize: " << newFontbuttonSize;

    QDialog OpenTableDialog;
    ui->widgetButton->setStyleSheet("font: 400 "+FontButton+"pt Segoe UI;");

    // setLabelFont ---------------------------------------------------------------------------------------------------------------

    int defaultLabelFont = 8;
    int intLabelFont = defaultLabelFont*perScale/100;
    QFont Fontlabel("Segoe UI", max(intLabelFont, 3));
    ui->label->setFont(Fontlabel);

}

void employee::OpenScreen(int index){
    int w = 1024, h = 768;

    switch(index){
    case 0:
        setFixedSize(w*3.5, h*3.5);
        break;
    case 1:
        setFixedSize(w*1.50, h*1.50);
        break;
    case 2:
        setFixedSize(w*1.25, h*1.25);
        break;
    case 3:
        setFixedSize(w, h);
        break;
    case 4:
        setFixedSize(w*0.9, h*0.9);
        break;
    }

}
