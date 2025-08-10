#include <string>
#include <cmath>
#include <iomanip> //setw
#include <vector>  // vector
#include <cctype>  // to is upper, lower, digit, punct
#include <fstream> // Files
#include "C:\Users\User\source\repos\Testing\Testing\Inputs.h"
#include "C:\Users\User\source\repos\Testing\Testing\Utils.h"
#include "C:\Users\User\source\repos\Testing\Testing\String.h"
#include <iostream>
using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu();
void Login();

enum enMainMenuOption
{
    eShowClientsList = 1, eAddNewClients = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eManageUsers = 7, eLogout = 8
};

enum enTranactionMenuOption
{
    eDeposit = 1, eWithdraw = 2,
    eTotalBalances = 3, eMainMenu = 4
};

enum enManageUsersMenuOption
{
    eListUsers = 1, eAddNewUser = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, MainMenu = 6
};

struct stClient
{
    string AccountNumber;
    string PinCode;
    string FullName;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

struct stUser
{
    string Username;
    string Password;
    short Permissions;
    bool MarkForDelete = false;
};

stUser CurrentUser;

//============================== Client ==================================

short ReadMainMenuOption()
{
    short Choice = 0;

    do
    {
        cout << "Choose what do you want to do? [1 to 8]? ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 8);

    return Choice;
}

string ConvertClientRecordToLine(const stClient& Client, string Delim = "#//#")
{
    string Line = "";

    Line += Client.AccountNumber + Delim;
    Line += Client.PinCode + Delim;
    Line += Client.FullName + Delim;
    Line += Client.Phone + Delim;
    Line += to_string(Client.AccountBalance);

    return Line;
}

stClient ConvertLineToClientRecord(string Line, string Delim = "#//#")
{
    vector<string> vClientRecord = String::SplitString(Line, Delim);

    stClient Client;

    Client.AccountNumber = vClientRecord[0];
    Client.PinCode = vClientRecord[1];
    Client.FullName = vClientRecord[2];
    Client.Phone = vClientRecord[3];
    Client.AccountBalance = stod(vClientRecord[4]);

    return Client;
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient>& vClients, stClient& Client)
{
    for (const stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        stClient Client;
        string Line = "";

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToClientRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }

    return false;
}

stClient ReadNewClient()
{
    stClient Client;

    Client.AccountNumber = Inputs::ReadText("Enter Account Number : ");

    while (ClientExistByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with Account Number [" << Client.AccountNumber;
        cout << "] already exist.\n\n";

        Client.AccountNumber = Inputs::ReadText("Enter another Account Number : ");
    }

    Client.PinCode = Inputs::ReadText("Enter Pin Code       : ");
    Client.FullName = Inputs::ReadText("Enter Full Name      : ");
    Client.Phone = Inputs::ReadText("Enter Phone          : ");
    Client.AccountBalance = Inputs::ReadPositiveDoubleNumber("Enter Account Balance: ");

    return Client;
}

void PrintClientCard(const stClient& Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "--------------------------------------------\n";
    cout << "Account Number  :" << Client.AccountNumber << "\n";
    cout << "Pin Code        :" << Client.PinCode << "\n";
    cout << "Full Name       :" << Client.FullName << "\n";
    cout << "Phone           :" << Client.Phone << "\n";
    cout << "Account Balance :" << Client.AccountBalance << "\n";
    cout << "--------------------------------------------\n";
}

void SaveClientsDataToFile(string FileName, vector<stClient>& vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        string ClientDataLine = "";

        for (const stClient& Client : vClients)
        {
            if (Client.MarkForDelete == false)
            {
                ClientDataLine = ConvertClientRecordToLine(Client);
                MyFile << ClientDataLine << "\n";
            }
        }

        MyFile.close();
    }

}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    vector<stClient> vClients;

    if (MyFile.is_open())
    {
        string Line = "";
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToClientRecord(Line);
            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

void AddClientDataLineToFile(string FileName, string ClientDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << ClientDataLine << "\n";

        MyFile.close();
    }
}

void AddNewClient()
{
    stClient Client = ReadNewClient();
    AddClientDataLineToFile(ClientsFileName, ConvertClientRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';

    do
    {
        cout << "\nAdding New Client:\n";
        cout << "------------------\n";
        AddNewClient();

        cout << "\nClient Added Succesfully, Do you want to add more clients? [Y/N]? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
    cout << "-------------------------------\n";
    cout << "    Add New Clients Screen\n";
    cout << "-------------------------------\n";

    AddNewClients();
}

void ShowClientDetailsLine(const stClient& Client)
{
    cout << "| " << setw(15) << Client.AccountNumber;
    cout << "| " << setw(9) << Client.PinCode;
    cout << "| " << setw(32) << Client.FullName;
    cout << "| " << setw(12) << Client.Phone;
    cout << "| " << setw(12) << Client.AccountBalance;

    cout << "\n";
}

void ShowAllClientsScreen()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n" << Utils::Tabs(4) << "Clients List (" << vClients.size() << ") Client(s).\n";
    cout << "\n__________________________________________________________________________________________\n\n";
    cout << left;
    cout << "| " << setw(15) << "Account Number";
    cout << "| " << setw(9) << "Pin Code";
    cout << "| " << setw(32) << "Client Name";
    cout << "| " << setw(12) << "Phone";
    cout << "| " << setw(12) << "Balance";
    cout << "\n__________________________________________________________________________________________\n\n";

    if (vClients.size() == 0)
    {
        cout << Utils::Tabs(3) << "(No Clients Availabe In The System)\n";
    }
    else
    {
        for (const stClient& Client : vClients)
        {
            ShowClientDetailsLine(Client);
        }
    }

    cout << "__________________________________________________________________________________________\n\n";
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
    for (stClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
    stClient Client;
    char WantToDelete = 'Y';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nThe following are the client details:\n";
        PrintClientCard(Client);

        cout << "\nAre you sure you want to Delete this client? [Y/N]? ";
        cin >> WantToDelete;

        if (toupper(WantToDelete) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            vClients = LoadClientsDataFromFile(ClientsFileName);

            cout << "\nClient Deleted Succesfully!\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
        return false;
    }
}

void ShowDeleteClientScreen()
{
    cout << "-------------------------------\n";
    cout << "     Delete Client Screen\n";
    cout << "-------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = Inputs::ReadText("Enter Account Number: ");

    DeleteClientByAccountNumber(AccountNumber, vClients);
}

stClient ChangeClientDetails(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;
    Client.PinCode = Inputs::ReadText("\nEnter Pin Code       : ");
    Client.FullName = Inputs::ReadText("Enter Full Name      : ");
    Client.Phone = Inputs::ReadText("Enter Phone          : ");
    Client.AccountBalance = Inputs::ReadPositiveDoubleNumber("Enter Account Balance: ");

    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
    stClient Client;
    char WantToUpdate = 'Y';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nThe following are the client details:\n";
        PrintClientCard(Client);

        cout << "\nAre you sure you want to Update this client? [Y/N]? ";
        cin >> WantToUpdate;

        if (toupper(WantToUpdate) == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientDetails(AccountNumber);
                    break;
                }
            }

            SaveClientsDataToFile(ClientsFileName, vClients);

            cout << "\nClient Updated Succesfully!\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
        return false;
    }
}

void ShowUpdateClientScreen()
{
    cout << "-------------------------------\n";
    cout << "     Update Client Screen\n";
    cout << "-------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = Inputs::ReadText("Enter Account Number: ");

    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
    cout << "-------------------------------\n";
    cout << "     Find Client Screen\n";
    cout << "-------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    stClient Client;
    string AccountNumber = Inputs::ReadText("Enter Account Number: ");

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
}

void GoBackToMainMenu()
{
    cout << "\nPress Any Key To Go Back To Main Menu . . ." << endl;
    system("pause>0");
    ShowMainMenu();
}

void GoBackToTransactionsMenu()
{
    cout << "\nPress any key to go back to Transactions Menu . . ." << endl;
    system("pause>0");
    ShowTransactionsMenu();
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient>& vClients)
{
    char Answer = 'n';

    cout << "\nAre you sure you want to perform this transaction? [Y/N]? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (stClient& Client : vClients)
        {
            if (Client.AccountNumber == AccountNumber)
            {
                Client.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\nDone Succesfully, New Balance is: " << Client.AccountBalance << "\n";
                return true;
            }
        }

        return false;
    }

}

void ShowDepositScreen()
{
    cout << "-------------------------------\n";
    cout << "        Deposit Screen\n";
    cout << "-------------------------------\n";

    stClient Client;

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = Inputs::ReadText("Enter Account Number : ");

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber;
        cout << "] is not found.\n\n";

        AccountNumber = Inputs::ReadText("Enter Account Number : ");
    }

