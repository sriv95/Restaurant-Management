#ifndef RESERVE_H
#define RESERVE_H

#include <QDialog>

namespace Ui {
class reserve;
}

class reserve : public QDialog
{
    Q_OBJECT

public:
    explicit reserve(QWidget *parent = nullptr);
    ~reserve();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::reserve *ui;
};

#endif // RESERVE_H
