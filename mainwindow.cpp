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
}

RestuarantManagement::~RestuarantManagement()
{}

int RestuarantManagement::GetSelectingTableNo(){
    return ui.SelectingTable->text().toInt();
}

void RestuarantManagement::SetSelectingTable(QString no){
    ui.SelectingTable->setText(QString(no));

}

void RestuarantManagement::on_TableBtn_clicked()
{
    QPushButton *buttonSender = qobject_cast<QPushButton*>(sender());
    if (buttonSender) {
        QString buttonText = buttonSender->text(); // Get the text from the button
        SetSelectingTable(buttonText[6]);
    }
}


void RestuarantManagement::on_RefreshBtn_clicked()
{

}



