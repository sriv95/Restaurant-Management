#include "stockwindow.h"
#include "ui_stockwindow.h"
#include "header\json.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QSet>
#include <QList>
#include <QDir>

StockWindow::StockWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StockWindow)
{
    ui->setupUi(this);

    ui->tableStocks->verticalHeader()->setVisible(false);
    ui->tableStocks->horizontalHeader()->setSortIndicatorShown(false);


    qDebug()<<"Current Directory is: "<<QDir::currentPath();

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
    // Now attempt to parse the JSON content
    json data;
    getAllData(data);

    // If the "Stocks" section exists and is not empty, load the stock data
    stocks.clear();  // Clear any existing stocks before loading new data
    for (const auto& item : data["Stocks"]) {
        if (!item.is_array() || item.size() < 2) {
            // qDebug() << "Invalid stock format!";
            continue;  // Skip invalid stock entries
        }

        StockItem s;
        s.name = item[0].get<std::string>();
        s.quantity = item[1].get<double>();
        stocks.push_back(s);

        // qDebug() << "Loaded Stock: " << QString::fromStdString(s.name) << "-" << s.quantity;
    }

    refreshTable();  // Refresh the table to show the loaded stock data
}

void StockWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    int row = item->row();
    int col = item->column();

    if (col == 1) {  // Name Column
        stocks[row].name = item->text().toStdString();
    }
    else if (col == 2) {  // Quantity Column
        stocks[row].quantity = item->text().toDouble();
    }
}



void StockWindow::refreshTable()
{
    ui->tableStocks->clearContents();
    ui->tableStocks->setRowCount(stocks.size());

    for (int i = 0; i < stocks.size(); ++i) {
        // No. Column (Auto Numbering) - Read Only
        QTableWidgetItem *numItem = new QTableWidgetItem(QString::number(i + 1));
        numItem->setFlags(numItem->flags() & ~Qt::ItemIsEditable);
        ui->tableStocks->setItem(i, 0, numItem);

        // Name Column - Editable
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(stocks[i].name));
        ui->tableStocks->setItem(i, 1, nameItem);

        // Quantity Column - Editable
        QTableWidgetItem *qtyItem = new QTableWidgetItem(QString::number(stocks[i].quantity));
        qtyItem->setTextAlignment(Qt::AlignRight);  // Right-align quantity
        ui->tableStocks->setItem(i, 2, qtyItem);
    }
}




void StockWindow::on_AddBtn_clicked()
{
    StockItem newItem("New Stock",0);
    stocks.append(newItem);
    refreshTable();
}



void StockWindow::on_DeleteBtn_clicked()
{
    // Get the selected table items
    QList<QTableWidgetItem*> selectedItems = ui->tableStocks->selectedItems();

    // If no items are selected, show an error message
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Delete Error", "No item selected.");
        return;
    }

    // Create a set to store unique row indices to avoid duplicates
    QSet<int> rowsToDelete;

    // Collect the row indices from selected items
    for (QTableWidgetItem* item : selectedItems) {
        rowsToDelete.insert(item->row());
    }

    // Convert the QSet to a QList and sort it in descending order
    QList<int> rowsList = QList<int>::fromVector(rowsToDelete.values());
    std::sort(rowsList.begin(), rowsList.end(), std::greater<int>());

    // Remove rows from the stock list (in reverse order to avoid invalid indexing)
    for (int row : rowsList) {
        // Ensure that the row index is valid
        if (row >= 0 && row < stocks.size()) {
            stocks.erase(stocks.begin() + row);
        }
    }

    // Refresh the table to reflect the updated stock list
    refreshTable();

}




void StockWindow::savetojson()
{
    json allData;

    getAllData(allData);

    // Clear old Stocks data to overwrite with new data
    // qDebug() << "Clearing old Stocks data...";
    allData["Stocks"].clear();

    // qDebug() << "Saving new Stocks data...";
    // Push the current stock data
    for (const auto& stock : stocks) {
        allData["Stocks"].push_back({stock.name, stock.quantity});
    }

    // Write the updated JSON back to the file
    // qDebug() << "Opening file for writing...";
    setAllData(allData);

    // qDebug() << "Stock data successfully saved!";
    QMessageBox::information(this, "Save Successful", "Data has been successfully saved to the file.");
}



void StockWindow::on_SaveBtn_clicked()
{
    savetojson();
}

