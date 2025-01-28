#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "opentabledialog.h"

class RestuarantManagement : public QMainWindow
{
    Q_OBJECT

public:
    RestuarantManagement(QWidget *parent = nullptr);
    ~RestuarantManagement();

private slots:

    void on_RefreshBtn_clicked();

    void SetSelectingTable(QString);

    int GetSelectingTableNo();

    void on_TableBtn_clicked();

    void on_CheckBills_clicked();

    void setMainBtnVisible(bool);

    void on_OpenTableBtn_clicked();

private:
    Ui::RestuarantManagementClass ui;
};
