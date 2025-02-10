#include "editmenu.h"
#include "qpushbutton.h"
#include "ui_editmenu.h"
#include "header/json.h"

//define variables
QTableWidget *menutable;
QTableWidget *ingtable; //ingredient table

void setupTable(QTableWidget *table){
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
}

editmenu::editmenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editmenu)
{
    ui->setupUi(this);

    this->setWindowTitle("Edit Menu");

    //Set Both Variables
    menutable = ui->MenuTable;
    ingtable = ui->IngTable;

    //Setup Both Table
    setupTable(menutable);
    setupTable(ingtable);

    on_RefreshBtn_clicked(); //Refresh on setup
}

editmenu::~editmenu()
{
    delete ui;
}

void editmenu::on_RefreshBtn_clicked()
{
    menutable->setRowCount(0); //delete all rows

    json Menus;
    getData(Menus,"Menus");

    //Create rows
    for (int i = 0; i < lenData(Menus); ++i) {
        menutable->insertRow(i); //Insert row
        QPushButton *pb = new QPushButton("Edit Ingredients...",menutable); //Define push button
        pb->setFocusPolicy(Qt::NoFocus);

        menutable->setCellWidget(i,4,pb);

        menutable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Menus[i][0]))); //Name
        menutable->setItem(i,1,new QTableWidgetItem(QString::number(int(Menus[i][1])))); //Price

        //Type
        QString type = QString::fromStdString(Menus[i][2]);
        if(type=="Dishes") menutable->setItem(i,2,new QTableWidgetItem("🍽️Dishes"));
        else menutable->setItem(i,2,new QTableWidgetItem("🍷Drinks"));

        menutable->setItem(i,3,new QTableWidgetItem(QString::number(lenData(Menus[i][3])))); //Ingredients Count
    }
}

