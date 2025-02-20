#ifndef ORDERSTOCK_H
#define ORDERSTOCK_H

#include <QDialog>
#include "header/json.h"

namespace Ui {
class OrderStock;
}

class OrderStock : public QDialog
{
    Q_OBJECT

public:
    explicit OrderStock(QWidget *parent = nullptr);
    ~OrderStock();

private:
    Ui::OrderStock *ui;
};

#endif // ORDERSTOCK_H
