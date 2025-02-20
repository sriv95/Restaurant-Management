
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

   -  Click `New` to create a new `data.json` file with default keys compatible with the program.

   -  Click `Open` to select an existing `data.json` file.

3. If `data.json` is found, the `Continue` button will appear to proceed to the main UI.
   
---

### 🗂️ **JSON Data File Configuration**
Opens the "File Configuration:" window.

#### **Features**

- ##### **Status Text**: 
   - Shows whether the JSON file is found or missing based on the default PATH.
     - If not, it displays "Data not found"
     - If the file is found, it displays "Data found at...".

- ##### **Buttons**:
  - `New`: Creates a `data.json` file with default keys and updates the status text to "Data created at...".

  - `Open`: Allows you to choose an existing `.json` file and updates the status text to "Data opened at...".

  - `Continue`: Appears after a data file is found, created, or opened. Clicking it leads to the main UI.

  - `Force Close`: Immediately terminates the app. Used when no `.json` file is found and the user wants to exit.

  - `Open File...`: Available in the main UI. Opens the File Configuration window.
 
---

### 📋 **Table Management**
On the main UI, there are a total of 9 clickable icons related to **Table Management** labeled from "Table#1" to "Table#9", which are located in the center of the UI.

#### **Features**

- ##### **Table Selection**:
  - Clicking a `Table` icon, reveals more buttons at the bottom:
    - `Order Food`
    - `Open Table`
    - `Reserve`
    - `Check Bills` at the bottom right corner.

- ##### **Buttons**:
  - `Refresh`: Resets the UI, hiding additional buttons.

  - `Open Table`: Opens the "Open Table" window.
    - Seats can be selected by clicking icons (1-4 seats) or manually entering a number (can exceed 4).
    - Clicking `OK` confirms and completes the table opening process.

  - `Change seat`: Opens the "Open Table" window.
    - Appears in place of `Open Table` after that said table has seats selected.

---

### 📲 **Reserve Table**
The `Reserve` button appears once a `Table` icon is selected.

#### **Features**

- ##### **Buttons**:
  - `Reserve`: Opens the "Reserve" window.
    - Shows boxes where user has to fill in info:
      - `Table Number` - fixed (no edit).
      - `Reservation Time`
      - `Customer Name`
      - `Phone Number`
    - Clicking `Reserve` confirms and completes the process.
      
  - `Unreserve`: Appears in place of `Reserve` after clicking on the reserved `table` icon. Revokes the reservation.

---

### 📃 **Billing System**
The `Check Bills` button appears once a `Table` icon is selected.

#### **Features**

- ##### **Buttons**:
  - `Check Bills`: Shows receipt of a `table`.
    - Includes the `table` number, current date and time.
    - Shows the table's orders, amount of each order, pricing, and total price.
      
  - `Confirm Payment`: Makes the said `table` available.
    - Appears in place of `Check Bills` after `Check Bills` is clicked.
   
---

### 📦 **Stock Management**
On the main UI, there is a button called `Stocks`.

#### **Features**

- ##### **Buttons**:
  - `Stocks`: Opens the "Stock" window.
    - Three columns are shown:
      - `No.` (uneditable)
      - `Name`
      - `Quantity`
      - Double clicking a slot you want to edit.
    
    - `Add`: Creates a new row in the "table."
      - `No.` column has a value added by 1 each time `Add` is clicked.
      - `Name` column has a value "New Stock" as a default.
      - `Quantity` column has a value "0" as a default.
        
    - `Delete`: Deletes a row.
      - Selecting any slot and clicking `Delete` will delete the row the slot is associated with.
      - Can multi-delete by using `ctrl` and `shift` operations.

    - `Save`: Saves all the texts/data in the "table" to the .json file.
      - Reopening the "Stock" window after clicking `Save` makes everything in the "table" return. 

---

### 👥 **Employee Management**
On the main UI, there is a button called `Employee`.

#### **Features**

- ##### **Buttons**:
  - `Employee`: Opens the "Employee" window.
    - Six columns are shown:
      - `No.` (invisible header)
      - `Name`
      - `Age`
      - `Gender`
      - `Salary`
      - `Job`
      - Double clicking a slot you want to edit.
    
    - `Add`: Creates a new row in the "table."
      - `No.` column has a value added by 1 each time `Add` is clicked.
      - Other columns have an empty value as a default.
    
    - `Delete`: Deletes a row.
      - Selecting the first `No.` column and clicking `Delete` will delete the row the `No.` is associated with.
      - Can multi-delete by using `ctrl` and `shift` operations.

    - `Save`: Saves all the texts/data in the "table" to the `.json` file.
      - Reopening the "Employee" window after clicking `Save` makes everything in the "table" return.

    - `Refresh`: Returns to the state the last time the json file is saved.

---

### 📖 **Menu Editing**
On the main UI, there is a button called `Edit Menu`.

#### **Features**

- ##### **Edit Menu**:
  - Left side "table"
  - Six columns are shown:
      - `No.` (invisible header)
      - `Menu`
        - Double clicking a slot you want to edit.
      - `Price`
        - It's in the form of boxes. You can both click to edit and type or manually click the up and down icons or you can even use your scroll mouse to either increase or decrease the price.
      - `Type`
        - Click and you can select to categorize the menu type as: `Dishes`, `Drinks`, or `Not Selected`.
      - `Ingredients` (uneditable)
      - `Edit Ingredients`
        - Shows the ingredients of that menu you clicked with on the right side "table".
          
  - ###### **Buttons**:
  
    - `Add Menu`: Creates a new row in the left "table."
      - `No.` column has a value added by 1 each time `Add Menu` is clicked.
      - `Menu` and `Ingredients` have an empty value as default.
      - `Price` has "0" value as default.'
      - `Type` has the `Not Selected` as default.
        
    - `Delete Menu`: Deletes a row.
      - Selecting any slot and clicking `Delete` will delete the row the slot is associated with.

- ##### **Edit Ingredients**:
  - Right side "table"
  - Data/texts are shown after clicking `Edit Ingredients` from the `Edit Menu` side.
  - Four columns are shown:
    - `No.` (invisible header)
    - `Ingredients`
      - Clicking its slot, you can choose from all the given ingredients.
    - `Quantity`
      - You can both click to edit and type or manually click the up and down icons or you can even use your scroll mouse to either increase or decrease the price.
    - `Del`
      - Appears as a big red X cross icon in each row. Delete the row X icon is associated with.
        
  - ###### **Buttons**:
    - `Add Ingredients`: - Creates a new row in the "table."
      - `No.` column has a value added by 1 each time `Add` is clicked.
      - `Ingredients` has "-" as a default value.
      - `Quantity` has "0" as a default value.


- ##### **Buttons**:
  - `Edit Menu`: Opens the "Edit Menu" window.
    
    - `Refresh`: Returns to the state the last time the json file is saved.
      
    - `Save`: Saves all the texts/data in the "table" to the .json file.



---

-------------------------------------------------UNFINISHED-------------------------------------------------










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
