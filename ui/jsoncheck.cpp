#include "jsoncheck.h"
#include "ui_jsoncheck.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <header/json.h>
#include <QSettings>

using namespace std;


jsoncheck::jsoncheck(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::jsoncheck)
{
    ui->setupUi(this);
    ui->textlabel->setAlignment(Qt::AlignCenter);
    ui->textlabel->setWordWrap(true);
    ui->continueBtn->setVisible(false);

    QSettings settings("MyApp", "RestaurantSystem");
    QString lastUsedPath = settings.value("jsonFilePath", "").toString();

    if (!lastUsedPath.isEmpty()) {
        QFileInfo fileInfo(lastUsedPath);
        if (fileInfo.exists()) {
            ui->textlabel->setText("✅Data found at " + fileInfo.absoluteFilePath() + "✅");
            ui->continueBtn->setVisible(true);
        } else {
            ui->textlabel->setText("⚠️Data Not Found⚠️");
        }
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
    QString filePath = QDir::currentPath() + "/data.json";

    QSettings settings("MyApp", "RestaurantSystem");
    settings.setValue("jsonFilePath", filePath);

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
        QSettings settings("MyApp", "RestaurantSystem");
        settings.setValue("jsonFilePath", filePath);

        qDebug() << "File selected: " << filePath;

        updateFilePath(filePath.toStdString());
        // currentFilePath = filePath;

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

