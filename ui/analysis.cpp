#include "analysis.h"
#include "ui_analysis.h"
#include <header/json.h>
#include <QtCore>
#include <QtGui>
#include <QtCharts/QtCharts>
#include <QtCharts/QLineSeries>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

analysis::analysis(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::analysis)
{
    ui->setupUi(this);
    this->setWindowTitle("Analysis");
    startUI_setup();
    Show_Chart();
}

analysis::~analysis()
{
    delete ui;
}

//========================================//


struct Menus_struct
{
    vector<string> name;
    vector<string> type;
    vector<int> amount;
};

Menus_struct count_menus;

json menus , statement;

vector<double> chartData_income , chartData_expenses;
vector<QDate> chartData_Date;

vector<QDate> week_dates , month_dates , year_dates;

QChartView *chartView;

//========================================//

void analysis::startUI_setup()
{
    chartView = 0;
    Refresh_calendar();
    ui->comboBox_search_mode->setCurrentIndex(5);
    ui->calendar_start_date->setEnabled(false);
    ui->calendar_end_date->setEnabled(false);

    ui->NO_DATA->hide();

    getData(menus , "Menus");
    getData(statement , "Statement");

    for (unsigned int i = 0 ; i < menus.size() ; i++)
    {
        count_menus.name.push_back(menus[i][0]);
        count_menus.type.push_back(menus[i][2]);
        count_menus.amount.push_back(0);
    }


    for (unsigned int i = 0 ; i < statement.size() ; i++)
    {
        QDate Date_ = QDate::fromString(QString::fromStdString(statement[i][3]) , "dd-MM-yyyy");

        auto it = std::find(chartData_Date.begin(), chartData_Date.end(), Date_);
        if (it == chartData_Date.end())
        {
            chartData_Date.push_back(Date_);
            chartData_income.push_back(0);
            chartData_expenses.push_back(0);

            int newindex = chartData_Date.size() - 1;

            if (statement[i][2] >= 0) chartData_income[newindex] += abs(double(statement[i][2]));
            else chartData_expenses[newindex] += abs(double(statement[i][2]));
        }
        else
        {
            int index = std::distance(chartData_Date.begin() , it);

            if (statement[i][2] > 0) chartData_income[index] += abs(double(statement[i][2]));
            else chartData_expenses[index] += abs(double(statement[i][2]));
        }

        string menus_name = statement[i][0];
        int menus_amount = statement[i][1];

        unsigned int menus_index_in_count_menus = std::distance(count_menus.name.begin() , std::find(count_menus.name.begin() , count_menus.name.end() , menus_name));
        count_menus.amount[menus_index_in_count_menus] += menus_amount;
    }

    for (unsigned int i = 0 ; i < count_menus.name.size() ; i++)
    {
        qDebug() << "Name : " << count_menus.name[i] << " Type : " << count_menus.type[i] << " amount : " << count_menus.amount[i];
    }


    vector<QDate> unSort_chartData_Date = ::chartData_Date;
    std::sort(chartData_Date.begin() , chartData_Date.end());
    vector<unsigned int> sorted_index;

    for (unsigned int i = 0 ; i < unSort_chartData_Date.size() ; i++)
    {
        if (std::find(sorted_index.begin() , sorted_index.end() , i) != sorted_index.end()) continue;

        unsigned int chnaged_index = std::distance(chartData_Date.begin() , std::find(chartData_Date.begin() , chartData_Date.end() , unSort_chartData_Date[i]));
        if (chnaged_index != i)
        {
            qDebug() << QString::number(chnaged_index);

            double swap_income_data = chartData_income[chnaged_index];
            chartData_income[chnaged_index] = chartData_income[i];
            chartData_income[i] = swap_income_data;

            double swap_expenses_data = chartData_expenses[chnaged_index];
            chartData_expenses[chnaged_index] = chartData_expenses[i];
            chartData_expenses[i] = swap_expenses_data;

            sorted_index.push_back(i);
            sorted_index.push_back(chnaged_index);
        }
    }

    for (unsigned int i = 0 ; i < chartData_Date.size() ; i++)
    {
        qDebug() << "Date : "<< chartData_Date[i].toString("dd-MM-yyyy") << "Income : " << QString::number(chartData_income[i]) << "expenses : " << QString::number(chartData_expenses[i]);
    }
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
    week_dates = Get_Week_Dates(ui->calendar_start_date->selectedDate());

    // ลบ format เก่าทั้งหมดออก
    ui->calendar_start_date->setDateTextFormat(QDate(), QTextCharFormat());
    ui->calendar_end_date->setDateTextFormat(QDate(), QTextCharFormat());

    // สร้าง Format สำหรับไฮไลต์
    QTextCharFormat Highlight_Format;
    Highlight_Format.setBackground(Qt::green);
    Highlight_Format.setForeground(Qt::black);

    for (unsigned int i = 0 ; i < week_dates.size() ; i++)
    {
        if (week_dates[i] != ui->calendar_start_date->selectedDate()) ui->calendar_start_date->setDateTextFormat(week_dates[i], Highlight_Format);
    }
}


