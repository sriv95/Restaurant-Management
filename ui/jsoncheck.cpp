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


jsoncheck::jsoncheck(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::jsoncheck)
{
    ui->setupUi(this);
    ui->textlabel->setAlignment(Qt::AlignCenter);
    ui->textlabel->setWordWrap(true);
    ui->continueBtn->setVisible(false);

        QString openFilePath = "data.json";
        QFileInfo openFileInfo(openFilePath);

        if (openFileInfo.exists()) {
            ui->textlabel->setText("✅ Data found at " + openFileInfo.absoluteFilePath() + "✅");
            ui->continueBtn->setVisible(true);
        } else {
            ui->textlabel->setText("⚠️Data Not Found⚠️");
        }

}



jsoncheck::~jsoncheck()
{
    delete ui;
}


void jsoncheck::on_newBtn_clicked()
{
    newData();
    currentFilePath = "data.json";

        QFileInfo fileInfo("data.json");

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

        ui->textlabel->setText("Data opened at " + filePath + " ✔️");
        ui->continueBtn->setVisible(true);
        } else {
        qDebug() << "No file selected";
    }

}


void jsoncheck::on_continueBtn_clicked()
{
    this->close();
}

