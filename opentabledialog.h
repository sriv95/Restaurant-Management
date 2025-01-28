#ifndef OPENTABLEDIALOG_H
#define OPENTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class OpenTableDialog;
}

class OpenTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenTableDialog(QWidget *parent = nullptr);
    ~OpenTableDialog();

private slots:
    void on_numBtn_clicked();

private:
    Ui::OpenTableDialog *ui;
};

#endif // OPENTABLEDIALOG_H
