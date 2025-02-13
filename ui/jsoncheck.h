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

    void on_openBtn_clicked();

    void on_newBtn_clicked();

    void on_continueBtn_clicked();


    void on_delBtn_clicked();

private:
    Ui::jsoncheck *ui;

};

#endif // JSONCHECK_H
