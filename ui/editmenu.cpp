#include "editmenu.h"
#include "qcombobox.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "ui_editmenu.h"
#include "header/json.h"
#include <QResizeEvent>

#include <QSpinBox>

//define variables
QTableWidget *menutable;
QTableWidget *ingtable; //ingredient table
json Menus;

editmenu::editmenu(QWidget *parent, int index)
    : QDialog(parent)
    , ui(new Ui::editmenu)
{
    ui->setupUi(this);
    OpenScreen(index);

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
    ingtable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    on_RefreshBtn_clicked(false); //Refresh on setup

    connect(menutable, &QTableWidget::itemChanged, this, &editmenu::onMenuTableItemChanged); //Connect item changed (Menu Name)
}

editmenu::~editmenu()
{
    delete ui;
}

void editmenu::RefreshIng(int i){
    //Define pb function
    ingtable->setRowCount(0); //delete all rows
    //get ingredients data from lamda variables
    json ingName = Menus[i][3];
    json ingQuan = Menus[i][4];

    json Stocks;
    getData(Stocks,"Stocks");

    for(int j = 0; j < lenData(ingName); j++){
        ingtable->insertRow(j); //Insert row
        //Name
        QComboBox *namebox = new QComboBox;
        namebox->addItem("-");
        namebox->setCurrentIndex(0);
        ingtable->setCellWidget(j,0,namebox);
        for(auto item : Stocks){ //using iterator for loop
            QString name = QString::fromStdString(item[0]);
            namebox->addItem(name);
            if(QString::fromStdString(ingName[j]) == name ) namebox->setCurrentText(name);
        }
        connect(namebox, &QComboBox::currentTextChanged, this, [i,j](QString val){
            Menus[i][3][j] = val.toStdString(); //Set Name to current value
        });

        //Quantity
        QDoubleSpinBox *Quan = new QDoubleSpinBox();
        Quan->setMinimum(0);
        Quan->setMaximum(1000000); //Maximum Quan
        Quan->setValue(double(ingQuan[j]));
        ingtable->setCellWidget(j,1,Quan);
        connect(Quan, &QDoubleSpinBox::valueChanged,this, [i,j](double val){ //Connect Quan QDoubleSpinBox function
            Menus[i][4][j] = val; //Set Quantity to current value
        });

        //Delete Button
        QPushButton *delBtn = new QPushButton("❌");
        delBtn->setFixedSize(QSize(30,30));
        ingtable->setCellWidget(j,2,delBtn);
        connect(delBtn, &QPushButton::clicked, this, [j, i, this](){ //Connect Delete Button function
            //Define Delete Button Function
            ingtable->removeRow(j);
            Menus[i][3].erase(j);
            Menus[i][4].erase(j);
            RefreshIng(i);
        });
    }

    //Update information
    ui->ingNo->setText(QString::number(i+1));
    ui->ingLabel->setText(QString::fromStdString(Menus[i][0]));
}

