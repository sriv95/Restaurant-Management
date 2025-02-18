#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <filesystem>
#include <header/json.h>
#include <QMessageBox>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->seed_input->setText("12345");
    ui->calendar_Start->setSelectedDate(QDate::currentDate().addMonths(-5));
    ui->calendar_End->setSelectedDate(QDate::currentDate().addMonths(5));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Generate_Button_clicked()
{
    ui->Generate_Button->setEnabled(false);

    RunGenerate();

    ui->Generate_Button->setEnabled(true);
}


void MainWindow::RunGenerate()
{
    int seed;
    if(ui->checkBox_Auto_random->isChecked())
    {
        seed = rand();
        ui->seed_input->setText(QString::number(seed));
    }
    else seed = ui->seed_input->toPlainText().toInt();

    srand(seed);

    //------------------------------------------//

    create_empty_data_json();
    Employee();
    Menus_and_Stocks();
    Statement();
}


void MainWindow::create_empty_data_json()
{
    std::filesystem::create_directory("OutPut_Example_data");
    ofstream source;
    source.open("./OutPut_Example_data/data.json");
    source.close();

    //***************************************************//

    json templatedata = json::object();

    templatedata["Employee"] = json::array();
    templatedata["Menus"] = json::array();
    templatedata["Reservation"] = json::array();
    templatedata["Statement"] = json::array();
    templatedata["Stocks"] = json::array();

    json tables = json::array();
    for (int i = 1; i <= 9; ++i) {
        json table = json::object();
        json bills = json::array();
        json menu = json::array();

        menu.push_back("");
        bills.push_back(menu);
        bills.push_back(json::array());
        bills.push_back(json::array());

        table["Bills"] = bills;
        table["No"] = i;
        table["Reserved"] = "";
        table["Seats"] = 0;

        tables.push_back(table);
    }
    templatedata["Tables"] = tables;

    //***************************************************//

    setAllData(templatedata);
}


void MainWindow::Employee()
{
    vector<int> Employee_Age = {50, 100, 40, 28, 28, 20, 35, 1500, 3000};
    vector<string> Employee_Gender = {"Male", "Male", "Male", "Male", "Female", "Male", "Female", "Male", "Male"};
    vector<string> Employee_Job = {"Manager", "Chef", "Prep", "Dishwasher", "Cashier", "Server", "Server", "Bartender", "Janitor"};
    vector<string> Employee_Name = {"Tony Stark", "Steve Roger", "Bruce Banner", "Peter Parker", "Mary Jane", "Miles Morales", "Natasha Romanoff", "Thor Odinson", "Pang"};
    vector<double> Employee_Salary = {100000, 50000, 40000, 20000, 10000, 15000, 15000, 30000, 35000};

    json Employee;
    for (unsigned int i = 0 ; i < Employee_Name.size() ; i++)
    {
        Employee[i]["Age"] = Employee_Age[i];
        Employee[i]["Gender"] = Employee_Gender[i];
        Employee[i]["Job"] = Employee_Job[i];
        Employee[i]["Name"] = Employee_Name[i];
        Employee[i]["Salary"] = Employee_Salary[i];
    }

    setData(Employee , "Employee");
}


void MainWindow::Menus_and_Stocks()
{
    vector<vector<string>> Dished_Ingredients;
    vector<vector<string>> Drinks_Ingredients;

    Menus(Dished_Ingredients , Drinks_Ingredients);

    Stocks(Dished_Ingredients , Drinks_Ingredients);
}


