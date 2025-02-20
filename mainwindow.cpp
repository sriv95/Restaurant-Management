#include "mainwindow.h"
#include "func/tables.h"
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <header/json.h>
#include <QTimer>
#include <QDateTime>

#include <ui/reserve.h>
#include <ui/employee.h>
#include <ui/opentabledialog.h>
#include <ui/reserve.h>
#include <ui/stockwindow.h>
#include <ui/statement.h>
#include <ui/editmenu.h>
#include <ui/orderfood.h>
#include <ui/analysis.h>
#include "ui/jsoncheck.h"
#include <QResizeEvent>
#include <QScreen>
#include <QFont>
json restaurantData;

int Table_Count = 9;
int ScreensizeComboBox = 2;

void RestuarantManagement::showError(QString text){
    QMessageBox Error;
    Error.setText(text);
    Error.setIcon(QMessageBox::Warning);
    Error.setWindowTitle("Error!");
    Error.exec();
}

void RestuarantManagement::setMainBtnVisible(bool tf){
    ui.CheckBills->setVisible(tf);
    ui.ReserveBtn->setVisible(tf);
    ui.OpenTableBtn->setVisible(tf);
}

RestuarantManagement::RestuarantManagement(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    on_backtosetup_clicked();
    ui.comboBox->setCurrentIndex(ScreensizeComboBox);
    MoreScreensize(ScreensizeComboBox,ScreensizeComboBox);
    //resize(1280,960);

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
    ui.OrderFoodBtn->hide();
    setMainBtnVisible(false);

    QTimer *timer=new QTimer(this);
    connect (timer ,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start();
}

void  RestuarantManagement::showTime()
{
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
    if (ui.Receipt->isVisible() && ui.CheckBills->text() == "Confirm Payment") {
        ui.Receipt->hide();
        ui.CheckBills->setText("Check Bills");
    }

    if(ui.SelectingTable->text()!=no) {
        ui.Receipt->show();
        ui.SelectingTable->setText(QString(no));
        setMainBtnVisible(true);
        ui.OrderFoodBtn->show();
        updateReserveButtonText(no.toInt()); //TT
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
        ui.OrderFoodBtn->hide();
        setMainBtnVisible(false);
    }
}

void RestuarantManagement::on_TableBtn_clicked()
{

    if (ui.Receipt->isVisible() && ui.CheckBills->text() == "Confirm Payment") {
        ui.Receipt->hide();
        ui.CheckBills->setText("Check Bills");
    }

    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    QString buttonName = buttonSender->objectName();
    QString table_no = buttonName.split("_").last();

    SetSelectingTable(table_no);
    on_CheckBills_clicked();
}
void RestuarantManagement::getData()
{
    getAllData(restaurantData);
    Changeseats();

}

void RestuarantManagement::updateTablesStatus()
{
    json Tables;
    ::getData(Tables,"Tables");


    for(int i=1;i<=Table_Count;++i){
        int seat = Tables [i-1]["Seats"];
        QString Reserved= QString::fromStdString(Tables[i-1]["Reserved"]);
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);
        if(button){
            if(seat != 0)button->setText("Table#"+ QString::number(i) + "\n" + QString::number(seat)+ "👤");
            else if(Reserved !=""){
                json Reservation;
                ::getData(Reservation,"Reservation");
                for(auto item : Reservation){
                    if(item[0]==i&&item[1]==Reserved.toStdString()){
                        button->setText("Table#"+ QString::number(i) + "\nReserved: "+ Reserved +"\n"+QString::fromStdString(item[2])+"\n"+QString::fromStdString(item[3]));
                        break;
                    }
                }
            }
            else button->setText("Table#"+ QString::number(i) + "\nAvailable");

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
    getData();

    ui.Receipt_DateTime->setText(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));

    int selectedTableNumber = GetSelectingTableNo();
    if (selectedTableNumber == 0) {
        ui.Receipt->hide();
        return;
    }

    json Bills = restaurantData["Tables"][selectedTableNumber - 1]["Bills"];
    json emptyBills = json::array({json::array({""}), json::array(), json::array()});

    if (ui.CheckBills->text() == "Confirm Payment") {
        if (Bills == emptyBills || Bills[0].empty()) {
            QMessageBox::warning(this, "No Orders", "No bills found for this table. Please add an order before confirming payment.");
            return;
        }

        vector<string> billNames;
        vector<int> billPrices;
        getBills(Bills, restaurantData["Menus"], billNames, billPrices);

        QString currentDate = QDateTime::currentDateTime().toString("dd-MM-yyyy");
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm");

        for (size_t i = 0; i < billNames.size(); ++i) {
            json statementItem = json::array();
            statementItem.push_back(billNames[i]);
            statementItem.push_back(billPrices[i] / (restaurantData["Menus"][i][1].get<int>()));
            statementItem.push_back(billPrices[i]);
            statementItem.push_back(currentDate.toStdString());
            statementItem.push_back(currentTime.toStdString());
            restaurantData["Statement"].push_back(statementItem);
        }

        restaurantData["Tables"][selectedTableNumber - 1]["Seats"] = 0;
        restaurantData["Tables"][selectedTableNumber - 1]["Reserved"] = "";
        restaurantData["Tables"][selectedTableNumber - 1]["Bills"] = emptyBills;

        setAllData(restaurantData);

        ui.Receipt->hide();
        ui.CheckBills->setText("Check Bills");
        on_RefreshBtn_clicked();
        return;
    }

    bool Receipt_Status = !ui.Receipt->isVisible();
    ui.Receipt->setVisible(Receipt_Status);
    ui.Receipt_TableNo->setText(QString("Table#%1").arg(selectedTableNumber));

    if (Receipt_Status) {
        if (Bills == emptyBills) {
            ui.ReceiptListName->clear();
            ui.ReceiptListPrice->clear();
            ui.Receipt_Total->setText("0 Baht");
        } else {
            vector<string> billNames;
            vector<int> billPrices;
            getBills(Bills, restaurantData["Menus"], billNames, billPrices);

            ui.ReceiptListName->clear();
            ui.ReceiptListPrice->clear();
            int totalAmount = 0;

            for (size_t i = 0; i < billNames.size(); ++i) {
                QString menuName = QString::fromStdString(billNames[i]);
                int basePrice = 0;
                for (auto& menu : restaurantData["Menus"]) {
                    if (menu[0] == billNames[i]) {
                        basePrice = menu[1].get<int>();
                        break;
                    }
                }

                int quantity = 0, extra = 0;
                if (Bills.is_array() && Bills.size() == 3) {
                    if (i < Bills[1].size()) quantity = Bills[1][i].get<int>();
                    if (i < Bills[2].size()) extra = Bills[2][i].get<int>();
                }

                QString priceDetail = QString("%1 Baht x%2").arg(basePrice).arg(quantity);
                if (extra != 0) priceDetail += QString(" + %1 Baht").arg(extra);

                ui.ReceiptListName->addItem(menuName);

                //ราคาชิดขวา
                QListWidgetItem *priceItem = new QListWidgetItem(priceDetail);
                priceItem->setTextAlignment(Qt::AlignRight);
                ui.ReceiptListPrice->addItem(priceItem);

                totalAmount += (basePrice * quantity) + extra;
            }


            ui.Receipt_Total->setText(QString::number(totalAmount) + " Baht");
        }

        ui.CheckBills->setText("Confirm Payment");
    } else {
        ui.CheckBills->setText("Check Bills");
    }
}

void RestuarantManagement::on_OpenTableBtn_clicked()
{
    qDebug() << ui.comboBox->currentIndex();
    int Index = ui.comboBox->currentIndex();
    OpenTableDialog OpenTableDialog(this,Index);
    OpenTableDialog.setModal(true);
    OpenTableDialog.setWindowTitle("Open Table");
    connect(&OpenTableDialog, &OpenTableDialog::returnValue, this, &RestuarantManagement::onTableReturnValue);
    OpenTableDialog.exec();
}

void RestuarantManagement::onTableReturnValue(const QString &data){
    json Tables;
    ::getData(Tables,"Tables");
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
    ::getData(Tables,"Tables");

    int No = GetSelectingTableNo() - 1 ;
    if(No >= 0)
    {
    if(Tables[No]["Seats"] != 0) ui.OpenTableBtn->setText("Change seat");
    else ui.OpenTableBtn->setText("Open Table");
    }
}


void RestuarantManagement::on_ReserveBtn_clicked()
{
    int tableNo = GetSelectingTableNo();
    if (ui.ReserveBtn->text() == "Unreserve") {
        removeReservation(tableNo);
        ui.ReserveBtn->setText("Reserve");
    } else {
        reserve reserve(tableNo,this,ScreensizeComboBox);
        reserve.setModal(true);
        reserve.setWindowTitle("Reserve");
        reserve.exec();
    }
    on_RefreshBtn_clicked();
}

bool RestuarantManagement::isTableReserved(int tableNo) {
    json reservations;
    ::getData(reservations, "Reservation");

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

void RestuarantManagement::on_Employee_clicked()
{
    employee employee(this,ScreensizeComboBox);
    employee.exec();
}

void RestuarantManagement::on_Stocks_clicked()
{
    StockWindow *stockWin = new StockWindow(this, ScreensizeComboBox);


    stockWin->setWindowFlags(Qt::Window);


    stockWin->show();
    stockWin->raise();
    stockWin->activateWindow();
}

void RestuarantManagement::on_EditMenu_clicked()
{
    editmenu editmenu(this,ScreensizeComboBox);
    editmenu.exec();
}


void RestuarantManagement::on_Statement_clicked()
{
    Statement stateWin(this, ScreensizeComboBox);
    stateWin.setWindowTitle("Statement");
    stateWin.exec();
}

void RestuarantManagement::on_OrderFoodBtn_clicked()
{
    int tableNo = GetSelectingTableNo();

    json restaurantData;
    getAllData(restaurantData);

    
    json &tableData = restaurantData["Tables"][tableNo - 1];

    // added 15/2/25
    if (tableData["Seats"].get<int>() == 0) {
        QMessageBox::warning(this, "No customers",
                             "No customers are seated at this table.");
        return;
    }

    json &stocks = restaurantData["Stocks"];

    OrderFoodDialog orderDialog(this);

    json menuData;
    ::getData(menuData, "Menus");
    orderDialog.loadMenu(menuData);

    if (orderDialog.exec() == QDialog::Accepted) {
        QString food = orderDialog.getSelectedFood();
        int quantity = orderDialog.getQuantity();
        int extraPrice = orderDialog.getExtraPrice();

        for (const auto &menu : menuData) {
            if (menu[0] == food.toStdString()) {
                auto ingredients = menu[3];
                auto amounts = menu[4];
                bool hasEnoughStock = true;

                for (size_t i = 0; i < ingredients.size(); ++i) {
                    std::string ingredient = ingredients[i];
                    double amountNeeded = amounts[i].get<double>() * quantity;

                    for (auto &stock : stocks) {
                        if (stock[0] == ingredient) {
                            double remainingStock = stock[1].get<double>() - amountNeeded;
                            if (remainingStock < 0) {
                                hasEnoughStock = false;
                                QMessageBox::warning(this, "Insufficient Stock",
                                                     QString("Not enough stock for %1. Remain: %2 in stock")
                                                         .arg(QString::fromStdString(ingredient))
                                                         .arg(stock[1].get<double>()));
                                break;
                            }
                        }
                    }
                    if (!hasEnoughStock) break;
                }

                if (!hasEnoughStock) {
                    return;
                }

                for (size_t i = 0; i < ingredients.size(); ++i) {
                    std::string ingredient = ingredients[i];
                    double amountNeeded = amounts[i].get<double>() * quantity;

                    for (auto &stock : stocks) {
                        if (stock[0] == ingredient) {
                            double remainingStock = stock[1].get<double>() - amountNeeded;
                            stock[1] = std::max(0.0, remainingStock);

                            if (remainingStock < 10) {
                                QMessageBox::warning(this, "Low Stock",
                                                     QString("Low stock warning: %1 is running low (remaining: %2 in stock).")
                                                         .arg(QString::fromStdString(ingredient))
                                                         .arg(remainingStock));
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }

        json &tableBills = tableData["Bills"];
        json emptyBills = json::array({json::array({""}), json::array(), json::array()});

        if (tableBills == emptyBills || (tableBills[0].size() == 1 && tableBills[0][0] == "")) {
            tableBills[0].clear();
        }

        tableBills[0].push_back(food.toStdString());
        tableBills[1].push_back(quantity);
        tableBills[2].push_back(extraPrice);

        setAllData(restaurantData);
        updateTablesStatus();
    }
}

void RestuarantManagement::on_Analysis_clicked()
{
    // json Data;
    // ::getAllData(Data);
    // if(Data["Statements"].size()<=0&&Data["Menus"].size()<=0) {
    //     showError(".json file data Statements or Menus is empty");
    //     return;}
    analysis analysis(this);
    analysis.exec();
}

void RestuarantManagement::on_backtosetup_clicked()
{
    // static int Counter = 0;
    // if (Counter > 2) {
    //     this->close();
    //     return;
    // }

    jsoncheck *jsonCheck = new jsoncheck(this);
    jsonCheck->setWindowTitle("File Configuration");
    jsonCheck->exec();

    if (!checkData()) {
        // Counter++;
        on_backtosetup_clicked();
    }
}void RestuarantManagement::resizeEvent(QResizeEvent *event){
    QSize newSize = event->size();

    int newWidth = newSize.width();
    int newHeight = newSize.height();

    //int newHeight = (newWidth * 3) / 4; // คำนวณขนาดใหม่ตามอัตราส่วน 4:3
    qDebug() << "New Width:" << newWidth << ", New Height:" << newHeight;
    //resize(newWidth, newHeight);

    //Default font size ------------------------------------------------------------------------------------------------------

    qDebug() <<"--------------------------------------------------------------------------------------------------------------";
    int perScale = newWidth*100/1024;
    double Scale = perScale/100.0;
    qDebug() <<"perScale: " << perScale <<" Scale: "<< Scale;



    // setStyleSheetFrame -----------------------------------------------------------------------------------------------------------------------

    int newSizeFont = width() / 104;  // ปรับขนาดฟอนต์ตามความกว้างของหน้าต่าง //88,104
    QString FontFrame = QString::number(newSizeFont);
    qDebug() <<"newSizeFont: " << newSizeFont;

    QFont font("Segoe UI", std::max(newSizeFont, 10));  // กำหนดฟอนต์ขั้นต่ำ 10
    ui.frame->setStyleSheet("font: 800 "+FontFrame+"pt Segoe UI;");
    qDebug() <<"newSizeFont: " << newSizeFont;
    qDebug() <<"newframeSizeFont: " << FontFrame;

    // FontheaderFont -----------------------------------------------------------------------------------------------------------------------

    QFont FontheaderFont("Sitka", max(newSizeFont*3, 25));
    ui.MyRestuarantManager->setFont(FontheaderFont);
    QFont checkFontheaderFont = ui.MyRestuarantManager->font();
    qDebug() <<"MyRestuarantManager->font(): " << checkFontheaderFont;


    // FontDatetime -----------------------------------------------------------------------------------------------------------------------

    QFont FontDatetime("SimSun", max(newSizeFont*4/3, 11));
    ui.Digital_clock->setFont(FontDatetime);
    QFont checkFontDatetime = ui.Digital_clock->font();
    qDebug() <<"label_2->font(): " <<checkFontDatetime;

    // setscaleButton-----------------------------------------------------------------------------------------------------------------------


    int sizebutton = perScale;
    qDebug() <<"BottonTable size: " <<sizebutton;

    for(int i=1;i<=Table_Count;++i){
        QString btnName = QString("Table_").append(QString::number(i));
        QPushButton *button = this->findChild<QPushButton *>(btnName);


        if(button){
            button->setFixedSize(sizebutton,sizebutton);
            //button->setFont(font);
        }
        else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
    }

    // setStyleSheetReceipt-----------------------------------------------------------------------------------------------------------------------

    int newsizefontReceipt = newSizeFont*4/5;
    qDebug() <<"newsizefontReceipt: " <<newsizefontReceipt;
    QString fReceipt = QString::number(newsizefontReceipt);

    QFont Receiptfont("Segoe UI", std::max(newsizefontReceipt, 3));
    ui.Receipt->setStyleSheet("background-color: rgb(255, 255, 255);"
                                "font: 800 "+fReceipt+"pt Segoe UI;"
                                "color: rgb(0, 0, 0);");

    // QFont R = ui.Receipt->font();
    // qDebug() <<"Receipt->font(): " <<R;
    // QSize fR = ui.Receipt->frameSize();
    //  qDebug() <<"Receipt->frameSize: " <<fR;


    qDebug() <<"--------------------------------------------------------------------------------------------------------------";

}



void RestuarantManagement::on_comboBox_activated(int index)
{
    qDebug() <<"on_comboBox_activated: " <<index;
    MoreScreensize(index,ScreensizeComboBox);
}

void RestuarantManagement::MoreScreensize(int index,int &ScreensizeComboBox){
    ScreensizeComboBox = index;
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




// void RestuarantManagement::on_Eventscreen_buttontablesize(){
//     double side = 125;

//     for(int i=1;i<=Table_Count;++i){
//         QString btnName = QString("Table_").append(QString::number(i));
//         QPushButton *button = this->findChild<QPushButton *>(btnName);


//         if(button){
//         button->setFixedSize(side,side);
//         }
//         else  qDebug()<<"Error: Button Not Found (Button Name: "<<btnName<<")";
//     }


// }
