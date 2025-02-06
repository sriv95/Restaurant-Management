#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <opentabledialog.h>
#include "header/json.h"
using json = nlohmann::json;


class RestuarantManagement : public QMainWindow
{
    Q_OBJECT

public:
    RestuarantManagement(QWidget *parent = nullptr);
    ~RestuarantManagement();

    void on_TableBtn_clicked();

private slots:

    void on_RefreshBtn_clicked();

    void SetSelectingTable(QString);

    int GetSelectingTableNo();

    void on_CheckBills_clicked();

    void setMainBtnVisible(bool);

    void on_OpenTableBtn_clicked();

    void onTableReturnValue(const QString &);

    void on_ReserveBtn_clicked();

    // void onReserveReturnValue(const QString &);

private:
    json j;
    void loadData();
    Ui::RestuarantManagementClass ui;
    OpenTableDialog *openTableDialog; // Declare OpenTableDialog as a member
};