void editmenu::on_RefreshBtn_clicked(bool NoGetdata=false)
{
    menutable->setRowCount(0); //delete all rows
    ingtable->setRowCount(0); //delete all rows

    //Set Menu Ingredients information
    ui->ingLabel->setText("No Selected Menu");
    ui->ingNo->setText("0");

    if(!NoGetdata) getData(Menus,"Menus");

    //Create rows
    for (int i = 0; i < lenData(Menus); ++i) {
        menutable->insertRow(i); //Insert row

        // Add Button
        QPushButton *pb = new QPushButton("Edit Ingredients...",menutable); //Define push button
        pb->setFocusPolicy(Qt::NoFocus);
        menutable->setCellWidget(i,4,pb);
        //Connect pb function
        connect(pb, &QPushButton::clicked, this, [i, this]() {
            RefreshIng(i);
        });

        menutable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Menus[i][0]))); //Name

        //Price
        QSpinBox *Price = new QSpinBox();
        Price->setMinimum(0);
        Price->setMaximum(1000); //Maximum Price
        Price->setValue(int(Menus[i][1]));
        menutable->setCellWidget(i,1,Price);
        connect(Price, &QSpinBox::valueChanged,this, [i](int val){ //Connect Price QSpinBox function
            Menus[i][1] = val; //Set Price to current value
        });

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
        connect(typebox, &QComboBox::currentIndexChanged,this, [i](int id){ //Connect typebox function
            //Define typebox function
            switch (id) {
            case 0:
                Menus[i][2] = "Dishes";
                break;
            case 1:
                Menus[i][2] = "Drinks";
                break;
            case 2:
                Menus[i][2] = "Not Selected";
                break;
            default:
                break;
            }

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
    for(auto *item : menutable->selectedItems()) {
        //Delete row using iterator for loop
        int i = item->row(); //Get current row
        if(i>=0) Menus.erase(i); //Delete from Menus and use i>=0 from handling i == -1 then crash
    }
    on_RefreshBtn_clicked(true);
}

void editmenu::on_AddIngBtn_clicked()
{
    int i = ui->ingNo->text().toInt()-1; //Find current editing
    if(i>=0){
        int rowCount = ingtable->rowCount(); //get last row position
        Menus[i][3][rowCount]="";
        Menus[i][4][rowCount]=0;
        RefreshIng(i);
    }
}


void editmenu::on_SaveMenuBtn_clicked()
{
    QMessageBox Save_successful_Message;
    Save_successful_Message.setWindowTitle("🎉🥳✅✅🥳🎉");
    Save_successful_Message.setText("✅🟢🥳🎉-Save successful-🎉🥳🟢✅");
    Save_successful_Message.setWindowFlags(Qt::Popup);


    for(int i=0;i<lenData(Menus);i++){
        //Handling empty Menu name
        if(Menus[i][0]==""||Menus[i][0]==nullptr){
            QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗There is an empty cell in row " + QString::number(i+1) + ".");
            return;
        }

        //Handling Not Selected type
        if(Menus[i][2]=="Not Selected"||Menus[i][2]==nullptr){
            QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗There is a Not Selected type in row " + QString::number(i+1) + ".");
            return;
        }

        //Handling 0 Ingredients
        if(lenData(Menus[i][3])<=0){
            QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗There is no ingredients in row " + QString::number(i+1) + ".");
            return;
        }

        //Handling empty ingredients name
        for(auto j : Menus[i][3]){
            if(j==""){
                QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗There is empty ingredients in row " + QString::number(i+1) + ".");
                return;
            }
        }

        //Handling 0 Quantity
        for(auto j : Menus[i][4]){
            if(j==0){
                QMessageBox::about(this , "❗warning❗" , "❗Save failed.❗There is 0 Quantity in row " + QString::number(i+1) + ".");
                return;
            }
        }
    }

    setData(Menus,"Menus");
    Save_successful_Message.exec();
}

void editmenu::onMenuTableItemChanged(QTableWidgetItem *item)
{
    int Row = item->row();
    int Col = item->column();
    if(Col==0) Menus[Row][0] = item->text().toStdString(); //Change Menu Name
}

void editmenu::resizeEvent(QResizeEvent *event){
    QSize newSize = event->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();
    qDebug() << "-------------------editmenu-------------------";
    qDebug() << "New Width:" << newWidth << ", New Height:" << newHeight;

    int defaultWidth = 1024;
    int perScale = newWidth*100/defaultWidth;
    double Scale = perScale/100.0;
    qDebug() <<"perScale: " << perScale <<" Scale: "<< Scale;

    resizeFont(perScale);



}

void editmenu::resizeFont(double perScale)
{
    // setFontLabel_3&4 ---------------------------------------------------------------------------------------------------------------

    int defaultlFontLabel_3 = 20;
    int intFontLabel_3 = defaultlFontLabel_3*perScale/100;
    QString FontLabel_3 = QString::number(intFontLabel_3);

    ui->label_3->setStyleSheet("font: 400 "+FontLabel_3+"pt Segoe UI;");
    ui->label_4->setStyleSheet("font: 400 "+FontLabel_3+"pt Segoe UI;");

    qDebug() <<"FontLabel_3&4: "  << ui->label_3->font();


    // setFront TextSelecteMenu -----------------------------------------------------------------------------------------------------------------------

    int defaultFrontTextSelecteMenu = 6;
    int intFrontTextSelecteMenu = defaultFrontTextSelecteMenu*perScale/100;
    QFont FrontTextSelecteMenu("Segoe UI", max(intFrontTextSelecteMenu, 3));
    ui->TextSelecteMenu->setFont(FrontTextSelecteMenu);
    qDebug() <<"TextSelecteMenu: "  << ui->TextSelecteMenu->font();

    //setFont MenuTable & IngTable-----------------------------------------------------------------------------------------------------------------------

    int defaultHeaderFont = 6;
    int intHeaderFont = defaultHeaderFont*perScale/100;
    QFont HeaderFont("Segoe UI", intHeaderFont, QFont::Bold);
    ui->MenuTable->horizontalHeader()->setFont(HeaderFont);
    ui->IngTable->horizontalHeader()->setFont(HeaderFont);
    qDebug() <<"MenuTable_HeaderFont: "  << ui->MenuTable->horizontalHeader()->font();
    qDebug() <<"IngTable_HeaderFont: "  << ui->IngTable->horizontalHeader()->font();

    int defaultFontTable = 6;
    int intFontTable = defaultFontTable*perScale/100;
    QFont FontTable("Segoe UI", max(intFontTable, 3));
    ui->MenuTable->setFont(FontTable);
    ui->IngTable->setFont(FontTable);
    qDebug() <<"FontMenuTable: "  << ui->MenuTable->font();
    qDebug() <<"FontIngTable: "  << ui->IngTable->font();


    // setFont Widget_Button2 -----------------------------------------------------------------------------------------------------------------------

    int defaultFontWidget_Button2 = 9;
    int intFontWidget_Button2 = defaultFontWidget_Button2*perScale/100;
    QString FontWidget_Button2 = QString::number(intFontWidget_Button2);
    ui->Widget_Button2->setStyleSheet("font: 400 "+FontWidget_Button2+"pt Segoe UI;");
    qDebug() <<"Widget_Button: "  << ui->Widget_Button2->font();

    // setFont Button -----------------------------------------------------------------------------------------------------------------------

    ui->AddMenuBtn->setStyleSheet("font: 400 "+FontWidget_Button2+"pt Segoe UI;");
    ui->DelMenuBtn->setStyleSheet("font: 400 "+FontWidget_Button2+"pt Segoe UI;");
    ui->AddIngBtn->setStyleSheet("font: 400 "+FontWidget_Button2+"pt Segoe UI;");





}

void editmenu::OpenScreen(int index){
    int w = 1024, h = 568; //18:10 1024x576 16:9

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
