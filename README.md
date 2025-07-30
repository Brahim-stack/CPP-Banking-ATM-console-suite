# Banking & ATM Console Suite  
A mini-core-banking back-office **+** ATM console application written in modern C++ (C++17) using a *functional-first* design.

|
 Project 
|
 Role 
|
 Folder 
|
|
---------
|
------
|
--------
|
|
**
BankSystem
**
|
 Back-office used by bank employees & admins 
|
`BankSystem/`
|
|
**
ATMSystem
**
|
 Front-office used by clients at an ATM      
|
`ATMSystem/`
|
|
**
SharedComponents
**
|
 Tiny “data layer” that holds the shared text-file database 
|
`SharedComponents/`
|

Both executables work on the **same flat-file database** (`data/Clients.txt`, `data/Users.txt`) so you can run them in parallel and instantly see changes reflected in the other program.

---

## ✨ Features
### BankSystem (back-office)
* User authentication & role-based access (bit-mask permissions)
* Full CRUD on clients
* Per-client transactions (deposit/withdraw)
* Total balance report
* User management (add, update, delete, search)
* Logout / multi-session support

### ATMSystem (front-office)
* Client login with account number + PIN
* Quick withdraw (preset amounts)
* Normal withdraw (custom amount, multiples of 5)
* Deposit
* Check balance
* Logout

### SharedComponents
* Clean, delimiter-based serialization  `#//#`
* Pure, reusable functions  
  `SplitString → ConvertLineToRecord → business logic → ConvertRecordToLine`

---

## 🧑‍💻  Functional-first C++
The code is organised as a **pipeline of pure functions** that:

1. Read immutable structs (`strClient`, `strUser`) from disk  
2. Transform copies of those structs (no shared mutable state)  
3. Persist them back to disk

This approach makes the program:
* Easy to reason about & test
* Free of hidden side-effects
* Naturally thread-friendly should you decide to add concurrency later

---

## 🗄  Flat-file database format
```
AccountNumber#//#Pin#//#Name#//#Phone#//#Balance
1001#//#1234#//#Jane Smith#//#+44 123-456#//#2000.50
```
Exactly the same idea for `Users.txt`.

---

## 🏗️  Build & Run

```bash
# Bank System
cd BankSystem
g++ -std=c++17 -O2 main.cpp -o BankSystem.exe
./BankSystem.exe            # or .\BankSystem.exe on Windows

# ATM System
cd ../ATMSystem
g++ -std=c++17 -O2 main.cpp -o ATMSystem.exe
./ATMSystem.exe
```

> No external libraries are required—only a C++17 compliant compiler.

---

## 📂  Repository structure
```
.
├── BankSystem/
│   └── main.cpp
├── ATMSystem/
│   └── main.cpp
├── SharedComponents/
│   └── data/
│       ├── Clients.txt
│       └── Users.txt
└── README.md
```

---

## 🛣️  Road-map
* [ ] Unit tests with **Catch2**
* [ ] Replace flat files with **SQLite / LiteDB**
* [ ] Multithreaded transaction queue
* [ ] GUI front-end (Qt / ImGui)

---

## 🤝  Contributing
Pull requests are welcome!  
Please open an issue first to discuss what you would like to change.

---

## 📜  Licence
MIT – do whatever you want, no warranty.
