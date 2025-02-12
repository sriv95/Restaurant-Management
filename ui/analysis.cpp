#include "analysis.h"
#include "ui_analysis.h"
#include <QtCore>
#include <QtGui>
#include <QtCharts/QtCharts>
#include <QtCharts/QLineSeries>
#include <vector>
#include <algorithm>

using namespace std;

analysis::analysis(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::analysis)
{
    ui->setupUi(this);
    this->setWindowTitle("Analysis");
    Show_Chart();
}

analysis::~analysis()
{
    delete ui;
}

void analysis::Show_Chart()
{
    // Create a line series
    vector<double> xData = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99
    };

    vector<double> income_Data = {
        49.65, 94.14, 58.45, 37.14, 97.52, 48.87, 16.71, 60.95, 69.91, 40.50,
        90.08, 94.13, 1.40, 57.38, 85.38, 72.71, 40.01, 53.55, 20.17, 40.59,
        77.12, 51.25, 70.13, 48.69, 25.31, 86.01, 18.15, 68.85, 28.65, 95.12,
        20.92, 20.48, 28.81, 22.47, 16.25, 24.43, 41.27, 18.38, 74.56, 46.83,
        69.89, 65.50, 71.93, 44.21, 59.77, 34.14, 31.72, 45.81, 22.46, 2.38,
        64.82, 72.79, 29.42, 27.82, 11.22, 89.26, 20.31, 44.68, 51.82, 20.21,
        50.99, 34.99, 1.94, 33.17, 40.32, 42.25, 45.73, 30.77, 97.06, 88.84,
        55.55, 29.12, 4.97, 14.30, 85.72, 72.85, 3.99, 54.66, 4.25, 53.33,
        58.85, 43.26, 40.90, 49.15, 10.60, 45.81, 22.68, 73.33, 75.03, 83.80,
        28.07, 95.62, 62.94, 3.70, 6.87, 49.91, 52.51, 72.57, 48.97, 94.17
    };

    vector<double> expenses_Data = {
        57.28, 95.19, 69.36, 30.78, 87.95, 82.21, 95.16, 57.67, 22.26, 77.31,
        74.33, 28.26, 82.85, 38.99, 96.03, 1.88, 8.00, 22.49, 34.76, 80.89,
        21.95, 42.89, 91.63, 73.43, 58.65, 54.36, 37.38, 74.77, 16.94, 63.66,
        3.83, 76.04, 87.86, 87.26, 58.16, 44.26, 92.22, 24.98, 53.92, 59.65,
        11.17, 62.42, 90.16, 96.51, 34.03, 77.11, 74.39, 7.31, 1.74, 76.02,
        19.80, 13.69, 5.32, 41.79, 13.22, 2.27, 25.81, 2.13, 97.50, 36.02,
        63.79, 1.53, 50.90, 46.42, 36.40, 71.01, 75.36, 11.28, 45.98, 81.28,
        16.94, 92.03, 49.71, 64.83, 17.45, 58.55, 58.49, 12.29, 89.44, 11.34,
        10.73, 83.92, 99.47, 23.91, 98.06, 40.87, 5.46, 68.66, 46.00, 38.48,
        76.53, 78.72, 69.05, 35.13, 15.59, 89.38, 26.93, 28.23, 64.34, 9.43
    };

    QLineSeries *series_income = new QLineSeries;
    QLineSeries *series_expenses = new QLineSeries;

    for (unsigned long long int i = 0; i < xData.size(); i++) {
        series_income->append(xData[i], income_Data[i]);
        series_expenses->append(xData[i], expenses_Data[i]);
    }

    //==================================================================================//

    QPen pen_income(Qt::green); //ตั้งค่าสีให้กับเส้นกราฟ
    pen_income.setWidth(2);  // ตั้งค่าความหนาของเส้นกราฟ
    series_income->setName("Income"); // ชื่อให้กับเส้นกราฟ
    series_income->setPen(pen_income); //ตั้งค่าเส้นของกราฟ income ให้เป็น pen_income ตามที่ตั้งค่าก่อนหน้า

    QPen pen_expenses(Qt::red); //ตั้งค่าสีให้กับเส้นกราฟ
    pen_expenses.setWidth(2);  // ตั้งค่าความหนาของเส้นกราฟ
    series_expenses->setName("expenses"); // ชื่อให้กับเส้นกราฟ
    series_expenses->setPen(pen_expenses); //ตั้งค่าเส้นของกราฟ income ให้เป็น pen_income ตามที่ตั้งค่าก่อนหน้า

    //==================================================================================//

    // Create a chart
    QChart *chart = new QChart;
    chart->addSeries(series_income); //เพิ่มเส้นกราฟ series_income ลงไปใน chart
    chart->addSeries(series_expenses); //เพิ่มเส้นกราฟ series_expenses ลงไปใน chart
    chart->setTitle("Analysis"); //ตั้งชื่อ chart

    //==================================================================================//

    //ตั้งค่าแต่ละแกนของกราฟ
    //chart->createDefaultAxes(); //เป็นฟังก์ชันใน Qt Charts ที่ใช้ สร้างแกน X และแกน Y โดยอัตโนมัติ ตามข้อมูลที่เพิ่มเข้าไปในกราฟ
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, *max_element(xData.begin() , xData.end()) );
    axisX->setTickCount(20);
    axisX->setLabelFormat("%.0f");
    axisX->setTitleText("Time");
    \
        vector<long double> max_y(2);
    max_y.push_back(*max_element(income_Data.begin() , income_Data.end()));
    max_y.push_back(*max_element(expenses_Data.begin() , expenses_Data.end()));

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, *max_element(max_y.begin() , max_y.end()));
    axisY->setTickCount(20);
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("Amount");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series_income->attachAxis(axisX);
    series_income->attachAxis(axisY);
    series_expenses->attachAxis(axisX);
    series_expenses->attachAxis(axisY);

    //==================================================================================//

    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); //ทำให้เส้นคมชัดขึ้น

    //==================================================================================//

    // Add chart view to layout
    ui->Chart_Layout->addWidget(chartView);

    //==================================================================================//
}
