#ifndef EDITMENU_H
#define EDITMENU_H

#include <QDialog>

namespace Ui {
class editmenu;
}

class editmenu : public QDialog
{
    Q_OBJECT

public:
    explicit editmenu(QWidget *parent = nullptr);
    ~editmenu();

private slots:
    void on_RefreshBtn_clicked();

private:
    Ui::editmenu *ui;
};

#endif // EDITMENU_H
