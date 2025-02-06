#include "mainwindow.h"
#include <header/json.h>
#include <opentabledialog.h>
#include <reserve.h>
#include <QDebug>
#include <QDir>
#include <employee.h>

using namespace std;

void RestuarantManagement::setMainBtnVisible(bool tf){
    ui.CheckBills->setVisible(tf);
    ui.ReserveBtn->setVisible(tf);
    ui.OpenTableBtn->setVisible(tf);
}

RestuarantManagement::RestuarantManagement(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    qDebug()<<"Current Directory is: "<<QDir::currentPath();
    connect(ui.Table_1, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_2, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_3, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_4, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_5, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_6, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_7, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_8, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_9, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);

    ui.Receipt->hide();
    setMainBtnVisible(false);
}

RestuarantManagement::~RestuarantManagement()
{}

int RestuarantManagement::GetSelectingTableNo(){
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

    // buttonSender->setText("Table#"+table_no+"\n .."); //⚠️needs getTableStatus
}

void RestuarantManagement::on_RefreshBtn_clicked()
{

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
    QString x = data; //handle unused variable
    qDebug() << x;
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

