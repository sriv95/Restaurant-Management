#include "jsoncheck.h"
#include "ui_jsoncheck.h"
#include "mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

jsoncheck::jsoncheck(QWidget *parent)
    : QWidget(parent)
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

QJsonObject createTemplateData() {
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
        billsArray.append(QJsonArray());  // Modifiers

        tableObject["Bills"] = billsArray;
        tableObject["No"] = i;
        tableObject["Reserved"] = "";
        tableObject["Seats"] = ((i==2 || i==3) ? 4 : 0);

        tablesArray.append(tableObject);
    }

    jsonObject["Tables"] = tablesArray;

    return jsonObject;
}


void jsoncheck::on_newBtn_clicked()
{
    QFile file("data.json");

    qDebug() << "Opening file to create new data: " << file.fileName();

    if (file.open(QIODevice::WriteOnly)) {
        qDebug() << "File opened successfully for writing.";

        QJsonObject jsonObject = createTemplateData();

        // Create the QJsonDocument and write to file
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());
        file.close();

        QFileInfo fileInfo(file.fileName());
        ui->textlabel->setText("Data created at " + fileInfo.absoluteFilePath());
        ui->continueBtn->setVisible(true);

    } else {
        qDebug() << "Failed to open file for writing.";
        QMessageBox::critical(this, "Error", "Could not create the file.");
    }
}



void jsoncheck::on_openBtn_clicked()
{
    qDebug() << "Open button clicked";

    QMessageBox::information(this, "Test", "Open button clicked!");  // Show message box for testing

    QString filePath = QFileDialog::getOpenFileName(this, "Select data.json", "", "JSON Files (*.json)");

    if (!filePath.isEmpty()) {
        qDebug() << "File selected: " << filePath;

        if (QFile::exists("data.json")) {
            // If it exists, remove the existing file first
            if (!QFile::remove("data.json")) {
                qDebug() << "Failed to remove existing data.json";
                ui->textlabel->setText("Error removing existing data.json.");
                return;
            } else {
                qDebug() << "Existing data.json removed successfully";
            }
        }

        if (QFile::copy(filePath, "data.json")) {
            qDebug() << "File copied successfully";
            ui->textlabel->setText("Data found at " + filePath);
            ui->continueBtn->setVisible(true);
        } else {
            qDebug() << "Error copying file";
            ui->textlabel->setText("Error copying file.");
        }
    } else {
        qDebug() << "No file selected";
    }

}


void jsoncheck::on_continueBtn_clicked()
{
    RestuarantManagement *mainWindow = new RestuarantManagement();
    mainWindow->show();

    this->close();
}