    PrintClientCard(Client);

    double Amount = 0;
    Amount = Inputs::ReadPositiveDoubleNumber("\nEnter deposit amount: ");

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen()
{
    cout << "-------------------------------\n";
    cout << "        Withdraw Screen\n";
    cout << "-------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    stClient Client;
    string AccountNumber = Inputs::ReadText("Enter Account Number : ");

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber;
        cout << "] is not found.\n\n";

        AccountNumber = Inputs::ReadText("Enter Account Number : ");
    }

    PrintClientCard(Client);

    double Amount = 0;
    Amount = Inputs::ReadPositiveDoubleNumber("\nEnter withdraw amount: ");

    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << "\n";
        Amount = Inputs::ReadPositiveDoubleNumber("\nEnter another withdraw amount: ");
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowClientBalanceLine(const stClient& Client)
{
    cout << "| " << setw(15) << Client.AccountNumber;
    cout << "| " << setw(32) << Client.FullName;
    cout << "| " << setw(12) << Client.AccountBalance;

    cout << "\n";
}

void ShowTotalBalancesList()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n" << Utils::Tabs(4) << "Balances List (" << vClients.size() << ") Clients(s).\n";
    cout << "\n__________________________________________________________________________________________\n\n";
    cout << left;
    cout << "| " << setw(15) << "Account Number";
    cout << "| " << setw(32) << "Client Name";
    cout << "| " << setw(12) << "Balance";
    cout << "\n__________________________________________________________________________________________\n\n";

