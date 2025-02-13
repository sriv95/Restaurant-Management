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
    QFile file("data.json");

    qDebug() << "Opening file to create new data: " << file.fileName();

    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject jsonObject = createTemplate();

        // Create the QJsonDocument and write to file
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());
        file.close();

        QFileInfo fileInfo(file.fileName());
        // QMessageBox::information(this, "Created successful", "data.json has been created successfully. 😃😘💪🏿💪🏿");
        QMessageBox Created;
        Created.setText("data.json has been created successfully. 😃😘💪🏿💪🏿");
        Created.setIcon(QMessageBox::Information);
        Created.setWindowFlags(Qt::Popup);
        Created.exec();

        ui->textlabel->setText("Data created at " + fileInfo.absoluteFilePath() + " ✔️");
        ui->continueBtn->setVisible(true);

    } else {
        QMessageBox::critical(this, "Error", "Could not create the file.");
    }
}



void jsoncheck::on_openBtn_clicked()
{
    QMessageBox::information(this, "Test", "Open button clicked!");  // Show message box for testing

    QString filePath = QFileDialog::getOpenFileName(this, "Select data.json", "", "JSON Files (*.json)");

    if (!filePath.isEmpty()) {
        qDebug() << "File selected: " << filePath;

        if (QFile::exists("data.json")) {
            // If it exists, remove the existing file first
            if (!QFile::remove("data.json")) {
                ui->textlabel->setText("Error removing existing data.json.");
                return;
            } else {
                qDebug() << "Existing data.json removed successfully";
            }
        }

        if (QFile::copy(filePath, "data.json")) {
            // QMessageBox::information(this, "Opened successful", "data.json has been opened successfully. 😉💋💋");
            QMessageBox Opened;
            Opened.setText("data.json has been opened successfully. 😉💋💋");
            Opened.setIcon(QMessageBox::Information);
            Opened.setWindowFlags(Qt::Popup);
            Opened.exec();

            ui->textlabel->setText("Data found at " + filePath + " ✔️");
            ui->continueBtn->setVisible(true);

        } else {
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


void jsoncheck::on_delBtn_clicked()
{
    QFile file("data.json");

    if (file.exists()) {
        if (file.remove()) {
            ui->continueBtn->setVisible(false);
            ui->textlabel->setText("⚠️Data Not Found⚠️");
            // QMessageBox::information(this, "File Deleted", "data.json has been deleted successfully. 💀💀💀");
            QMessageBox Filedeleted;
            Filedeleted.setText("data.json has been deleted successfully. 💀💀💀");
            Filedeleted.setIcon(QMessageBox::Information);
            Filedeleted.setWindowFlags(Qt::Popup);
            Filedeleted.exec();

        } else {
            QMessageBox::critical(this, "Error", "Failed to delete data.json.");
        }
    } else {
        QMessageBox::warning(this, "File Not Found", "data.json does not exist.");
    }
}

