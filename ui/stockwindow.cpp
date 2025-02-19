#include "stockwindow.h"
#include "ui_stockwindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QSet>
#include <QList>
#include <QResizeEvent>


StockWindow::StockWindow(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::StockWindow)
{
    ui->setupUi(this);
    OpenScreen(index);

    ui->tableStocks->verticalHeader()->setVisible(false);
    ui->tableStocks->horizontalHeader()->setSortIndicatorShown(false);


    ui->tableStocks->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableStocks->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableStocks->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    connect(ui->tableStocks, &QTableWidget::itemChanged, this, &StockWindow::on_tableWidget_itemChanged);
    loadfromjson();
}

StockWindow::~StockWindow()
{
    delete ui;
}



void StockWindow::loadfromjson()
{
    getAllData(stockJson);
    refreshTable();
}



void StockWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    int row = item->row();
    int col = item->column();

    if (col == 1) {  // Name Column
        stockJson["Stocks"][row][0] = item->text().toStdString();
    }
    else if (col == 2) {  // Quantity Column
        stockJson["Stocks"][row][1] = item->text().toDouble();
    }
}



void StockWindow::refreshTable()
{
    ui->tableStocks->clearContents();
    ui->tableStocks->setRowCount(stockJson["Stocks"].size());
    for (unsigned long long int i = 0; i < stockJson["Stocks"].size(); ++i) {
        // No. Column (Auto Numbering) - Read Only
        QTableWidgetItem *numItem = new QTableWidgetItem(QString::number(i + 1));
        numItem->setFlags(numItem->flags() & ~Qt::ItemIsEditable);
        ui->tableStocks->setItem(i, 0, numItem);

        // Name Column - Editable
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(stockJson["Stocks"][i][0]));
        ui->tableStocks->setItem(i, 1, nameItem);

        // Quantity Column - Editable
        QTableWidgetItem *qtyItem = new QTableWidgetItem(QString::number(stockJson["Stocks"][i][1].get<double>()));
        qtyItem->setTextAlignment(Qt::AlignRight);  // Right-align quantity
        ui->tableStocks->setItem(i, 2, qtyItem);
    }
}



void StockWindow::on_AddBtn_clicked()
{
    stockJson["Stocks"].push_back({"New Stock", 0});
    refreshTable();
}



void StockWindow::on_DeleteBtn_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableStocks->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Delete Error", "No item selected.");
        return;
    }

    QSet<int> rowsToDelete;

    for (QTableWidgetItem* item : selectedItems) {
        rowsToDelete.insert(item->row());
    }

    QList<int> rowsList = QList<int>::fromVector(rowsToDelete.values());
    std::sort(rowsList.begin(), rowsList.end(), std::greater<int>());

    for (int row : rowsList) {
        if (row >= 0 && row < stockJson["Stocks"].size()) {
            stockJson["Stocks"].erase(stockJson["Stocks"].begin() + row);
        }
    }

    refreshTable();
}



void StockWindow::savetojson()
{
    setAllData(stockJson);

    // Create a QMessageBox object
    QMessageBox Save_Message;
    Save_Message.setWindowTitle("Save Successful");
    Save_Message.setText("Data has been successfully saved to the file.");
    Save_Message.setIcon(QMessageBox::Information);
    Save_Message.setWindowFlags(Qt::Popup);  // Set as a popup window
    Save_Message.exec();  // Execute the message box
}



void StockWindow::on_SaveBtn_clicked()
{
    savetojson();
}

void StockWindow::resizeEvent(QResizeEvent *event){
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

    int defaultLabelFont = 8;
    int newintLabelFont = defaultLabelFont*perScale/100;
    QString FontLabel = QString::number(newintLabelFont);

    ui->label->setStyleSheet("font: 400 "+FontLabel+"pt Segoe UI;");
    qDebug() <<"FontLabel: "  << ui->label->font();

    //setFontTableStocks -----------------------------------------------------------------------------------------------------------------------

    int defaultHeaderFont = 12;
    int intHeaderFont = defaultHeaderFont*perScale/100;
    QFont HeaderFont("Segoe UI", intHeaderFont, QFont::Bold);
    ui->tableStocks->horizontalHeader()->setFont(HeaderFont);
    qDebug() <<"HeaderFont: "  << ui->tableStocks->horizontalHeader()->font();

    int defaultFontTableStocks = 10;
    int intFontTableStocks = defaultFontTableStocks*perScale/100;
    QFont FontStatement_Table("Segoe UI", max(intFontTableStocks, 3));
    ui->tableStocks->setFont(FontStatement_Table);
    qDebug() <<"FontTableStocks: "  << ui->tableStocks->font();

    // setFontWidget_Button -----------------------------------------------------------------------------------------------------------------------

    int defaultFontWidget_Button = 12;
    int intFontWidget_Button = defaultFontWidget_Button*perScale/100;
    QString FontWidget_Button = QString::number(intFontWidget_Button);
    ui->Widget_Button->setStyleSheet("font: 400 "+FontWidget_Button+"pt Segoe UI;");
    qDebug() <<"Widget_Button: "  << ui->Widget_Button->font();



}

void StockWindow::OpenScreen(int index){
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
