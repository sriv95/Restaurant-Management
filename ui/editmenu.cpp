#include "editmenu.h"
#include "qpushbutton.h"
#include "ui_editmenu.h"

QTableWidget *menutable; //define tw

editmenu::editmenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editmenu)
{
    ui->setupUi(this);

    this->setWindowTitle("Edit Menu");

    menutable = ui->tableWidget; //Set Variable

    menutable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    menutable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    menutable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    menutable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // for (int i = 0; i < 3; ++i) {
    //     int rowPosition = menutable->rowCount();
    //     menutable->insertRow(rowPosition);
    //     QPushButton *pb = new QPushButton("Edit Ingredients...");
    //     menutable->setCellWidget(rowPosition,3,pb);
    // }
}

editmenu::~editmenu()
{
    delete ui;
}

