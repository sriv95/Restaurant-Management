#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QDialog>

namespace Ui {
class analysis;
}

class analysis : public QDialog
{
    Q_OBJECT

public:
    explicit analysis(QWidget *parent = nullptr);
    ~analysis();

private:
    Ui::analysis *ui;

    void Show_Chart();
};

#endif // ANALYSIS_H
