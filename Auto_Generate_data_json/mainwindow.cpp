#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <filesystem>
#include <header/json.h>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    RunGenerate();
}


void MainWindow::RunGenerate()
{
    create_empty_data_json();
    Employee();
    Menus();
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


void MainWindow::Menus()
{
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
}


void MainWindow::Reservation()
{

}


void MainWindow::Statement()
{

}


void MainWindow::Stocks()
{

}
