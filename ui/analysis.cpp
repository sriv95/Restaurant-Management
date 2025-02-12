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

    on_Refresh_clicked();
}

analysis::~analysis()
{
    delete ui;
}


void analysis::Update_Selectable_and_Highlight_DateRange()
{
    QDate startDate = ui->calendar_start_date->selectedDate();
    QDate endDate = ui->calendar_end_date->selectedDate();
    ui->calendar_start_date->setMaximumDate(endDate);
    ui->calendar_end_date->setMinimumDate(startDate);

    // ลบ format เก่าทั้งหมดออก
    ui->calendar_start_date->setDateTextFormat(QDate(), QTextCharFormat());
    ui->calendar_end_date->setDateTextFormat(QDate(), QTextCharFormat());

    // สร้าง Format สำหรับไฮไลต์
    QTextCharFormat Highlight_Range_Format;
    Highlight_Range_Format.setBackground(Qt::green);
    Highlight_Range_Format.setForeground(Qt::black);

    for (QDate date = startDate.addDays(1) ; date <= endDate; date = date.addDays(1))
    {
        ui->calendar_start_date->setDateTextFormat(date, Highlight_Range_Format);
        ui->calendar_end_date->setDateTextFormat(date.addDays(-1), Highlight_Range_Format);
    }
}


void analysis::Update_Highlight_Week()
{
    vector<QDate> week_dates = Get_Week_Dates(ui->calendar_start_date->selectedDate());

    // ลบ format เก่าทั้งหมดออก
    ui->calendar_start_date->setDateTextFormat(QDate(), QTextCharFormat());
    ui->calendar_end_date->setDateTextFormat(QDate(), QTextCharFormat());

    // สร้าง Format สำหรับไฮไลต์
    QTextCharFormat Highlight_Format;
    Highlight_Format.setBackground(Qt::green);
    Highlight_Format.setForeground(Qt::black);

    for (int i = 0 ; i < week_dates.size() ; i++)
    {
        if (week_dates[i] != ui->calendar_start_date->selectedDate()) ui->calendar_start_date->setDateTextFormat(week_dates[i], Highlight_Format);
    }
}


void analysis::Update_Highlight_Month()
{
    vector<QDate> month_dates = Get_Month_Dates(ui->calendar_start_date->selectedDate());

    // ลบ format เก่าทั้งหมดออก
    ui->calendar_start_date->setDateTextFormat(QDate(), QTextCharFormat());
    ui->calendar_end_date->setDateTextFormat(QDate(), QTextCharFormat());

    // สร้าง Format สำหรับไฮไลต์
    QTextCharFormat Highlight_Format;
    Highlight_Format.setBackground(Qt::green);
    Highlight_Format.setForeground(Qt::black);

    for (unsigned long int i = 0 ; i < month_dates.size() ; i++)
    {
        if (month_dates[i] != ui->calendar_start_date->selectedDate()) ui->calendar_start_date->setDateTextFormat(month_dates[i], Highlight_Format);
    }

    // for (int i = 0 ; i < month_dates.size() ; i++)
    // {
    //     qDebug() << i+1 << ". " << month_dates[i].toString("dd-MM-yyyy");
    // }
}


void analysis::Update_Highlight_Year()
{
    vector<QDate> year_dates = Get_Year_Dates(ui->calendar_start_date->selectedDate());

    // ลบ format เก่าทั้งหมดออก
    ui->calendar_start_date->setDateTextFormat(QDate(), QTextCharFormat());
    ui->calendar_end_date->setDateTextFormat(QDate(), QTextCharFormat());

    // สร้าง Format สำหรับไฮไลต์
    QTextCharFormat Highlight_Format;
    Highlight_Format.setBackground(Qt::green);
    Highlight_Format.setForeground(Qt::black);

    for (unsigned long int i = 0 ; i < year_dates.size() ; i++)
    {
        if (year_dates[i] != ui->calendar_start_date->selectedDate()) ui->calendar_start_date->setDateTextFormat(year_dates[i], Highlight_Format);
    }

    // for (int i = 0 ; i < year_dates.size() ; i++)
    // {
    //     qDebug() << i+1 << ". " << year_dates[i].toString("dd-MM-yyyy");
    // }
}


