
// ฟังก์ชันรับค่า Bills และตั้งค่าให้กับ BillsNames & BillsPrices
void getBills(const json& Bills, const json& Menus, vector<string>& BillsNames, vector<int>& BillsPrices) {
    
    if (Bills.is_array() && Bills.size() >= 2) { //ตรวจสอบว่า Bills เป็น array และมีขนาดไม่น้อยกว่า 2

        const auto& names = Bills[0];
        const auto& quantities = Bills[1];
        // บิลชื่อเมนูอยู่ที่ index 0 และจำนวนเมนูอยู่ที่ index 1

        if (names.is_array() && quantities.is_array()) { //ตรวจสอบว่า names และ quantities เป็น array
            for (size_t i = 0; i < names.size(); ++i) { //ลูปผ่านทุกชื่อเมนูและหาข้อมูลราคาและจำนวนของเมนูนั้น
                string menuName = names[i].get<string>(); //ชื่อเมนูจาก array names
                int quantity = quantities[i].get<int>(); //จำนวนของเมนูที่สั่งจาก array quantities

                // ค้นหาราคาของเมนูจาก Menus
                int menuPrice = 0;
                for (const auto& menu : Menus) { //ลูปผ่าน Menus เพื่อหาข้อมูลราคาเมนูที่ตรงกับ menuName
                    if (menu[0] == menuName) { //ตรวจสอบว่าเมนูใน Menus ตรงกับชื่อเมนู (menuName)
                        menuPrice = menu[1].get<int>();
                        break;
                    }
                }

                // เพิ่มชื่อเมนูและราคาลงใน BillsNames & BillsPrices
                if (menuPrice > 0) {
                    BillsNames.push_back(menuName);
                    BillsPrices.push_back(menuPrice * quantity); // ราคาคูณจำนวนที่สั่ง
                }
            }
        }
    }
}

