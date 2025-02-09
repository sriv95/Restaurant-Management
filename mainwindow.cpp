#include "mainwindow.h"
#include <header/json.h>
#include <ui/reserve.h>
#include <ui/employee.h>
#include <ui/opentabledialog.h>
#include <ui/reserve.h>
#include <ui/stockwindow.h>
#include <ui/editmenu.h>


using namespace std;

int Table_Count = 9;

void RestuarantManagement::setMainBtnVisible(bool tf){
    ui.CheckBills->setVisible(tf);
    ui.ReserveBtn->setVisible(tf);
    ui.OpenTableBtn->setVisible(tf);
}

RestuarantManagement::RestuarantManagement(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    updateTablesStatus();
    for(int i=1;i<=Table_Count;++i){
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);
        button->setStyleSheet("QPushButton {"                                   // sutup buttonTable-color
                              "background-color: #535455;"  // background-color
                              "color: white;"               // text-color
                              "border-radius: 12px;"        // Rounded corners
                              "font-size: 16px;"            // Font size
                              "}");
        if(button) connect(button, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }

    ui.Receipt->hide();
    setMainBtnVisible(false);

    on_EditMenu_clicked(); //Edit Menu Fast Test
}

RestuarantManagement::~RestuarantManagement()
{}

int RestuarantManagement::GetSelectingTableNo(){
    return ui.SelectingTable->text().toInt();
}

void RestuarantManagement::SetSelectingTable(QString no){
    for(int i =1 ; i <=Table_Count ; i++)
    {
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);
        button->setStyleSheet("QPushButton {"
                              "background-color: #535455;"  // background-color
                              "color: white;"               // text-color
                              "border-radius: 12px;"        // Rounded corners
                              "font-size: 16px;"            // Font size
                              "}");
    }
    if(ui.SelectingTable->text()!=no) {
        ui.SelectingTable->setText(QString(no));
        setMainBtnVisible(true);
        QString btnName = QString("Table_").append(no);
        QPushButton *button = this->findChild<QPushButton *>(btnName);
        button->setStyleSheet("QPushButton {"
                              "background-color: #4CAF50;"  // Green background
                              "color: white;"               // White text
                              "border-radius: 12px;"        // Rounded corners
                              //"border:5px solid "
                              "font-size: 16px;"            // Font size
                              "}");
    }
    else {
        ui.SelectingTable->setText(QString('0'));
        setMainBtnVisible(false);
    }
}

void RestuarantManagement::on_TableBtn_clicked()
{
    QPushButton *buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonName = buttonSender->objectName(); // Get the name from the button
    QString table_no = buttonName.split("_").last();
    SetSelectingTable(table_no);
    Changeseats();

}

void RestuarantManagement::updateTablesStatus()
{
    json Tables;
    getData(Tables,"Tables");


    for(int i=1;i<=Table_Count;++i){
        int seat = Tables [i-1]["Seats"];
        QString Reserved= QString::fromStdString(Tables[i-1]["Reserved"]);
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);
        if(button){
            if(seat != 0)button->setText("Table#"+ QString::number(i) + "\n" + QString::number(seat)+ "👤");
            else if(Reserved !="")button->setText("Table#"+ QString::number(i) + "\nReserved: "+ Reserved);
            else button->setText("Table#"+ QString::number(i) + "\nAvallable");

        }
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }
}


void RestuarantManagement::on_RefreshBtn_clicked()
{
    ui.SelectingTable->setText(QString('0'));
    QString table_no = "0";
    ui.Receipt->hide();
    SetSelectingTable(table_no);
    updateTablesStatus(); //
}

void RestuarantManagement::on_CheckBills_clicked()
{
    ui.Receipt->setVisible(!ui.Receipt->isVisible()); //Checkbill
}


void RestuarantManagement::on_OpenTableBtn_clicked()
{
    OpenTableDialog OpenTableDialog;
    OpenTableDialog.setModal(true);
    connect(&OpenTableDialog, &OpenTableDialog::returnValue, this, &RestuarantManagement::onTableReturnValue);
    OpenTableDialog.exec();
}

void RestuarantManagement::onTableReturnValue(const QString &data){
    json Tables;
    getData(Tables,"Tables");
    int No = GetSelectingTableNo() - 1 ;
    int seat = data.toInt(); //handle unused variable
    // if(seat > 4 or seat < 0)
    // {
    //     qDebug() << "Error: Number of seats. Enter the number of seats again.";
    //     seat = 0;
    // }
    //qDebug() << "Seats: " << seat << "TableNo: " <<GetSelectingTableNo(); //
    Tables [No]["Seats"] = seat;

    setData(Tables, "Tables");
    updateTablesStatus();

}
void RestuarantManagement::Changeseats() //แก้บัค
{
    json Tables;
    getData(Tables,"Tables");

    int No = GetSelectingTableNo() - 1 ;
    if(No >= 0)
    {
    if(Tables[No]["Seats"] != 0) ui.OpenTableBtn->setText("Change seat");
    else ui.OpenTableBtn->setText("Open Table");
    }
}


void RestuarantManagement::on_ReserveBtn_clicked()
{
    reserve reserve;
    reserve.setModal(true);
    //connect returnValue
    reserve.exec();
}

// void RestuarantManagement::onReserveReturnValue(const QString &data){

// }

void RestuarantManagement::on_Employee_clicked()
{
    employee employee(this);
    employee.exec();
}

void RestuarantManagement::on_Stocks_clicked()
{
    StockWindow *stockWin = new StockWindow(this);


    stockWin->setWindowFlags(Qt::Window);


    stockWin->show();
    stockWin->raise();
    stockWin->activateWindow();
}



void RestuarantManagement::on_EditMenu_clicked()
{
    editmenu editmenu(this);
    editmenu.exec();
}