    double TotalBalances = 0;
    if (vClients.size() == 0)
    {
        cout << Utils::Tabs(3) << "(No Clients Availabe In The System)\n";
    }
    else
    {
        for (const stClient& Client : vClients)
        {
            TotalBalances += Client.AccountBalance;
            ShowClientBalanceLine(Client);
        }
    }
    cout << "__________________________________________________________________________________________\n\n";
    cout << Utils::Tabs(4) << "Total Balances = " << TotalBalances << "\n\n";
}

short ReadTransactionsMenuOption()
{
    short Choice = 0;

    do
    {
        cout << "Choose what do you want to do? [1 to 4]? ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 4);

    return Choice;
}

void PerformTransactionsMenuOption(enTranactionMenuOption TransactionsMenuOption)
{
    switch (TransactionsMenuOption)
    {
    case enTranactionMenuOption::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;
    case enTranactionMenuOption::eWithdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    case enTranactionMenuOption::eTotalBalances:
        system("cls");
        ShowTotalBalancesList();
        GoBackToTransactionsMenu();
        break;
    case enTranactionMenuOption::eMainMenu:
        ShowMainMenu();
        break;
    }
}

void ShowTransactionsMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "         Transactions Menu Screen\n";
    cout << "===========================================\n";
    cout << "        [1] Deposit\n";
    cout << "        [2] Withdraw\n";
    cout << "        [3] Total Balances\n";
    cout << "        [4] Main Menu\n";
    cout << "===========================================\n";
    PerformTransactionsMenuOption((enTranactionMenuOption)ReadTransactionsMenuOption());
}

