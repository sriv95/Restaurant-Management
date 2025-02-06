#ifndef STOCKWINDOW_H
#define STOCKWINDOW_H

#include <QWidget>
#include <QVector>
#include <QTableWidgetItem>
#include "header\json.h"
#include <string>

using namespace std;

namespace Ui {
class StockWindow;
}


struct StockItem {
    std::string name;
    double quantity;


    StockItem() : name(""), quantity(0.0) {}
    StockItem(const std::string& n, double q) : name(n), quantity(q) {}
};


class StockWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StockWindow(QWidget *parent = nullptr);
    ~StockWindow();

private slots:

    void on_AddBtn_clicked();

    void on_DeleteBtn_clicked();

    void on_SaveBtn_clicked();




private:
    Ui::StockWindow *ui;
    QVector<StockItem> stocks;
    json stockJson;

    void refreshTable();
    void loadfromjson();
    void savetojson();
    void on_tableWidget_itemChanged(QTableWidgetItem *item);


};

#endif // STOCKWINDOW_H
