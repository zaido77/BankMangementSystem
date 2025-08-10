# 🏦 Bank Management System (Version 1.2)

A simple console-based C++ application that allows you to manage bank clients using basic file storage. This is the first version of the system, focusing on core **client management features**, basic **transactions menu** and now includes system users with permission-based access control.

---

## 📌 Features

### 👥 Client Management
- View all clients  
- Add new clients  
- Update existing clients  
- Delete clients  
- Find client by account number  
- Store and retrieve client data from a text file  

### 💰 Transactions
- Deposit to account  
- Withdraw from account  
- Show list of balances  
- Return to main menu  

### 🔑 User Management (NEW)
- View all system users  
- Add new users  
- Update existing users  
- Delete users (except the **Admin** account)  
- Find user by username  
- Store and retrieve user data from a text file  

### 🛡 Permissions System (NEW)
- Full access or **custom permissions** per user  
- Permissions include:
  1. Show Clients List  
  2. Add New Client  
  3. Delete Client  
  4. Update Client  
  5. Find Client  
  6. Transactions Menu  
  7. Manage Users  
- Users without required permissions will see an **Access Denied** screen.

### 🔐 Login System
- Username and password authentication  
- Only valid users can access the system  
- Permissions are enforced after login

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