void analysis::on_calendar_start_date_selectionChanged()
{
    int searchmode = ui->comboBox_search_mode->currentIndex();
    if (searchmode == 0)
    {
        QDate startDate = ui->calendar_start_date->selectedDate();
        QDate endDate = ui->calendar_end_date->selectedDate();
        if (startDate > endDate) ui->calendar_end_date->setSelectedDate(startDate);
        Update_Selectable_and_Highlight_DateRange();
    }
    else if (searchmode == 2)
    {
        Update_Highlight_Week();
    }
    else if (searchmode == 3)
    {
        Update_Highlight_Month();
    }
    else if (searchmode == 4)
    {
        Update_Highlight_Year();
    }
}


void analysis::on_calendar_end_date_selectionChanged()
{
    int searchmode = ui->comboBox_search_mode->currentIndex();
    if (searchmode == 0)
    {
        QDate startDate = ui->calendar_start_date->selectedDate();
        QDate endDate = ui->calendar_end_date->selectedDate();
        if (startDate > endDate) ui->calendar_start_date->setSelectedDate(endDate);
        Update_Selectable_and_Highlight_DateRange();
    }
}


void analysis::on_Refresh_clicked()
{
    ui->calendar_start_date->setSelectedDate(QDate::currentDate());
    ui->calendar_end_date->setSelectedDate(QDate::currentDate());

    ui->calendar_start_date->setMaximumDate(QDate::currentDate().addYears(1000));
    ui->calendar_end_date->setMinimumDate(QDate::currentDate().addYears(-1000));

    // ลบ format ทั้งหมดออก
    ui->calendar_start_date->setDateTextFormat(QDate(), QTextCharFormat());
    ui->calendar_end_date->setDateTextFormat(QDate(), QTextCharFormat());
}


void analysis::on_comboBox_search_mode_currentIndexChanged(int searchmode)
{
    switch (searchmode) {
    case 0:
        on_Refresh_clicked();
        ui->calendar_start_date->setEnabled(true);
        ui->calendar_end_date->setEnabled(true);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        on_Refresh_clicked();
        ui->calendar_start_date->setEnabled(true);
        ui->calendar_end_date->setEnabled(false);
        break;
    }
}


vector<QDate> analysis::Get_Week_Dates(QDate selected_Date)
{
    vector<QDate> week_Dates;

    // หาววันแรกของสัปดา
    int day_Of_Week = selected_Date.dayOfWeek();  // 1 = จันทร์, 7 = อาทิตย์
    QDate day_start_Of_Week = selected_Date.addDays(-(day_Of_Week - 1));

    // เพิ่มวันทั้งอาทิตย์เข้า vector
    for (int i = 0; i < 7; ++i)
    {
        week_Dates.push_back(day_start_Of_Week.addDays(i));
    }

    return week_Dates;
}


vector<QDate> analysis::Get_Month_Dates(QDate selected_Date)
{
    vector<QDate> month_Dates;

    // หาววันแรกและวันสุดท้ายของเดือน
    QDate start_Of_Month(selected_Date.year(), selected_Date.month(), 1); // วันที่ 1 ของเดือนที่เลือก
    QDate end_Of_Month = start_Of_Month.addMonths(1).addDays(-1); // จะได้วันที่ 1 ของเดือนถัดไป และลบด้วย 1 วัน จะได้วันสุดท้ายของเดือนที่เลือก

     // เพิ่มวันทั้งเดือนเข้า vector
    for (QDate date = start_Of_Month; date <= end_Of_Month; date = date.addDays(1))
    {
        month_Dates.push_back(date);
    }

    return month_Dates;
}


vector<QDate> analysis::Get_Year_Dates(QDate selected_Date)
{
    vector<QDate> year_Dates;

    // หาววันแรกและวันสุดท้ายของปี
    QDate start_Of_year(selected_Date.year(), selected_Date.month(), 1); // วันที่ 1 ของปีที่เลือก
    QDate end_Of_year = start_Of_year.addYears(1).addDays(-1); // จะได้วันที่ 1 ของปีถัดไป และลบด้วย 1 วันจะได้วันสุดท้ายของของปีที่เลือก

    // เพิ่มวันทั้งเดือนเข้า vector
    for (QDate date = start_Of_year; date <= end_Of_year; date = date.addDays(1))
    {
        year_Dates.push_back(date);
    }

    return year_Dates;
}


void analysis::on_calendar_start_date_currentPageChanged(int year, int month)
{
    int searchmode = ui->comboBox_search_mode->currentIndex();
    switch (searchmode) {
    case 3:
    case 4:
        ui->calendar_start_date->setSelectedDate(QDate(year , month , 1));
        break;
    }
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



