#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void RunGenerate();

    void create_empty_data_json();

    void Employee();

    void Menus(vector<vector<string>> &Dished_Ingredients , vector<vector<string>> &Drinks_Ingredients);

    void Reservation();

    void Statement();

    void Stocks(vector<vector<string>> Stocks_Dished_Ingredients , vector<vector<string>> Stocks_Drinks_Ingredients);

    void Menus_and_Stocks();

};
#endif // MAINWINDOW_H