void MainWindow::Menus(vector<vector<string>> &Dished_Ingredients , vector<vector<string>> &Drinks_Ingredients)
{
    //-------------------------------------------------------------//

    vector<string> Menus_Dished_Names = {"Spaghetti Carbonara", "Chicken Caesar Salad", "Grilled Salmon", "Beef Steak", "Margarita Pizza", "Fried Chicken", "Tom Yum Soup", "Pad Thai",
                                         "Green Curry Chicken", "Sushi Roll", "Burger", "Tacos", "French Fries", "Omelette", "Lasagna", "Fried Rice", "Clam Chowder", "Spring Rolls",
                                         "BBQ Ribs", "Chocolate Cake"};

    vector<double> Menus_Dished_Price = {150, 120, 250, 300, 180, 140, 160, 130, 170, 200, 190, 150, 100, 90, 220, 110, 180, 140, 260, 200};

    vector<vector<string>> Menus_Dished_Ingredients = {{"pasta", "bacon", "egg", "parmesan"}, {"chicken", "lettuce", "parmesan", "dressing"}, {"salmon", "lemon", "garlic", "olive oil"},
                                                       {"beef", "salt", "pepper", "butter"}, {"dough", "tomato", "cheese", "basil"}, {"chicken", "flour", "egg", "oil"},
                                                       {"shrimp", "lime", "chili", "lemongrass"}, {"noodle", "shrimp", "egg", "tofu"}, {"chicken", "coconut milk", "chili", "basil"},
                                                       {"rice", "seaweed", "salmon", "cucumber"}, {"bun", "beef", "lettuce", "cheese"}, {"tortilla", "beef", "tomato", "cheese"},
                                                       {"potato", "salt", "oil"}, {"egg", "cheese", "butter"}, {"pasta", "beef", "tomato", "cheese"}, {"rice", "egg", "soy sauce", "onion"},
                                                       {"clam", "potato", "milk", "butter"}, {"wrapper", "shrimp", "lettuce", "carrot"}, {"ribs", "sauce", "garlic", "pepper"},
                                                       {"flour", "chocolate", "egg", "sugar"}};

    vector<vector<double>> Menus_Dished_Ingredients_Amounts = {{1, 0.2, 1, 0.1}, {0.2, 1, 0.1, 0.1}, {1, 0.2, 0.1, 0.1}, {1, 0.01, 0.01, 0.1}, {1, 0.5, 0.3, 0.1}, {1, 0.3, 1, 0.5}, {0.3, 0.2, 0.1, 0.1},
                                                               {1, 0.2, 1, 0.1}, {0.3, 0.5, 0.1, 0.1}, {1, 1, 0.3, 0.2}, {1, 1, 0.1, 0.2}, {1, 0.3, 0.2, 0.2}, {1, 0.01, 0.2}, {2, 0.1, 0.1},
                                                               {1, 0.3, 0.5, 0.3}, {1, 1, 0.1, 0.1}, {0.3, 0.5, 0.5, 0.1}, {1, 0.3, 0.2, 0.2}, {1, 0.2, 0.1, 0.01}, {1, 0.5, 2, 0.5}};

    {
        //============================ qDebug Dished ============================//

        // qDebug() << "Menus_Dished_Names.size() :" << Menus_Dished_Names.size() << "Menus_Dished_Price.size() :" << Menus_Dished_Price.size()
        //          << "Menus_Dished_Ingredients.size() :" << Menus_Dished_Ingredients.size() << "Menus_Dished_Ingredients_Amounts.size() :" << Menus_Dished_Ingredients_Amounts.size();

        // for (unsigned int i = 0 ; i < Menus_Dished_Names.size() ; i++)
        // {
        //     qDebug() << Menus_Dished_Ingredients[i].size() << " : " << Menus_Dished_Ingredients_Amounts[i].size();
        //     if (Menus_Dished_Ingredients[i].size() != Menus_Dished_Ingredients_Amounts[i].size())
        //     {
        //         QMessageBox::about(this , "❗warning❗" , "❗size not match❗");
        //     }
        // }
        // QMessageBox::about(this , "✅✅✅✅" , "✅✅✅✅ Ingredients and Ingredients_Amounts correct");

        //============================ qDebug Dished ============================//
    }

    //-------------------------------------------------------------//

    vector<string> Menus_Drinks_Names = {"Iced Coffee", "Green Tea Latte", "Smoothie (Berry)", "Lemonade", "Mango Juice", "Coconut Shake", "Mojito (Non-alcoholic)", "Hot Chocolate",
                                         "Iced Matcha", "Orange Juice"};

    vector<double> Menus_Drinks_Price = {60, 70, 80, 50, 55, 65, 75, 85, 90, 50};

    vector<vector<string>> Menus_Drinks_Ingredients = {{"coffee", "milk", "ice", "sugar"}, {"green tea", "milk", "sugar"}, {"strawberry", "yogurt", "honey"}, {"lemon", "sugar", "water"},
                                                       {"mango", "sugar", "water"},{"coconut", "milk", "ice"}, {"mint", "lime", "soda", "sugar"}, {"chocolate", "milk", "sugar"},
                                                       {"matcha", "milk", "ice"}, {"orange", "sugar", "water"}};

    vector<vector<double>> Menus_Drinks_Ingredients_Amounts = {{1, 0.2, 1, 0.1}, {1, 0.2, 0.1}, {0.5, 0.3, 0.1}, {1, 0.2, 1}, {1, 0.1, 1}, {1, 0.3, 1}, {0.1, 0.2, 1, 0.1}, {0.3, 0.5, 0.2}, {1, 0.3, 1},
                                                               {1, 0.1, 1}};

    {
        //============================ qDebug Drinks ============================//

        // qDebug() << "Menus_Drinks_Names.size() :" << Menus_Drinks_Names.size() << "Menus_Drinks_Price.size() :" << Menus_Drinks_Price.size()
        //          << "Menus_Drinks_Ingredients.size() :" << Menus_Drinks_Ingredients.size() << "Menus_Drinks_Ingredients_Amounts.size() :" << Menus_Drinks_Ingredients_Amounts.size();

        // for (unsigned int i = 0 ; i < Menus_Drinks_Names.size() ; i++)
        // {
        //     qDebug() << Menus_Drinks_Ingredients[i].size() << " : " << Menus_Drinks_Ingredients_Amounts[i].size();
        //     if (Menus_Drinks_Ingredients[i].size() != Menus_Drinks_Ingredients_Amounts[i].size())
        //     {
        //         QMessageBox::about(this , "❗warning❗" , "❗size not match❗");
        //     }
        // }
        // QMessageBox::about(this , "✅✅✅✅" , "✅✅✅✅ Ingredients and Ingredients_Amounts correct");

        //============================ qDebug Drinks ============================//
    }

    //-------------------------------------------------------------//

    json Menus;
    unsigned int i = 0;
    for ( ; i < Menus_Dished_Names.size() ; i++)
    {
        Menus[i][0] = Menus_Dished_Names[i];
        Menus[i][1] = Menus_Dished_Price[i];
        Menus[i][2] = "Dishes";
        Menus[i][3] = Menus_Dished_Ingredients[i];
        Menus[i][4] = Menus_Dished_Ingredients_Amounts[i];
    }

    for (unsigned int j = 0 ; j < Menus_Drinks_Names.size() ; i++ , j++)
    {
        Menus[i][0] = Menus_Drinks_Names[j];
        Menus[i][1] = Menus_Drinks_Price[j];
        Menus[i][2] = "Drinks";
        Menus[i][3] = Menus_Drinks_Ingredients[j];
        Menus[i][4] = Menus_Drinks_Ingredients_Amounts[j];
    }

    //-------------------------------------------------------------//

    setData(Menus , "Menus");

    //------------ To - Stocks -------------//

    Dished_Ingredients = Menus_Dished_Ingredients;
    Drinks_Ingredients = Menus_Drinks_Ingredients;


    //------------ To - Stocks -------------//
}


