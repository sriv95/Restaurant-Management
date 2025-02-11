#include "mainwindow.h"
#include <header/json.h>
#include <opentabledialog.h>
#include <reserve.h>
#include <QTimer> //TT
#include <QDateTime> //TT


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

    for(int i=1;i<=Table_Count;++i){
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);
        if(button) connect(button, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }

    ui.Receipt->hide();
    setMainBtnVisible(false);

    //บอกเวลาเเละวันที่
    QTimer *timer=new QTimer(this);
    connect (timer ,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start();
}

void  RestuarantManagement::showTime() //TT
{
    // ดึงวันที่และเวลา
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeText = dateTime.toString("dd/MM/yyyy HH:mm:ss");

    ui.Digital_clock->setText(dateTimeText);
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
        updateReserveButtonText(no.toInt()); //TT
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
    QString x = data;
    qDebug("x");
}


void RestuarantManagement::on_ReserveBtn_clicked()
{
    int tableNo = GetSelectingTableNo();
    if (ui.ReserveBtn->text() == "Unreserve") {
        removeReservation(tableNo);
        ui.ReserveBtn->setText("Reserve");
    } else {
        reserve reserve(tableNo);
        reserve.setModal(true);
        reserve.exec();
    }
}

bool RestuarantManagement::isTableReserved(int tableNo) {
    json reservations;
    getData(reservations, "Reservation");

    for (const auto &reservation : reservations) {
        if (reservation[0] == tableNo) {
            return true;
        }
    }
    return false;
}

void RestuarantManagement::updateReserveButtonText(int tableNo) {
    if (isTableReserved(tableNo)) {
        ui.ReserveBtn->setText("Unreserve");
    } else {
        ui.ReserveBtn->setText("Reserve");
    }
}

void RestuarantManagement::removeReservation(int tableNo) {
    json allData;
    getAllData(allData);

    json updatedReservations = json::array();
    for (const auto &reservation : allData["Reservation"]) {
        if (reservation[0] != tableNo) {
            updatedReservations.push_back(reservation);
        }
    }
    allData["Reservation"] = updatedReservations;

    for (auto &table : allData["Tables"]) {
        if (table["No"] == tableNo) {
            table["Reserved"] = "";
        }
    }

    setAllData(allData);

    qDebug() << "Reservation for table" << tableNo << "removed.";
}

// void RestuarantManagement::onReserveReturnValue(const QString &data){

// }
