#include "editmenu.h"
#include "qpushbutton.h"
#include "ui_editmenu.h"
#include "header/json.h"

//define variables
QTableWidget *menutable;
QTableWidget *ingtable; //ingredient table

editmenu::editmenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editmenu)
{
    ui->setupUi(this);

    this->setWindowTitle("Edit Menu");

    //Set Both Variables
    menutable = ui->MenuTable;
    ingtable = ui->IngTable;

    //Setup Menu Table
    menutable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    menutable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    menutable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    menutable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    menutable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    //Setup Ingredients Table
    ingtable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ingtable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

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

        // Add Button
        QPushButton *pb = new QPushButton("Edit Ingredients...",menutable); //Define push button
        pb->setFocusPolicy(Qt::NoFocus);
        menutable->setCellWidget(i,4,pb);
        connect(pb, &QPushButton::clicked, this, [i, Menus]() { //Connect pb function
            //Define pb function
            ingtable->setRowCount(0); //delete all rows
            //get ingredients data from lamda variables
            json ingName = Menus[i][3];
            json ingQuan = Menus[i][4];

            for(int j = 0; j < lenData(ingName); j++){
                ingtable->insertRow(j); //Insert row
                ingtable->setItem(j,0,new QTableWidgetItem(QString::fromStdString(ingName[j]))); //Name
                ingtable->setItem(j,1,new QTableWidgetItem(QString::number(double(ingQuan[j])))); //Quantity
            }

        });

        menutable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Menus[i][0]))); //Name
        menutable->setItem(i,1,new QTableWidgetItem(QString::number(int(Menus[i][1])))); //Price

        //Type
        QString type = QString::fromStdString(Menus[i][2]);
        if(type=="Dishes") menutable->setItem(i,2,new QTableWidgetItem("🍽️Dishes"));
        else menutable->setItem(i,2,new QTableWidgetItem("🍷Drinks"));

        menutable->setItem(i,3,new QTableWidgetItem(QString::number(lenData(Menus[i][3])))); //Ingredients Count
    }
}