void MainWindow::Stocks(vector<vector<string>> Dished_Ingredients , vector<vector<string>> Drinks_Ingredients)
{
    //-------------------------------------------------------------//

    vector<string> split_Ingredients;

    for (unsigned int i = 0 ; i < Dished_Ingredients.size() ; i++)
    {
        for (unsigned int j = 0 ; j < Dished_Ingredients[i].size() ; j++)
        {
            if (std::find(split_Ingredients.begin() , split_Ingredients.end() , Dished_Ingredients[i][j]) == split_Ingredients.end())
            {
                split_Ingredients.push_back(Dished_Ingredients[i][j]);
            }
        }
    }

    for (unsigned int i = 0 ; i < Drinks_Ingredients.size() ; i++)
    {
        for (unsigned int j = 0 ; j < Drinks_Ingredients[i].size() ; j++)
        {
            if (std::find(split_Ingredients.begin() , split_Ingredients.end() , Drinks_Ingredients[i][j]) == split_Ingredients.end())
            {
                split_Ingredients.push_back(Drinks_Ingredients[i][j]);
            }
        }
    }

    {
        //============================ qDebug ============================//

        // int total_in_vector = 0;
        // for (int i = 0 ; i < split_Ingredients.size() ; i++)
        // {
        //     total_in_vector = 0;
        //     for (int j = 0 ; j < split_Ingredients.size() ; j++)
        //     {
        //         if(split_Ingredients[i] == split_Ingredients[j]) total_in_vector++;
        //     }
        //     qDebug() << QString::fromStdString(split_Ingredients[i]) << " : " << total_in_vector;
        // }

        //============================ qDebug ============================//
    }

    //-------------------------------------------------------------//


    json Stocks;
    for (unsigned int i = 0 ; i < split_Ingredients.size() ; i++)
    {
        Stocks[i][0] = split_Ingredients[i];
        Stocks[i][1] = rand()%201 + 50;
    }

    setData(Stocks , "Stocks");

    //-------------------------------------------------------------//
}


