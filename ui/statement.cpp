#include "statement.h"
#include "ui_statement.h"
#include "header/json.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QResizeEvent>


Statement::Statement(QWidget *parent, int index)
    : QDialog(parent)
    , ui(new Ui::Statement)
{
    ui->setupUi(this);
    OpenScreen(index);

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

    QString selectedMonth = QString::number(ui->Month_ComboBox->currentIndex()+1);
    QString selectedYear = ui->Year_Input->text();
    int IntMonth=ui->Month_ComboBox->currentIndex()+1;
    if(IntMonth < 10)selectedMonth="0"+QString::number(IntMonth);
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

    ui->Total_Income->setText(QString::number(totalIncome, 'f', 2) + "฿");
    ui->Total_Expense->setText(QString::number(totalExpense, 'f', 2) + "฿");
    ui->Net_Balance->setText(QString::number(netProfit, 'f', 2) + "฿");
}

void Statement::resizeEvent(QResizeEvent *event){
    QSize newSize = event->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();
    qDebug() << "-------------------Statement-------------------";
    qDebug() << "New Width:" << newWidth << ", New Height:" << newHeight;

    int defaultWidth = 1024;
    int perScale = newWidth*100/defaultWidth;
    double Scale = perScale/100.0;
    qDebug() <<"perScale: " << perScale <<" Scale: "<< Scale;

    // setFontLabel ---------------------------------------------------------------------------------------------------------------

    int defaultLabelFont = 16;
    int newintLabelFont = defaultLabelFont*perScale/100;
    QString FontLabel = QString::number(newintLabelFont);

    ui->label->setStyleSheet("font: 400 "+FontLabel+"pt Segoe UI;");
    qDebug() <<"FontLabel: "  << ui->label->font();


    // setFrontWidget_Selection -----------------------------------------------------------------------------------------------------------------------

    int defaultFrontWidget_Selection = 10;
    int intFrontWidget_Selection = defaultFrontWidget_Selection*perScale/100;
    QFont FrontWidget_Selection("Segoe UI", max(intFrontWidget_Selection, 3));
    ui->Widget_Selection->setFont(FrontWidget_Selection);
    qDebug() <<"FrontWidget_Selection: "  << ui->Widget_Selection->font();

    //setFontStatement_Table -----------------------------------------------------------------------------------------------------------------------

    int defaultHeaderFont = 12;
    int intHeaderFont = defaultHeaderFont*perScale/100;
    QFont HeaderFont("Segoe UI", intHeaderFont, QFont::Bold);
    ui->Statement_Table->horizontalHeader()->setFont(HeaderFont);
    qDebug() <<"HeaderFont: "  << ui->Statement_Table->horizontalHeader()->font();

    int defaultFontStatement_Table = 10;
    int intTableFont = defaultFontStatement_Table*perScale/100;
    QFont FontStatement_Table("Segoe UI", max(intTableFont, 3));
    ui->Statement_Table->setFont(FontStatement_Table);
    qDebug() <<"FontStatement_Table: "  << ui->Statement_Table->font();

    // setFontWidget_Total -----------------------------------------------------------------------------------------------------------------------

    int defaultFontWidget_Total = 12;
    int intFontWidget_Total = defaultFontWidget_Total*perScale/100;
    QString Fontwidget_Total = QString::number(intFontWidget_Total);
    ui->Widget_Total->setStyleSheet("font: 400 "+Fontwidget_Total+"pt Segoe UI;");
    qDebug() <<"Widget_Total: "  << ui->Widget_Total->font();



}

void Statement::OpenScreen(int index){
    int w = 1024, h = 768;

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
