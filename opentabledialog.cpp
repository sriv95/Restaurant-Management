#include "opentabledialog.h"
#include "ui_opentabledialog.h"

OpenTableDialog::OpenTableDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenTableDialog)
{
    ui->setupUi(this);

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
    emit returnValue(ui->TextInput->toPlainText());
    this->close();
}

void OpenTableDialog::on_buttonBox_rejected()
{
    this->close();
}