void MainWindow::Statement()
{
    //-------------------------------------------------------------//

    json Statement;

    QDateTime Start_DateTime = ui->calendar_Start->selectedDate().startOfDay();
    QDateTime End_DateTime = ui->calendar_End->selectedDate().endOfDay();

    if (Start_DateTime > End_DateTime)
    {
        QMessageBox::about(this , "❗warning❗" , "เลือกวันให้มันถูกๆ หน่อยดิเฮ้ย");
        return;
    }

    int max_stock = ui->input_Max_Stock_per_day->toPlainText().toInt();
    int max_amount_stock = ui->input_Max_amount_Stock_per_time->toPlainText().toInt();
    int max_price_stock = ui->input_max_price_Stock->toPlainText().toInt();
    int min_price_stock = ui->input_min_price_Stock->toPlainText().toInt();

    if (min_price_stock > max_price_stock)
    {
        QMessageBox::about(this , "❗warning❗" , "Max Min ใส่ให้ถูกๆ หน่อยดิเฮ้ย");
        return;
    }

    int max_order = ui->input_Max_order_per_day->toPlainText().toInt();
    int max_amount_order = ui->input_max_amount_Menus_per_time->toPlainText().toInt();
    int chance_no_Statement = ui->input_Chance_of_no_Statement->toPlainText().toInt();
    int chance_income = ui->input_Chance_of_income->toPlainText().toInt();

    // qDebug() << max_order << " " << chance_no_Statement << " " << chance_income;

    json Menus;
    json Stocks;
    getData(Menus , "Menus");
    getData(Stocks , "Stocks");
    int Menus_size = Menus.size();
    int Stocks_size = Stocks.size();

    // qDebug() << Menus_size << " " << Stocks_size;

    //-------------------------------------------------------------//

    QDateTime GenerateTime = Start_DateTime;
    QDateTime temp_GenerateTime = Start_DateTime;
    int i = 0;

    for ( ; GenerateTime <= End_DateTime ; )
    {
        if (rand()%100 + 1 <= chance_no_Statement)
        {
            GenerateTime = GenerateTime.addDays(1);
            GenerateTime.setTime(QTime(0,0,0));
            continue;
        }

        if (rand()%100 + 1 <= chance_income)
        {
            //Income
            int Amount_order_in_day = rand()%max_order + 1;
            int min_minute = 1200/Amount_order_in_day;

            for (int j = 0 ; j < Amount_order_in_day ; j++ , i++)
            {
                int rand_amount = rand()%max_amount_order + 1;
                int rand_index_menu = rand()%Menus_size;

                Statement[i][0] = Menus[rand_index_menu][0];
                Statement[i][1] = rand_amount;
                Statement[i][2] = double(Menus[rand_index_menu][1]) * rand_amount;

                //---------------- TIME ----------------//

                int rand_minute = rand()%min_minute + 1;
                int rand_ms = rand()%50 + 1;

                GenerateTime = GenerateTime.addSecs(rand_minute*60).addMSecs(rand_ms);

                Statement[i][3] = GenerateTime.date().toString("dd-MM-yyyy").toStdString();
                Statement[i][4] = GenerateTime.time().toString("HH:mm:ss:ms").toStdString();

                //---------------- TIME ----------------//
            }

            // qDebug() << GenerateTime.toString("dd-MM-yyyy HH:mm:ss:ms");
            if (temp_GenerateTime > GenerateTime) QMessageBox::about(this , "❌❌❌" , "❌❌❌ Time ➕➕➕ Incorrect");
            temp_GenerateTime = GenerateTime;

            GenerateTime = GenerateTime.addDays(1);
            GenerateTime.setTime(QTime(0,0,0));
        }
        else
        {
            //Expenses
            int Amount_order_in_day = rand()%max_stock + 1;
            int min_minute = 1200/Amount_order_in_day;

            for (int j = 0 ; j < Amount_order_in_day ; j++ , i++)
            {
                int rand_amount = rand()%max_amount_stock + 1;
                int rand_index_menu = rand()%Stocks_size;
                int rand_price = rand()%(max_price_stock - min_price_stock + 1) + min_price_stock;

                Statement[i][0] = Stocks[rand_index_menu][0];
                Statement[i][1] = rand_amount;
                Statement[i][2] = -1 * (rand_amount * rand_price);

                // qDebug() << rand_amount << " * " << rand_price;

                //---------------- TIME ----------------//

                int rand_minute = rand()%min_minute + 1;
                int rand_ms = rand()%50 + 1;

                GenerateTime = GenerateTime.addSecs(rand_minute*60).addMSecs(rand_ms);

                Statement[i][3] = GenerateTime.date().toString("dd-MM-yyyy").toStdString();
                Statement[i][4] = GenerateTime.time().toString("HH:mm:ss:ms").toStdString();

                //---------------- TIME ----------------//
            }

            // qDebug() << GenerateTime.toString("dd-MM-yyyy HH:mm:ss:ms");
            if (temp_GenerateTime > GenerateTime) QMessageBox::about(this , "❌❌❌" , "❌❌❌ Time ➕➕➕ Incorrect");
            temp_GenerateTime = GenerateTime;

            GenerateTime = GenerateTime.addDays(1);
            GenerateTime.setTime(QTime(0,0,0));
        }
    }

    //-------------------------------------------------------------//

    setData(Statement , "Statement");
}
