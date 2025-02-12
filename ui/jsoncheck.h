#ifndef JSONCHECK_H
#define JSONCHECK_H

#include <QWidget>
#include <header/json.h>

namespace Ui {
class jsoncheck;
}

class jsoncheck : public QWidget
{
    Q_OBJECT

public:
    explicit jsoncheck(QWidget *parent = nullptr);
    ~jsoncheck();

private slots:
    void on_pushButton_clicked();

    void on_openBtn_clicked();

    void on_newBtn_clicked();

    void on_continueBtn_clicked();

private:
    Ui::jsoncheck *ui;
};

#endif // JSONCHECK_H