//============================== User ==================================

void GoBackToManageUsersMenu()
{
    cout << "\nPress any key to go back to Manage Users Menu . . ." << endl;
    system("pause>0");
    ShowManageUsersMenu();
}

string ConvertUserRecordToLine(stUser User, string Delim = "#//#")
{
    return User.Username + Delim + User.Password + Delim + to_string(User.Permissions);
}

stUser ConvertLineToUserRecord(string Line, string Delim = "#//#")
{
    stUser User;

    vector<string> vUserRecord = String::SplitString(Line, Delim);

    User.Username = vUserRecord[0];
    User.Password = vUserRecord[1];
    User.Permissions = stoi(vUserRecord[2]);

    return User;
}

void AddUserDataLineToFile(string FileName, string UserDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << UserDataLine << "\n";

        MyFile.close();
    }
}

vector<stUser> LoadUsersDataFromFile(string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    vector<stUser> vUsers;

    if (MyFile.is_open())
    {
        string Line = "";
        stUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToUserRecord(Line);
            vUsers.push_back(User);
        }

        MyFile.close();
    }

    return vUsers;
}

void SaveUsersDataToFile(string FileName, const vector<stUser>& vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        string UserDataLine = "";

        for (const stUser& User : vUsers)
        {
            if (User.MarkForDelete == false)
            {
                UserDataLine = ConvertUserRecordToLine(User);
                MyFile << UserDataLine << "\n";
            }
        }

        MyFile.close();
    }
}

short ReadManageUsersMenuOption()
{
    short Choice = 0;

    do
    {
        cout << "Choose what do you want to do? [1 to 6]? ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 6);

    return Choice;
}

void ShowUserDetailsLine(const stUser& User)
{
    cout << "| " << setw(15) << User.Username;
    cout << "| " << setw(15) << User.Password;
    cout << "| " << setw(12) << User.Permissions;

    cout << "\n";
}

void ShowAllUsersScreen()
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n" << Utils::Tabs(4) << "Users List (" << vUsers.size() << ") User(s).\n";
    cout << "\n__________________________________________________________________________________________\n\n";
    cout << left;
    cout << "| " << setw(15) << "Username";
    cout << "| " << setw(15) << "Password";
    cout << "| " << setw(12) << "Permissions";
    cout << "\n__________________________________________________________________________________________\n\n";

    for (const stUser& User : vUsers)
    {
        ShowUserDetailsLine(User);
    }

    cout << "__________________________________________________________________________________________\n\n";
}

short ReadPermissions()
{
    char HaveFullAccess = 'n';
    cout << "\nDo you want to give full access? [Y/N]? ";
    cin >> HaveFullAccess;

    if (toupper(HaveFullAccess) == 'Y')
        return -1;


    char Answer = 'n';
    short Permissions = 0;

    cout << "\nDo you want to give access to:\n";

    Answer = Inputs::ReadChar("\nShow Clients List? [Y/N]? ");
    Permissions |= ((toupper(Answer) == 'Y') ? 0b1 : 0);

    Answer = Inputs::ReadChar("\nAdd New Client? [Y/N]? ");
    Permissions |= ((toupper(Answer) == 'Y') ? 0b10 : 0);

    Answer = Inputs::ReadChar("\nDelete Client? [Y/N]? ");
    Permissions |= ((toupper(Answer) == 'Y') ? 0b100 : 0);

    Answer = Inputs::ReadChar("\nUpdate Client? [Y/N]? ");
    Permissions |= ((toupper(Answer) == 'Y') ? 0b1000 : 0);

    Answer = Inputs::ReadChar("\nFind Client? [Y/N]? ");
    Permissions |= ((toupper(Answer) == 'Y') ? 0b10000 : 0);

    Answer = Inputs::ReadChar("\nTranactions? [Y/N]? ");
    Permissions |= ((toupper(Answer) == 'Y') ? 0b100000 : 0);

    Answer = Inputs::ReadChar("\nManage Users? [Y/N]? ");
    Permissions |= ((toupper(Answer) == 'Y') ? 0b1000000 : 0);

    return Permissions;
}