void analysis::Update_Highlight_Month()
{
    month_dates = Get_Month_Dates(ui->calendar_start_date->selectedDate());

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
    year_dates = Get_Year_Dates(ui->calendar_start_date->selectedDate());

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
        Show_Chart();
    }
    else if (searchmode == 2)
    {
        Update_Highlight_Week();
        Show_Chart();
    }
    else if (searchmode == 3)
    {
        Update_Highlight_Month();
        Show_Chart();
    }
    else if (searchmode == 4)
    {
        Update_Highlight_Year();
        Show_Chart();
    }
    else Show_Chart();
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
        Show_Chart();
    }
}


void analysis::on_Refresh_clicked()
{
    Refresh_calendar();
}


void analysis::Refresh_calendar()
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
    case 5:
        on_Refresh_clicked();
        ui->calendar_start_date->setEnabled(false);
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
    QDate start_Of_year(selected_Date.year(), 1, 1); // วันที่ 1 ของปีที่เลือก
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

    Show_Chart();
}


void analysis::Show_Chart()
{
    ui->NO_DATA->hide();

    //==================================================================================//

    if (chartView != 0)
    {
        ui->Chart_Layout->removeWidget(chartView);
        delete chartView;
        chartView = 0;
    }

    //==================================================================================//

    QDate minDate_range , maxDate_range;
    int searchmode = ui->comboBox_search_mode->currentIndex();
    switch (searchmode)
    {
    case 0:
        minDate_range = ui->calendar_start_date->selectedDate();
        maxDate_range = ui->calendar_end_date->selectedDate();
        break;
    case 1:
        minDate_range = ui->calendar_start_date->selectedDate();
        maxDate_range = minDate_range;
        break;
    case 2:
        minDate_range = week_dates[0];
        maxDate_range = week_dates[week_dates.size()-1];
        break;
    case 3:
        minDate_range = month_dates[0];
        maxDate_range = month_dates[month_dates.size()-1];
        break;
    case 4:
        minDate_range = year_dates[0];
        maxDate_range = year_dates[year_dates.size()-1];
        break;
    case 5:
        minDate_range = QDate::currentDate().addYears(-1000);
        maxDate_range = QDate::currentDate().addYears(1000);
        break;
    }

    //==================================================================================//

    // Create a line series
    QLineSeries *series_income = new QLineSeries;
    QLineSeries *series_expenses = new QLineSeries;

    int add_count = 0;
    for (unsigned int i = 0; i < chartData_Date.size(); i++)
    {
        if (chartData_Date[i] < minDate_range or chartData_Date[i] > maxDate_range) continue;

        qint64 xValue = chartData_Date[i].startOfDay().toMSecsSinceEpoch();
        series_income->append(xValue, chartData_income[i]);
        series_expenses->append(xValue, chartData_expenses[i]);
        add_count++;
    }

    //==================================================================================//
    QPointF income_ONE_point , expenses_ONE_point;
    if (add_count == 1)
    {
        income_ONE_point = series_income->at(0);
        expenses_ONE_point = series_expenses->at(0);

        series_income->clear();
        series_income->append(income_ONE_point.x()+1 , income_ONE_point.y());
        series_income->append(income_ONE_point.x()+2 , income_ONE_point.y());

        series_expenses->clear();
        series_expenses->append(expenses_ONE_point.x()+1 , expenses_ONE_point.y());
        series_expenses->append(expenses_ONE_point.x()+2 , expenses_ONE_point.y());
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
    // ใช้ QDateTimeAxis สำหรับแกน X
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("dd MMM yy"); // ตั้งค่าการแสดงผลวันที่
    axisX->setTitleText("Date");
    if (add_count >= 10) axisX->setTickCount(10);
    else if (add_count < 10 and add_count > 1) axisX->setTickCount(add_count);
    else if (add_count == 1)
    {
        axisX->setRange(QDateTime::fromMSecsSinceEpoch(income_ONE_point.x()+1) , QDateTime::fromMSecsSinceEpoch(income_ONE_point.x()+2));
        axisX->setTickCount(2);
    }
    else
    {
        axisX->setRange(QDateTime(minDate_range , QTime(0,0,1)) , QDateTime(maxDate_range , QTime(0,0,2)));
        axisX->setTickCount(2);
    }


    \
    vector<long double> max_y(2);
    max_y.push_back(*max_element(chartData_income.begin() , chartData_income.end()));
    max_y.push_back(*max_element(chartData_expenses.begin() , chartData_expenses.end()));
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, *max_element(max_y.begin() , max_y.end()));
    axisY->setTickCount(10);
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
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); //ทำให้เส้นคมชัดขึ้น

    //==================================================================================//

    // Add chart view to layout
    ui->Chart_Layout->addWidget(chartView);

    //==================================================================================//

    if (add_count == 0)
    {
        ui->NO_DATA->show();
    }
}


void analysis::on_accept_clicked()
{
    Show_Chart();
}

