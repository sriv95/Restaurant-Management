#include "jsoncheck.h"
#include "ui_jsoncheck.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <header/json.h>

using namespace std;

const string Keys[] = {"Employee", "Menus", "Reservation", "Statement", "Stocks", "Tables"};
jsoncheck::jsoncheck(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::jsoncheck)
{
    ui->setupUi(this);
    ui->textlabel->setAlignment(Qt::AlignCenter);
    ui->textlabel->setWordWrap(true);
    ui->continueBtn->setVisible(false);

    QFileInfo fileInfo("data.json");
    if (fileInfo.exists()) {
        ui->textlabel->setText("✅Data found at " + fileInfo.absoluteFilePath() + "✅");
        ui->continueBtn->setVisible(true);
    } else {
        ui->textlabel->setText("⚠️Data Not Found⚠️");
    }
}



jsoncheck::~jsoncheck()
{
    delete ui;
}

QJsonObject createTemplate() {
    QJsonObject jsonObject;
    jsonObject["Employee"] = QJsonArray();
    jsonObject["Menus"] = QJsonArray();
    jsonObject["Reservation"] = QJsonArray();
    jsonObject["Statement"] = QJsonArray();
    jsonObject["Stocks"] = QJsonArray();

    QJsonArray tablesArray;
    for (int i=1; i <= 9; ++i) {
        QJsonObject tableObject;
        QJsonArray billsArray;
        QJsonArray menu;

        menu.append("");
        billsArray.append(menu);  // Menu items
        billsArray.append(QJsonArray());  // Quantities
        billsArray.append(QJsonArray());

        tableObject["Bills"] = billsArray;
        tableObject["No"] = i;
        tableObject["Reserved"] = "";
        tableObject["Seats"] = 0;

        tablesArray.append(tableObject);
    }

    jsonObject["Tables"] = tablesArray;

    return jsonObject;
}


void jsoncheck::on_newBtn_clicked()
{
    newData();


        QFileInfo fileInfo(currentFilePath);

        QMessageBox Created;
        Created.setText("data.json has been created successfully. 😃😘💪🏿💪🏿");
        Created.setIcon(QMessageBox::Information);
        Created.setWindowFlags(Qt::Popup);
        Created.exec();

        ui->textlabel->setText("Data created at " + fileInfo.absoluteFilePath() + " ✔️");
        ui->continueBtn->setVisible(true);

}



void jsoncheck::on_openBtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select data.json", "", "JSON Files (*.json)");

    if (!filePath.isEmpty()) {
        qDebug() << "File selected: " << filePath;

        updateFilePath(filePath.toStdString());
        currentFilePath = filePath;

        QMessageBox Opened;
        Opened.setText("data.json has been opened successfully. 😉💋💋");
        Opened.setIcon(QMessageBox::Information);
        Opened.setWindowFlags(Qt::Popup);
        Opened.exec();

        ui->textlabel->setText("Data found at " + filePath + " ✔️");
        ui->continueBtn->setVisible(true);
        } else {
        qDebug() << "No file selected";
    }

}


void jsoncheck::on_continueBtn_clicked()
{
    this->close();
}

