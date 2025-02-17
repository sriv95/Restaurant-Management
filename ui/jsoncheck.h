#ifndef JSONCHECK_H
#define JSONCHECK_H

#include <QDialog>
#include <header/json.h>

namespace Ui {
class jsoncheck;
}

class jsoncheck : public QDialog
{
    Q_OBJECT

public:
    explicit jsoncheck(QWidget *parent = nullptr);
    ~jsoncheck();

private slots:

    void on_openBtn_clicked();

    void on_newBtn_clicked();

    void on_continueBtn_clicked();


    void on_cancelBtn_clicked();

private:
    Ui::jsoncheck *ui;


};

#endif // JSONCHECK_H
