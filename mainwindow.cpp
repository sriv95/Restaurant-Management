#include "mainwindow.h"
#include <json.hpp>

using namespace std;



RestuarantManagement::RestuarantManagement(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

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
    ui.RBtn->hide();
    ui.ReserveBtn->hide();
}

RestuarantManagement::~RestuarantManagement()
{}

int RestuarantManagement::GetSelectingTableNo(){
    return ui.SelectingTable->text().toInt();
}

void RestuarantManagement::SetSelectingTable(QString no){
    if(ui.SelectingTable->text()!=no) {
        ui.SelectingTable->setText(QString(no));
        ui.RBtn->show();
        ui.ReserveBtn->show();
    }
    else {
        ui.SelectingTable->setText(QString('0'));
        ui.RBtn->hide();
        ui.ReserveBtn->hide();
    }
}

void RestuarantManagement::on_TableBtn_clicked()
{
    QPushButton *buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonText = buttonSender->text(); // Get the text from the button
    //Crash handling
    if(buttonText.length()>=7){
        SetSelectingTable(buttonText[6]);
    }

    // buttonSender->setText("Table#"+QString(buttonText[6])+"\n .."); //⚠️needs getTableStatus
}

void RestuarantManagement::on_RefreshBtn_clicked()
{

}

void RestuarantManagement::on_RBtn_clicked()
{
    ui.Receipt->setVisible(!ui.Receipt->isVisible()); //Checkbill
}

