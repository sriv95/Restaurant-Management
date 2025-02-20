
# 🍽️My Restaurant Manager

  

  **My Restaurant Manager** is an application that is designed to help restaurant owners streamline daily operations. It manages table reservations, orders, billing, stock, employees, and financial reports. All data is stored in JSON format for easy import/export.
  **My Restaurant Manager** uses C++, Qt 6 and [nlohmann-json](https://github.com/nlohmann/json). 

  

## **✨Features**

-  **🗂️JSON Data File Configuration**: `Create` a new JSON File template for the program or `Open` an existing JSON file (must follow the template). Uses JSON to import/export all data in a single file.

-  **📋Table Management**: **View** and `manage` table availability and reservations.

-  **📲Reserve Table**: `Reserve` tables and `save` reservations in JSON file.

-  **🛒Order Processing**: `Track` customer orders and `update` their status from data.

-  **📃Billing System**: `Generate` customer bills and `store` billing details in JSON.

-  **📦Stock Management**: `Monitor` inventory and `update` stock levels.

-  **👥Employee Management**: `Store` employee data, roles, and schedules.

-  **📖Menu Editing**: `Add`, `update`, or `remove` menu items.

-  **🍽️Order Food**: `Process` food orders, **link** them to customer bills, and `update` stock accordingly.

-  **📊Financial Statements**: `Generate` daily, weekly, or monthly sales reports, including expenses.

-  **📈Business Analysis**: `Analyze` trends and performance to optimize operations.

---

## 🛠**Usage**
### ⚙  **First-Time Setup**
1. Launch the application.
2. The File Configuration window will pop up. If no `data.json` is detected, you can:

   -- Click `New` to create a new `data.json` file with default keys compatible with the program.

   -- Click `Open` to select an existing `data.json` file.

3. If `data.json` is found, the `Continue` button will appear to proceed to the main UI.

### 🗂️ **JSON Data File Configuration**
- Status Text: Shows whether the JSON file is found or not based on the default PATH. If not, it displays "Data not found"; if the file is found, it displays "Data found at...".

- `New` button: Creates a `data.json` file with default keys and updates the status text to "Data created at...".

- `Open` button: Allows you to choose an existing `.json` file and updates the status text to "Data opened at...".

- `Continue` button: Appears after a data file is found, created, or opened. Clicking it leads to the main UI.

- `Force Close` button: Immediately terminates the app. Used when no `.json` file is found and the user wants to exit.

- `Open File...` button: Available in the main UI. Opens the File Configuration window.
 

-------------------------------------------------UNFINISHED-------------------------------------------------
### 📋 **Table Management**
- Click `Tables` icons in the main ui.
- Green = Available, Red = Reserved.
- Click an available table to make a reservation.

### 📲 **Reserve Table**
- Select a table and click `Reserve`.
- Enter customer details and confirm.
- Reservation will be saved in `data.json`.

### 🛒 **Order Processing**
- Click `Orders` to view pending orders.
- Select an order, update its status (e.g., "Preparing", "Served").
- Orders are saved automatically.

### 📃 **Billing System**
- Click `Billing` to see customer bills.
- Select a bill and choose `Generate PDF` or `Print`.
- Bills are stored in `data.json`.

### 📦 **Stock Management**
- Click `Stock` to view available items.
- Click `Add Item` or `Update Stock` to edit.
- Changes are reflected in `data.json`.

### 👥 **Employee Management**
- Click `Employees` to view staff details.
- Add, edit, or remove employee records.
- Work schedules are stored in JSON.

### 📖 **Menu Editing**
- Click `Menu` to view food items.
- Click `Add Item` to introduce a new dish.
- Click an existing item to modify details.

### 🍽️ **Order Food**
- Click `New Order`.
- Select menu items and confirm.
- Order will be saved and linked to billing.

### 📊 **Financial Statements**
- Click `Reports` to generate sales reports.
- Choose between `Daily`, `Weekly`, or `Monthly`.
- Sales data will be calculated and displayed.

### 📈 **Business Analysis**
- Click `Analytics` to view business insights.
- Analyze trends like best-selling items or peak hours.
- Data is sourced from `data.json`.

---
