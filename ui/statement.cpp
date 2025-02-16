#include "statement.h"
#include "ui_statement.h"
#include "header/json.h"
#include <QMessageBox>
#include <QHeaderView>


Statement::Statement(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Statement)
{
    ui->setupUi(this);

    ui->Statement_Table->verticalHeader()->setVisible(false);
    ui->Statement_Table->horizontalHeader()->setSortIndicatorShown(false);
    ui->Statement_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    connect(ui->Month_ComboBox, &QComboBox::currentTextChanged, this, &Statement::loadData);
    connect(ui->Year_Input, &QLineEdit::textChanged, this, &Statement::loadData);

    loadData();
}

Statement::~Statement()
{
    delete ui;
}

void Statement::loadData()
{
    json data;
    getAllData(data);

    if (!data.contains("Statement")) {
        QMessageBox::warning(this, "Error", "No Statement data found in JSON");
        return;
    }

    QString selectedMonth = ui->Month_ComboBox->currentText();
    QString selectedYear = ui->Year_Input->text();

    if (selectedMonth.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a month");
        return;
    }

    json statementData = data["Statement"];
    ui->Statement_Table->setRowCount(0);
    double totalIncome = 0, totalExpense = 0;
    int row = 0;

    for (const auto& entry : statementData) {
        if (entry.size() < 5) continue;

        QString name = QString::fromStdString(entry[0]);
        int quantity = entry[1];
        double price = entry[2];
        QString date = QString::fromStdString(entry[3]);
        QString time = QString::fromStdString(entry[4]);

        QStringList dateParts = date.split("-");
        if (dateParts.size() != 3) continue;

        //qDebug() << "Checking Date:" << date << "Month in JSON:" << dateParts[1] << "Selected Month:" << selectedMonth;

        if (dateParts[1] != selectedMonth) continue;
        if (!selectedYear.isEmpty() && dateParts[2] != selectedYear) continue;

        ui->Statement_Table->insertRow(row);

        ui->Statement_Table->setItem(row, 0, new QTableWidgetItem(date));
        ui->Statement_Table->setItem(row, 1, new QTableWidgetItem(time));
        ui->Statement_Table->setItem(row, 2, new QTableWidgetItem(name));
        ui->Statement_Table->setItem(row, 3, new QTableWidgetItem(QString::number(quantity)));
        QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(price, 'f', 2) + "฿");

        if (price > 0) {
            priceItem->setBackground(QColor(60, 179, 113)); // Green for income
            totalIncome += price;
        } else {
            priceItem->setBackground(QColor(255, 99, 71)); // Red for expense
            totalExpense += -price;
        }

        ui->Statement_Table->setItem(row, 4, priceItem);
        row++;
    }

    double netProfit = totalIncome - totalExpense;

    //qDebug() << "Updating Summary - Income:" << totalIncome << "Expense:" << totalExpense << "Net Balance:" << netProfit;

    ui->Total_Income->setText(QString::number(totalIncome, 'f', 2) + "฿");
    ui->Total_Expense->setText(QString::number(totalExpense, 'f', 2) + "฿");
    ui->Net_Balance->setText(QString::number(netProfit, 'f', 2) + "฿");
}
