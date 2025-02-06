#include "mainwindow.h"
#include <header/json.h>
#include <opentabledialog.h>
#include <reserve.h>
#include <QFile>
#include <QTextStream>
#include "header/json.h"
#include "header/tables.h"
#include <fstream>
#include <QDateTime>


using json = nlohmann::json;
json j;
using namespace std;

void RestuarantManagement::loadData()
{
    QFile file("data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open data.json";
        return;
    }

    QTextStream in(&file);
    QString jsonData = in.readAll();
    file.close();

    try {
        j = json::parse(jsonData.toStdString()); // แปลง QString -> JSON
        qDebug() << "Data loaded successfully!";
    } catch (json::parse_error &e) {
        qDebug() << "JSON parse error: " << e.what();
    }
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
    loadData();
    connect(ui.Table_1, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_2, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_3, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_4, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_5, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_6, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_7, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_8, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);
    connect(ui.Table_9, &QPushButton::clicked, this, &RestuarantManagement::on_TableBtn_clicked);

    ui.Receipt->hide();
    setMainBtnVisible(false);
}

RestuarantManagement::~RestuarantManagement()
{}

int RestuarantManagement::GetSelectingTableNo(){
    return ui.SelectingTable->text().toInt();
}

void RestuarantManagement::SetSelectingTable(QString no)
{
    // ถ้าใบเสร็จยังเปิดอยู่และยังไม่ได้กด Confirm Payment
    // (เช็คจากปุ่ม ui.CheckBills->text() == "Confirm Payment")
    if (ui.Receipt->isVisible() && ui.CheckBills->text() == "Confirm Payment") {
        // ซ่อนใบเสร็จ
        ui.Receipt->hide();
        // เปลี่ยนข้อความปุ่มกลับเป็น "Check Bills"
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
    // ถ้าใบเสร็จยังเปิดอยู่และยังไม่ได้กด Confirm Payment
    if (ui.Receipt->isVisible() && ui.CheckBills->text() == "Confirm Payment") {
        // ยกเลิกการดูบิล
        ui.Receipt->hide();
        ui.CheckBills->setText("Check Bills");
    }


    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    QString buttonName = buttonSender->objectName();
    QString table_no = buttonName.split("_").last();

    SetSelectingTable(table_no);

    // Refresh the bills for the new selected table
    on_CheckBills_clicked();
}


void RestuarantManagement::on_RefreshBtn_clicked()
{
    
}

void RestuarantManagement::on_CheckBills_clicked()
{
    ui.Receipt_DateTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    int selectedTableNumber = GetSelectingTableNo();
    if (selectedTableNumber == 0) {
        ui.Receipt->hide();
        return;
    }

    // ส่วนสำหรับ "Confirm Payment"
    if (ui.CheckBills->text() == "Confirm Payment") {
        // ดึง Bills ของโต๊ะที่เลือก
        json Bills = j["Tables"][selectedTableNumber - 1]["Bills"];

        // ดึงข้อมูลจาก Bills
        vector<string> billNames;
        vector<int> billPrices;
        getBills(Bills, j["Menus"], billNames, billPrices);

        // สำหรับแต่ละรายการ บันทึกลงใน Statement
        for (size_t i = 0; i < billNames.size(); ++i) {
            // สร้างรายการ Statement ในรูปแบบ ["MenuName", quantity, finalPrice]
            json statementItem = json::array();
            statementItem.push_back(billNames[i]);
            statementItem.push_back(billPrices[i] / (j["Menus"][i][1].get<int>())); // คำนวณ quantity
            statementItem.push_back(billPrices[i]);
            j["Statement"].push_back(statementItem);
        }

        // รีเซ็ตข้อมูลโต๊ะ (Seats, Reserved, Bills)
        j["Tables"][selectedTableNumber - 1]["Seats"] = 0;
        j["Tables"][selectedTableNumber - 1]["Reserved"] = "";
        j["Tables"][selectedTableNumber - 1]["Bills"] = json::array({
            json::array({""}),
            json::array(),
            json::array()
        });

        // บันทึกข้อมูลลงในไฟล์ data.json
        std::ofstream file("data.json");
        if (file.is_open()) {
            file << j.dump(4); // indent 4 เพื่อให้อ่านง่าย
            file.close();
        }

        // ซ่อน Receipt และเปลี่ยนข้อความปุ่มกลับเป็น "Check Bills"
        ui.Receipt->hide();
        ui.CheckBills->setText("Check Bills");
        return;
    }

    // สลับสถานะการแสดง Receipt
    bool isReceiptVisible = !ui.Receipt->isVisible();
    ui.Receipt->setVisible(isReceiptVisible);
    ui.Receipt_TableNo->setText(QString("Table#%1").arg(selectedTableNumber));

    if (isReceiptVisible) {
        json Bills = j["Tables"][selectedTableNumber - 1]["Bills"];
        // กำหนดค่า Bills เริ่มต้น (empty)
        json emptyBills = json::array({
            json::array({""}),
            json::array(),
            json::array()
        });

        if (Bills == emptyBills) {
            // ถ้า Bills เป็นค่าเริ่มต้น ให้แสดง Receipt ว่าง
            ui.ReceiptListName->clear();
            ui.ReceiptListPrice->clear();
            ui.Receipt_Total->setText("0 บาท");
        } else {
            // ดึงข้อมูล Bills ออกมา
            vector<string> billNames;
            vector<int> billPrices;
            getBills(Bills, j["Menus"], billNames, billPrices);

            ui.ReceiptListName->clear();
            ui.ReceiptListPrice->clear();
            int totalAmount = 0;

            for (size_t i = 0; i < billNames.size(); ++i) {
                QString menuName = QString::fromStdString(billNames[i]);

                // หา basePrice จาก j["Menus"]
                int basePrice = 0;
                for (auto& menu : j["Menus"]) {
                    if (menu[0] == billNames[i]) {
                        basePrice = menu[1].get<int>();
                        break;
                    }
                }

                // ดึง quantity และ extra จาก Bills
                int quantity = 0;
                int extra = 0;
                if (Bills.is_array() && Bills.size() >= 3) {
                    const auto& quantities = Bills[1];
                    const auto& extras = Bills[2];
                    if (quantities.is_array() && i < quantities.size()) {
                        quantity = quantities[i].get<int>();
                    }
                    if (extras.is_array() && i < extras.size()) {
                        extra = extras[i].get<int>();
                    }
                }

                // สร้างข้อความรายละเอียด เช่น "50 บาท x1 + 10 บาท" (ถ้า extra ไม่เท่ากับ 0)
                QString priceDetail = QString::number(basePrice) + " บาท";
                priceDetail += " x" + QString::number(quantity);
                if (extra != 0) {
                    priceDetail += " + " + QString::number(extra) + " บาท";
                }

                ui.ReceiptListName->addItem(menuName);
                ui.ReceiptListPrice->addItem(priceDetail);
                totalAmount += (basePrice * quantity) + extra;
            }
            ui.Receipt_Total->setText(QString::number(totalAmount) + " บาท");
        }

        // เมื่อแสดง Receipt เปลี่ยนข้อความปุ่มเป็น "Confirm Payment"
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
