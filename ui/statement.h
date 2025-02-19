#ifndef STATEMENT_H
#define STATEMENT_H

#include <QDialog>

namespace Ui {
class Statement;
}

class Statement : public QDialog
{
    Q_OBJECT

public:
    explicit Statement(QWidget *parent = nullptr, int Index = 2);
    ~Statement();

private slots:

    void resizeEvent(QResizeEvent *event) override ;

    void OpenScreen(int index);

private:
    Ui::Statement *ui;
    void loadData();
};

#endif // STATEMENT_H
