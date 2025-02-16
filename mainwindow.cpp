#include "mainwindow.h"
#include <header/json.h>
#include <ui/reserve.h>
#include <ui/employee.h>
#include <ui/opentabledialog.h>
#include <ui/reserve.h>
#include <ui/stockwindow.h>
#include <QResizeEvent>
#include <QScreen>
#include <QFont>

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
    ui.comboBox->setCurrentIndex(2);
    on_comboBox_activated(2);

    updateTablesStatus();
    for(int i=1;i<=Table_Count;++i){
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);
        button->setStyleSheet("QPushButton {"                                   // sutup buttonTable-color
                              "background-color: #535455;"  // background-color
                              "color: white;"               // text-color
                              "border-radius: 12px;"        // Rounded corners
                              //"font-size: 16px;"            // Font size
                              "}");
        if(button) connect(button, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }

    ui.Receipt->hide();
    setMainBtnVisible(false);
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
                              //"font-size: 16px;"            // Font size
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
                              //"font-size: 16px;"            // Font size
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

void RestuarantManagement::resizeEvent(QResizeEvent *event){
    QSize newSize = event->size();

    int newWidth = newSize.width();
    int newHeight = newSize.height();

    //int newHeight = (newWidth * 3) / 4; // คำนวณขนาดใหม่ตามอัตราส่วน 4:3
    qDebug() << "New Width:" << newWidth << ", New Height:" << newHeight;
    //resize(newWidth, newHeight);



    int perScale = newWidth*100/1024;
    double Scale = perScale/100.0;
    qDebug() <<"perScale: " << perScale <<" Scale: "<< Scale;

    // -----------------------------------------------------------------------------------------------------------------------

    QFont FontheaderFont("Sitka", max(36, 10));
    ui.MyRestuarantManager->setFont(FontheaderFont);
    QFont checkFontheaderFont = ui.MyRestuarantManager->font();
    qDebug() <<"MyRestuarantManager->font(): " << checkFontheaderFont;


    // -----------------------------------------------------------------------------------------------------------------------

    QFont FontDatetime("SimSun", max(16, 10));
    ui.label_2->setFont(FontDatetime);
    QFont checkFontDatetime = ui.label_2->font();
    qDebug() <<"label_2->font(): " <<checkFontDatetime;

    // -----------------------------------------------------------------------------------------------------------------------

    int newSizeFont = width() / 88;  // ปรับขนาดฟอนต์ตามความกว้างของหน้าต่าง
    QString f = QString::number(newSizeFont);
    qDebug() <<"newSizeFont: " << newSizeFont;

    QFont font("Segoe UI", std::max(newSizeFont, 10));  // กำหนดฟอนต์ขั้นต่ำ 10
    //->setFont(font);
    ui.frame->setStyleSheet("font: 800 "+f+"pt Segoe UI;");
    qDebug() <<"newSizeFont: " << newSizeFont;
    qDebug() <<"newframeSizeFont: " << f;

    // -----------------------------------------------------------------------------------------------------------------------


    int sizebutton = perScale;
    qDebug() <<"BottonTable size: " <<sizebutton;

    for(int i=1;i<=Table_Count;++i){
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);


        if(button){
            button->setFixedSize(sizebutton,sizebutton);
            button->setFont(font);
        }
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }

    // -----------------------------------------------------------------------------------------------------------------------

    int newsizefontReceipt = newSizeFont*4/5;
    qDebug() <<"newsizefontReceipt: " <<newsizefontReceipt;
    QString fReceipt = QString::number(newsizefontReceipt);

    QFont Receiptfont("Segoe UI", std::max(newsizefontReceipt, 3));
    ui.Receipt->setStyleSheet("background-color: rgb(255, 255, 255);"
                                "font: 800 "+fReceipt+"pt Segoe UI;"
                                "color: rgb(0, 0, 0);");
    //ui.Receipt->setFont(Receiptfont);

    QFont c = ui.Receipt->font();
    qDebug() <<"Receipt->font(): " <<c;
    // QSize d = ui.Receipt->frameSize();
    //  qDebug() <<"Receipt->frameSize: " <<d;

}



void RestuarantManagement::on_comboBox_activated(int index)
{
    //int x = 1024, h = 768;
    switch(index){
    case 0:
        setFixedSize(3584,2688);
        //setFixedSize(x*3.5, h*3.5);
        break;
    case 1:
        setFixedSize(1536,1152);
        //setFixedSize(x*1.50, h*1.50);
        break;
    case 2:
        setFixedSize(1280,960);
        //setFixedSize(x*1.25, h*1.25);
        break;
    case 3:
        setFixedSize(1024,768);
        //setFixedSize(x, h);
        break;
    case 4:
        setFixedSize(922,692);
        //setFixedSize(x*0.9, h*0.9);
        break;
    }
}

void RestuarantManagement::on_Eventscreen_buttontablesize(){
    double side = 125;

    for(int i=1;i<=Table_Count;++i){
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);


        if(button){
        button->setFixedSize(side,side);
        }
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }


}
