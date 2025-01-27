#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class RestuarantManagement : public QMainWindow
{
    Q_OBJECT

public:
    RestuarantManagement(QWidget *parent = nullptr);
    ~RestuarantManagement();

private:
    Ui::RestuarantManagementClass ui;
};
