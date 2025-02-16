#include "opentabledialog.h"
#include "ui_opentabledialog.h"
#include <QMessageBox>

OpenTableDialog::OpenTableDialog(QWidget *parent,int index)
    : QDialog(parent)
    , ui(new Ui::OpenTableDialog)
{
    ui->setupUi(this);
    OpenScreen(index);

    this->setWindowTitle("Opentable");
    //connect(&RestuarantManagement, &RestuarantManagement::returnIndexscreen, this, &OpenTableDialog::resizeEvent);
    connect(ui->one, &QPushButton::clicked, this, &OpenTableDialog::on_numBtn_clicked);
    connect(ui->two, &QPushButton::clicked, this, &OpenTableDialog::on_numBtn_clicked);
    connect(ui->three, &QPushButton::clicked, this, &OpenTableDialog::on_numBtn_clicked);
    connect(ui->four, &QPushButton::clicked, this, &OpenTableDialog::on_numBtn_clicked);
}

OpenTableDialog::~OpenTableDialog()
{
    delete ui;
}

void OpenTableDialog::on_numBtn_clicked()
{
    QPushButton *buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonText = buttonSender->text();
    ui->TextInput->setPlainText(buttonText);
}

void OpenTableDialog::on_buttonBox_accepted()
{
    bool testvalue;
    int seat = ui->TextInput->toPlainText().trimmed().toInt(&testvalue);
    qDebug()<< testvalue;
    if(testvalue == true and seat > 0)
    {
        emit returnValue(ui->TextInput->toPlainText());
        this->close();
    }
    else
    {
        QMessageBox::about(this , "warning" , "Invalid data type. It should be a positive integer." );

    }
}

void OpenTableDialog::on_buttonBox_rejected() //cansel
{
    this->close();
}

void OpenTableDialog::resizeEvent(QResizeEvent *event){
    QSize newSize = event->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();
    qDebug() << "-------------------OpenTableDialog-------------------";
    qDebug() << "New Width:" << newWidth << ", New Height:" << newHeight;

    // setStyleSheetFrame -----------------------------------------------------------------------------------------------------------------------

    int newSizeFont = newWidth/20;
    QString FontOpenTableDialog = QString::number(newSizeFont);
    qDebug() <<"newSizeFont: " << newSizeFont;

    QDialog OpenTableDialog;
    ui->frame->setStyleSheet("font: 400 "+FontOpenTableDialog+"pt Segoe UI;");

    // QFont checkOpenTableDialogFont = OpenTableDialog.font();
    // qDebug() <<"OpenTableDialog->font(): " << checkOpenTableDialogFont;

    // setFrontbuttonBox -----------------------------------------------------------------------------------------------------------------------

    int intFrontbuttonBoxsize = newWidth/40;
    QString FrontbuttonBoxsize = QString::number(intFrontbuttonBoxsize);
    ui->buttonBox->setStyleSheet("font: 400 "+FrontbuttonBoxsize+"pt Segoe UI;");
}

void OpenTableDialog::OpenScreen(int index){
    int w = 320, h =240;

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
