#include "editmenu.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "ui_editmenu.h"
#include "header/json.h"

//define variables
QTableWidget *menutable;
QTableWidget *ingtable; //ingredient table
json Menus;

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

    on_RefreshBtn_clicked(false); //Refresh on setup
}

editmenu::~editmenu()
{
    delete ui;
}

void editmenu::on_RefreshBtn_clicked(bool NoGetdata=false)
{
    menutable->setRowCount(0); //delete all rows
    ingtable->setRowCount(0); //delete all rows

    if(!NoGetdata) getData(Menus,"Menus");

    //Create rows
    for (int i = 0; i < lenData(Menus); ++i) {
        menutable->insertRow(i); //Insert row

        // Add Button
        QPushButton *pb = new QPushButton("Edit Ingredients...",menutable); //Define push button
        pb->setFocusPolicy(Qt::NoFocus);
        menutable->setCellWidget(i,4,pb);
        connect(pb, &QPushButton::clicked, this, [i, this]() { //Connect pb function
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

            //Update information
            ui->ingNo->setText(QString::number(i+1));
            ui->ingLabel->setText(QString::fromStdString(Menus[i][0]));
        });

        menutable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Menus[i][0]))); //Name
        menutable->setItem(i,1,new QTableWidgetItem(QString::number(int(Menus[i][1])))); //Price

        //Type
        QString type = QString::fromStdString(Menus[i][2]);
        QComboBox *typebox = new QComboBox;
        typebox->addItem("🍽️Dishes");
        typebox->addItem("🍷Drinks");
        typebox->addItem("⚠️Not Selected");
        menutable->setCellWidget(i,2,typebox);
        if(type=="Dishes") typebox->setCurrentIndex(0);
        else if(type=="Drinks") typebox->setCurrentIndex(1);
        else typebox->setCurrentIndex(2);
        connect(typebox, &QComboBox::currentIndexChanged,this, [i, typebox](){ //Connect typebox function
            //Define typebox function
            Menus[i][2] = typebox->currentText().remove(0,2).toStdString(); //Set Menus removed emoji (first and second char or QString)
        });

        //Ingredients Count
        QTableWidgetItem *ingCountTWI = new QTableWidgetItem(QString::number(lenData(Menus[i][3])));
        ingCountTWI->setFlags(ingCountTWI->flags() & ~Qt::ItemIsEditable); //Made ingCountTWI uneditable
        menutable->setItem(i,3,ingCountTWI);
    }
}

void editmenu::on_AddMenuBtn_clicked()
{
    int rowCount = menutable->rowCount(); //get last row position
    //Create Blank data
    Menus[rowCount][0]="";
    Menus[rowCount][1]=0;
    Menus[rowCount][2]="Not Selected";
    Menus[rowCount][3]={};
    Menus[rowCount][4]={};
    on_RefreshBtn_clicked(true);
}

void editmenu::on_DelMenuBtn_clicked()
{
    for(auto *item : menutable->selectedItems()) menutable->removeRow(item->row());
}
