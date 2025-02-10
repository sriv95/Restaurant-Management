#ifndef JSONCHECK_H
#define JSONCHECK_H

#include <QWidget>

namespace Ui {
class jsoncheck;
}

class jsoncheck : public QWidget
{
    Q_OBJECT

public:
    explicit jsoncheck(QWidget *parent = nullptr);
    ~jsoncheck();

private:
    Ui::jsoncheck *ui;
};

#endif // JSONCHECK_H
