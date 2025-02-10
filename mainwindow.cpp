#include "mainwindow.h"
#include "header/json.h"
#include "opentabledialog.h"
#include "reserve.h"
#include "func/tables.h"
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>

json restaurantData;


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
}

RestuarantManagement::~RestuarantManagement()
{}

void RestuarantManagement::on_RefreshBtn_clicked()
{}

int RestuarantManagement::GetSelectingTableNo(){
    return ui.SelectingTable->text().toInt();
}

void RestuarantManagement::SetSelectingTable(QString no)
{
    if (ui.Receipt->isVisible() && ui.CheckBills->text() == "Confirm Payment") {
        ui.Receipt->hide();
        ui.CheckBills->setText("Check Bills");
    }

    if (ui.SelectingTable->text() != no) {
        ui.SelectingTable->setText(QString(no));
        setMainBtnVisible(true);
        ui.Receipt->show();
    } else {
        ui.SelectingTable->setText(QString('0'));
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
    OpenTableDialog OpenTableDialog;
    OpenTableDialog.setModal(true);
    connect(&OpenTableDialog, &OpenTableDialog::returnValue, this, &RestuarantManagement::onTableReturnValue);
    OpenTableDialog.exec();
}

void RestuarantManagement::onTableReturnValue(const QString &data){
    QString x = data; //handle unused variable
    qDebug("x");
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
