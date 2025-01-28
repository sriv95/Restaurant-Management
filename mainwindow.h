#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

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

    void on_RBtn_clicked();

private:
    Ui::RestuarantManagementClass ui;
};
