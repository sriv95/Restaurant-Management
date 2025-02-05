#include "mainwindow.h"
#include <header/json.h>
#include <opentabledialog.h>
#include <reserve.h>

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
        if(button) connect(button, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }

    ui.Receipt->hide();
    setMainBtnVisible(false);
}

RestuarantManagement::~RestuarantManagement()
{}

int RestuarantManagement::GetSelectingTableNo(){ // used by fong
    return ui.SelectingTable->text().toInt();
}

void RestuarantManagement::SetSelectingTable(QString no){
    if(ui.SelectingTable->text()!=no) {
        ui.SelectingTable->setText(QString(no));
        setMainBtnVisible(true);
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
    Changeseats(); // added by fong

    // buttonSender->setText("Table#"+table_no+"\n .."); //⚠️needs getTableStatus
}

void RestuarantManagement::updateTablesStatus() // added by fong
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


void RestuarantManagement::on_RefreshBtn_clicked()  //Edited by fong
{
    updateTablesStatus();
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

void RestuarantManagement::onTableReturnValue(const QString &data){ //edited by fong
    json Tables;
    getData(Tables,"Tables");
    int seat = data.toInt(); //handle unused variable
    int No = GetSelectingTableNo() - 1 ;
    qDebug() << "Seats: " << seat << "TableNo: " <<GetSelectingTableNo();
    Tables [No]["Seats"] = seat;

    setData(Tables, "Tables");
    updateTablesStatus();

}

void RestuarantManagement::Changeseats()
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
