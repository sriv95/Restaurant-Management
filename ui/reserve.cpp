#include "reserve.h"
#include "ui_reserve.h"
#include "header/json.h"
#include <QMessageBox>
#include <QResizeEvent>

reserve::reserve(int tableNo,QWidget *parent, int index)
    : QDialog(parent), ui(new Ui::reserve)
{
    ui->setupUi(this);
    OpenScreen(index);

    ui->TableNum->setText(QString::number(tableNo));
}

reserve::~reserve()
{
    delete ui;
}

void reserve::on_btnReserve_clicked()
{
    int tableNum = ui->TableNum->text().toInt();

    QString reservationTime = ui->ReservationTime->text();
    QString customerName = ui->CustomerName->text();
    QString phoneNum = ui->PhoneNum->text();

    if (reservationTime.isEmpty() || customerName.isEmpty() || phoneNum.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all fields correctly.");
        return;
    }

    json newData;
    newData[0] = tableNum;
    newData[1] = reservationTime.toStdString();
    newData[2] = customerName.toStdString();
    newData[3] = phoneNum.toStdString();

    json Data;
    getData(Data, "Reservation");

    int len = lenData(Data);

    qDebug()<<QString::fromStdString(Data.dump());
    Data[len] = newData;
    qDebug()<<QString::fromStdString(Data.dump());
    setData(Data, "Reservation");

    json tablesData;
    getData(tablesData, "Tables");

    for (auto &table : tablesData) {
        if (table["No"] == tableNum) {
            table["Reserved"] = reservationTime.toStdString();
            break;
        }
    }

    setData(tablesData, "Tables");

    this->close();
}

void reserve::on_Cancelbtn_clicked()
{
    this->close();
}

void reserve::resizeEvent(QResizeEvent *event){
    QSize newSize = event->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();
    qDebug() << "-------------------reserve-------------------";
    qDebug() << "New Width:" << newWidth << ", New Height:" << newHeight;

    int defaultWidth = 389;
    int perScale = newWidth*100/defaultWidth;
    double Scale = perScale/100.0;
    qDebug() <<"perScale: " << perScale <<" Scale: "<< Scale;

    // setFontLabel ---------------------------------------------------------------------------------------------------------------

    int defaultLabelFont = 18;
    int newintLabelFont = defaultLabelFont*perScale/100;
    QString FontLabel = QString::number(newintLabelFont);

    ui->label->setStyleSheet("font: 400 "+FontLabel+"pt Segoe UI;");
    qDebug() <<"FontLabel: "  << ui->label->font();


    // setFrontWidget_widget -----------------------------------------------------------------------------------------------------------------------
    int defaultFrontWidget = 12;
    int intFrontWidget = defaultFrontWidget*perScale/100;
    QFont FrontWidget("Segoe UI", max(intFrontWidget, 3));

    ui->widget->setFont(FrontWidget);
    ui->widget_2->setFont(FrontWidget);

    qDebug() <<"FrontWidget: "  << ui->widget->font();
    qDebug() <<"FrontWidget_Selection: "  << ui->widget->layout();


}

void reserve::OpenScreen(int index){
    int w = 389, h = 430;

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

