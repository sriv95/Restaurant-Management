#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QDialog>

namespace Ui {
class employee;
}

class employee : public QDialog
{
    Q_OBJECT

public:
    explicit employee(QWidget *parent = nullptr);
    ~employee();

private slots:

    void on_Refresh_Button_clicked();

    void Refresh_TableData();

private:
    Ui::employee *ui;
};

#endif // EMPLOYEE_H
