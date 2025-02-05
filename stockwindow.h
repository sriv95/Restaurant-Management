#ifndef STOCKWINDOW_H
#define STOCKWINDOW_H

#include <QWidget>

namespace Ui {
class StockWindow;
}

class StockWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StockWindow(QWidget *parent = nullptr);
    ~StockWindow();

private:
    Ui::StockWindow *ui;
};

#endif // STOCKWINDOW_H
