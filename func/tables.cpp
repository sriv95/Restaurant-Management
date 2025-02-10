#include "tables.h"

void getBills(const json& Bills, const json& Menus, vector<string>& BillsNames, vector<int>& BillsPrices) {
    if (Bills.is_array() && Bills.size() >= 3) {
        const auto& names = Bills[0];
        const auto& quantities = Bills[1];
        const auto& extras = Bills[2];

        if (names.is_array() && quantities.is_array() && extras.is_array()) {
            for (size_t i = 0; i < names.size(); ++i) {
                string menuName = names[i].get<string>();
                int quantity = quantities[i].get<int>();
                int extraCharge = extras[i].get<int>();

                int menuPrice = 0;
                for (const auto& menu : Menus) {
                    if (menu[0] == menuName) {
                        menuPrice = menu[1].get<int>();
                        break;
                    }
                }

                if (menuPrice > 0) {
                    BillsNames.push_back(menuName);
                    BillsPrices.push_back((menuPrice * quantity) + extraCharge);
                }
            }
        }
    }
}
