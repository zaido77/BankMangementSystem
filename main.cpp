#include <string>
#include <iomanip> //setw
#include <vector>  // vector
#include <cctype>  // to is upper, lower, digit, punct
#include <fstream> // Files
#include "Libraries\Inputs.h"
#include "Libraries\Utils.h"
#include "Libraries\String.h"
#include <iostream>
using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenu();
void ShowTransactionsMenu();

enum enMainMenuOption 
{ 
    eShowClientsList = 1, eAddNewClients = 2, 
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eExit = 7 
};

enum enTranactionMenuOption
{
    eDeposit = 1, eWithdraw = 2,
    eTotalBalances = 3, eMainMenu = 4
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

short ReadMainMenuOption()
{
    short Choice = 0;

    do
    {
        cout << "Choose what do you want to do? [1 to 7]? " ;
        cin >> Choice;
    } while (Choice < 1 || Choice > 7);

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

void ShowAllClientDetailsLine(const stClient& Client)
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
    cout << "| " << setw(9)  << "Pin Code";
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
            ShowAllClientDetailsLine(Client);
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

void ShowEndScreen()
{
    cout << "-------------------------------\n";
    cout << "        Program Ends :)\n";
    cout << "-------------------------------\n";
}

void ShowClientBalanceLine(const stClient& Client)
{
    cout << "| " << setw(15) << Client.AccountNumber;
    cout << "| " << setw(32) << Client.FullName;
    cout << "| " << setw(12) << Client.AccountBalance;

    cout << "\n";
}

double TotalBalances(const vector<stClient>& vClients)
{
    double TotalBalances = 0.0;

    for (const stClient& Client : vClients)
    {
        TotalBalances += Client.AccountBalance;
    }

    return TotalBalances;
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

    if (vClients.size() == 0)
    {
        cout << Utils::Tabs(3) << "(No Clients Availabe In The System)\n";
    }
    else
    {
        for (const stClient& Client : vClients)
        {
            ShowClientBalanceLine(Client);
        }
    }
    cout << "\n__________________________________________________________________________________________\n\n";
    cout << Utils::Tabs(4) << "Total Balances = " << TotalBalances(vClients) << "\n\n";
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

void GoBackToTransactionsMenu()
{
    cout << "Press any key to go back to Transactions Menu . . ." << endl;
    system("pause>0");
    ShowTransactionsMenu();
}

bool WithdrawClientBalance(string AccountNumber, double WithdrawAmount, vector<stClient>& vClients)
{
    for (stClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.AccountBalance -= WithdrawAmount;
            return true;
        }
    }

    return false;
}

void PerformWithdrawTransaction(vector<stClient>& vClients)
{
    stClient Client;
    string AccountNumber = Inputs::ReadText("Enter Account Number : ");
    
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber;
        cout << "] is not found.\n\n";

        AccountNumber = Inputs::ReadText("Enter Account Number : ");
    }

    double WithdrawAmount = 0;
    char WantToWithdraw = 'n';
    
    PrintClientCard(Client);
    
    WithdrawAmount = Inputs::ReadPositiveDoubleNumber("\nEnter withdraw amount: ");

    while (WithdrawAmount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << "\n";
        WithdrawAmount = Inputs::ReadPositiveDoubleNumber("\nEnter another withdraw amount: ");
    }

    cout << "\nAre you sure you want to perform this transaction? [Y/N]? ";
    cin >> WantToWithdraw;

    if (toupper(WantToWithdraw) == 'Y')
    {
        WithdrawClientBalance(AccountNumber, WithdrawAmount, vClients);

        cout << "\nDone Succesfully, New Balance is: " << Client.AccountBalance - WithdrawAmount << "\n";

        SaveClientsDataToFile(ClientsFileName, vClients);
    }
}

void ShowWithdrawScreen()
{
    cout << "-------------------------------\n";
    cout << "        Withdraw Screen\n";
    cout << "-------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    PerformWithdrawTransaction(vClients);
}

bool DepositClientBalance(string AccountNumber, double DepositAmount, vector<stClient>& vClients)
{
    for (stClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.AccountBalance += DepositAmount;
            return true;
        }
    }

    return false;
}

void PerformDepositTransaction(vector<stClient>& vClients)
{
    stClient Client;
    string AccountNumber = Inputs::ReadText("Enter Account Number : ");
    
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber;
        cout << "] is not found.\n\n";

        AccountNumber = Inputs::ReadText("Enter Account Number : ");
    }

    double DepositAmount = 0;
    char WantToDeposit = 'n';
    
    PrintClientCard(Client);
    DepositAmount = Inputs::ReadPositiveDoubleNumber("\nEnter deposit amount: ");

    cout << "\nAre you sure you want to perform this transaction? [Y/N]? ";
    cin >> WantToDeposit;

    if (toupper(WantToDeposit) == 'Y')
    {
        DepositClientBalance(AccountNumber, DepositAmount, vClients);

        cout << "\nDone Succesfully, New Balance is: " << Client.AccountBalance + DepositAmount << "\n";

        SaveClientsDataToFile(ClientsFileName, vClients);
    }
}

void ShowDepositScreen()
{
    cout << "-------------------------------\n";
    cout << "        Deposit Screen\n";
    cout << "-------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    PerformDepositTransaction(vClients);
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

void PreformMainMenuOption(enMainMenuOption MainMenuOption)
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
    case enMainMenuOption::eExit:
        system("cls");
        ShowEndScreen();
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
    cout << "        [7] Exit\n";
    cout << "===========================================\n";
    PreformMainMenuOption((enMainMenuOption)ReadMainMenuOption());
}

int main()
{
    ShowMainMenu();

    return 0;
}