# 🏦 Bank Management System (Version 1.1)

A simple console-based C++ application that allows you to manage bank clients using basic file storage. This is the first version of the system, focusing on core **client management features**, and now includes a basic **transactions menu**.

---

## 📌 Features

- View all clients  
- Add new clients  
- Update existing clients  
- Delete clients  
- Find client by account number  
- Store and retrieve client data from a text file  
- **Perform transactions:**
  - Deposit to account  
  - Withdraw from account  
  - Show list of balances  
  - Return to main menu  

---

## 📦 Data Format

Each line in `Clients.txt` represents a client record in the following format:

```
AccountNumber#//#PinCode#//#FullName#//#Phone#//#Balance
```

Example:
```
12345#//#5678#//#John Doe#//#0501234567#//#5000.00
```
