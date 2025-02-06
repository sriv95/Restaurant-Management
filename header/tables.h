#ifndef TABLES_H
#define TABLES_H

#include <vector>
#include <string>
#include <header/nlohmann-json.hpp>
using json = nlohmann::json;
using namespace std;

void getBills(const json& Bills, const json& Menus, vector<string>& BillsNames, vector<int>& BillsPrices);

#endif // TABLES_H