bool UserExistByUsername(string Username, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        stUser User;
        string Line = "";

        while (getline(MyFile, Line))
        {
            User = ConvertLineToUserRecord(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }

    return false;
}

stUser ReadNewUser()
{
    stUser User;

    User.Username = Inputs::ReadText("Enter Username: ");

    while (UserExistByUsername(User.Username, UsersFileName))
    {
        cout << "\nUser with Username [" << User.Username;
        cout << "] already exist.\n\n";

        User.Username = Inputs::ReadText("Enter another Username: ");
    }

    User.Password = Inputs::ReadText("Enter Password: ");
    User.Permissions = ReadPermissions();

    return User;
}

void AddNewUser()
{
    stUser User = ReadNewUser();
    AddUserDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
    char AddMore = 'Y';

    do
    {
        cout << "\nAdding New User:\n";
        AddNewUser();

        cout << "\nUser Added Succesfully, Do you want to add more users? [Y/N]? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewUsersScreen()
{
    cout << "-------------------------------\n";
    cout << "    Add New Users Screen\n";
    cout << "-------------------------------\n";

    AddNewUsers();
}

void PrintUserCard(const stUser& User)
{
    cout << "\nThe following are the user details:\n";
    cout << "----------------------------------------\n";
    cout << "Username    : " << User.Username << "\n";
    cout << "Password    : " << User.Password << "\n";
    cout << "Permissions : " << User.Permissions << "\n";
    cout << "----------------------------------------\n";
}

bool FindUserByUsername(string Username, vector<stUser>& vUsers, stUser& User)
{
    for (stUser& U : vUsers)
    {
        if (U.Username == Username)
        {
            User = U;
            return true;
        }
    }

    return false;
}

bool MarkUserForDeleteByUsername(string Username, vector<stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.Username == Username)
        {
            U.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

bool DeleteUserByUsername(string Username, vector<stUser>& vUsers)
{
    char Answer = 'n';
    stUser User;

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\nAre you sure you want to Delete this user? [Y/N]? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\nUser Deleted Succesfully!\n";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username [" << Username << "] is Not Found!\n";
        return false;
    }
}

void ShowDeleteUserScreen()
{
    cout << "-------------------------------\n";
    cout << "      Delete User Screen\n";
    cout << "-------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = Inputs::ReadText("Enter Username: ");

    if (Username == "Admin")
    {
        cout << "\nYou cannot Delete this User.\n";
        return;
    }

    DeleteUserByUsername(Username, vUsers);
}

stUser ChangeUserDetails(string Username)
{
    stUser User;

    User.Username = Username;
    User.Password = Inputs::ReadText("Enter Password: ");
    User.Permissions = ReadPermissions();

    return User;
}

bool UpdateUserByUsername(string Username, vector<stUser>& vUsers)
{
    char Answer = 'n';
    stUser User;

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\nAre you sure you want to Update this user? [Y/N]? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (stUser& U : vUsers)
            {
                if (U.Username == Username)
                {
                    U = ChangeUserDetails(Username);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\nUser Updated Succesfully!\n";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username [" << Username << "] is Not Found!\n";
        return false;
    }
}

void ShowUpdateUserScreen()
{
    cout << "-------------------------------\n";
    cout << "      Update User Screen\n";
    cout << "-------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = Inputs::ReadText("Enter Username: ");

    if (Username == "Admin")
    {
        cout << "\nYou cannot Update this User.\n";
        return;
    }

    UpdateUserByUsername(Username, vUsers);
}

void ShowFindUserScreen()
{
    cout << "-------------------------------\n";
    cout << "      Find User Screen\n";
    cout << "-------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUser User;
    string Username = Inputs::ReadText("Enter Username: ");

    if (FindUserByUsername(Username, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nUser with Username [" << Username << "] is Not Found!\n";
}

void PerformManageUsersMenuOption(enManageUsersMenuOption ManageUsersMenuOption)
{
    switch (ManageUsersMenuOption)
    {
    case enManageUsersMenuOption::eListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eAddNewUser:
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::MainMenu:
        ShowMainMenu();
        break;
    }
}

void ShowManageUsersMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "         Manage Users Menu Screen\n";
    cout << "===========================================\n";
    cout << "        [1] List Users\n";
    cout << "        [2] Add New User\n";
    cout << "        [3] Delete User\n";
    cout << "        [4] Update User\n";
    cout << "        [5] Find User\n";
    cout << "        [6] Main Menu\n";
    cout << "===========================================\n";
    PerformManageUsersMenuOption((enManageUsersMenuOption)ReadManageUsersMenuOption());
}

void ShowAcessDeniedScreen()
{
    cout << "------------------------------------\n";
    cout << "Access Denied,\n";
    cout << "You dont have Permission to do this,\n";
    cout << "Please Contact you Admin\n";
    cout << "------------------------------------\n\n";
}

bool IsNotAllowedToPerfromOption(enMainMenuOption MainMenuOption, const stUser& User)
{
    short OptionBinary = pow(2, MainMenuOption - 1);

    return ((OptionBinary & User.Permissions) == 0b0) ? true : false;
}

void PerformMainMenuOption(enMainMenuOption MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOption::eShowClientsList:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eAddNewClients:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOption::eTransactions:
        system("cls");
        ShowTransactionsMenu();
    case enMainMenuOption::eManageUsers:
        system("cls");
        ShowManageUsersMenu();
    case enMainMenuOption::eLogout:
        Login();
        break;
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "             Main Menu Screen\n";
    cout << "===========================================\n";
    cout << "        [1] Show Clients List\n";
    cout << "        [2] Add New Client\n";
    cout << "        [3] Delete Client\n";
    cout << "        [4] Update Client\n";
    cout << "        [5] Find Client\n";
    cout << "        [6] Transactions\n";
    cout << "        [7] Manage Users\n";
    cout << "        [8] Logout\n";
    cout << "===========================================\n";
    enMainMenuOption MainMenuOption = (enMainMenuOption)ReadMainMenuOption();

    if ((MainMenuOption != 8) && IsNotAllowedToPerfromOption(MainMenuOption, CurrentUser))
    {
        system("cls");
        ShowAcessDeniedScreen();
        GoBackToMainMenu();
    }

    PerformMainMenuOption(MainMenuOption);
}

stUser ReadUserLogin()
{
    stUser User;

    User.Username = Inputs::ReadText("Enter Username: ");
    User.Password = Inputs::ReadText("Enter Password: ");

    return User;
}

bool IsUserExistInSystem(stUser& User, const vector<stUser>& vUsers)
{
    for (const stUser& U : vUsers)
    {
        if ((U.Username == User.Username) && (U.Password == User.Password))
        {
            User.Permissions = U.Permissions;
            return true;
        }
    }

    return false;
}

void ShowLoginHeader()
{
    system("cls");
    cout << "-------------------------------\n";
    cout << "        Login Screen\n";
    cout << "-------------------------------\n";
}

void Login()
{
    stUser User;
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    ShowLoginHeader();
    User = ReadUserLogin();

    while (!IsUserExistInSystem(User, vUsers))
    {
        ShowLoginHeader();
        cout << "Invalid Username/Password!\n";
        User = ReadUserLogin();
    }

    CurrentUser = User;

    ShowMainMenu();
}

int main()
{
    Login();

    return 0;
}