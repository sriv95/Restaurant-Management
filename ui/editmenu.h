#ifndef EDITMENU_H
#define EDITMENU_H

#include "qtablewidget.h"
#include <QDialog>

namespace Ui {
class editmenu;
}

class editmenu : public QDialog
{
    Q_OBJECT

public:
    explicit editmenu(QWidget *parent = nullptr, int index = 2);
    ~editmenu();

private slots:
    void on_RefreshBtn_clicked(bool);

    void on_AddMenuBtn_clicked();

    void on_DelMenuBtn_clicked();

    void on_AddIngBtn_clicked();

    void RefreshIng(int);

    void on_SaveMenuBtn_clicked();

    void onMenuTableItemChanged(QTableWidgetItem *);

    void resizeEvent(QResizeEvent *event) override ;

    void OpenScreen(int index);

    void resizeFont(double perScale);

private:
    Ui::editmenu *ui;
};

#endif // EDITMENU_H
