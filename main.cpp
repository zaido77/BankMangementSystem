#include <string>
#include <iomanip> //setw
#include <vector>  // vector
#include <cctype>  // to is upper, lower, digit, punct
#include <fstream> // Files
#include <iostream>
using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMode { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Exit = 6 };

struct stClient
{
    string AccountNumber;
    string PinCode;
    string FullName;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

void ShowHeader(enMode Mode) 
{
    system("cls");

    string Header = "";

    switch (Mode)
    {
    case enMode::Add:
        Header = "Add New Clients Screen";
        break;
    case enMode::Delete:
        Header = "Delete Client Screen";
        break;
    case enMode::Update:
        Header = "Update Client Screen";
        break;
    case enMode::Find:
        Header = "Find Client Screen";
        break;
    case enMode::Exit:
        Header = "Program End :)";
        break;
    }

    cout << "-------------------------------\n";
    cout << "    " << Header << "\n";
    cout << "-------------------------------\n";
}

void ShowMainMenueScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "             Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "        [1] Show Clients List\n";
    cout << "        [2] Add New Client\n";
    cout << "        [3] Delete Client\n";
    cout << "        [4] Update Client\n";
    cout << "        [5] Find Client\n";
    cout << "        [6] Exit\n";
    cout << "===========================================\n";
    //cout << "Choose what do you want to do? [1 to 6]? ";
}

string ReadText(string Message = "Enter a Text?\n")
{
    string Text = "";
    cout << Message;
    getline(cin >> ws, Text);

    return Text;
}

int ReadNumberInRange(string Message, int From, int To)
{
    int Number;
    cout << Message << " [" << From << " - " << To << "]? ";
    cin >> Number;

    while (cin.fail() || Number < From || Number > To)
    {
        cin.clear();
        cin.ignore();

        cout << "Invalid Number, " << Message << " [" << From << " - " << To << "]? ";
        cin >> Number;
    }

    return Number;

}

double ReadPositiveDoubleNumber(string Message = "Enter a Positive Number?\n")
{
    double Number;
    cout << Message;
    cin >> Number;

    while (cin.fail() || Number <= 0)
    {
        cin.clear();
        cin.ignore(); //std::numeric_limits<std::streamsize>::max(), '\n'

        cout << "Invalid Number, " << Message;
        cin >> Number;
    }

    return Number;
}

void GoBackToMainMenue()
{
    cout << "Press any key to go back to Main Menu . . ." << endl;
    system("pause>0");
}

string Tabs(short NumberOfTabs)
{
    string t = "";

    for (short i = 0; i < NumberOfTabs; i++)
        t += "\t";

    return t;
}

vector<string> SplitString(string Str, string Delim)
{
    vector<string> vString;

    short pos = 0;
    string Word;

    while ((pos = Str.find(Delim)) != string::npos)
    {
        Word = Str.substr(0, pos);

        if (Word != "")
            vString.push_back(Word);

        Str.erase(0, pos + Delim.length());
    }

    if (Str != "")
        vString.push_back(Str);

    return vString;
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
    vector<string> vClientRecord = SplitString(Line, Delim);

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

stClient ReadNewClient(vector<stClient>& vClients)
{
    stClient Client;
    stClient ClientAlReadyExist;

    Client.AccountNumber = ReadText("Enter Account Number : ");

    while (FindClientByAccountNumber(Client.AccountNumber, vClients, ClientAlReadyExist))
    {
        cout << "\nClient with Account Number [" << ClientAlReadyExist.AccountNumber;
        cout << "] already exist.\n\n";

        Client.AccountNumber = ReadText("Enter another Account Number : ");
    }

    Client.PinCode        = ReadText("Enter Pin Code       : ");
    Client.FullName       = ReadText("Enter Full Name      : ");
    Client.Phone          = ReadText("Enter Phone          : ");
    Client.AccountBalance = ReadPositiveDoubleNumber("Enter Account Balance: ");

    return Client;
}

void PrintClientCard(const stClient& Client)
{
    cout << "--------------------------------------------------\n";
    cout << "Account Number  :" << Client.AccountNumber << "\n";
    cout << "Pin Code        :" << Client.PinCode << "\n";
    cout << "Full Name       :" << Client.FullName << "\n";
    cout << "Phone           :" << Client.Phone << "\n";
    cout << "Account Balance :" << Client.AccountBalance << "\n";
    cout << "--------------------------------------------------\n";
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

void AddNewClient(vector<stClient>& vClients)
{
    stClient Client = ReadNewClient(vClients);
    vClients.push_back(Client);
    AddClientDataLineToFile(ClientsFileName, ConvertClientRecordToLine(Client));

}

void AddClients(vector<stClient>& vClients)
{
    char AddMore = 'Y';

    do
    {
        ShowHeader(enMode::Add);
        cout << "Adding New Client:\n\n";
        AddNewClient(vClients);

        cout << "\nClient Added Succesfully, Do you want to add more clients? [Y/N]? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

    GoBackToMainMenue();
}

void ShowClient(const stClient& Client)
{
    cout << "| " << setw(17) << Client.AccountNumber;
    cout << "| " << setw(11) << Client.PinCode;
    cout << "| " << setw(22) << Client.FullName;
    cout << "| " << setw(14) << Client.Phone;
    cout << "| " << setw(12) << Client.AccountBalance;

    cout << "\n";
}

void ShowClientsList(const vector<stClient>& vClients)
{
    system("cls");

    cout << Tabs(4) << "Clients List (" << vClients.size() << ") Client(s).\n";
    cout << "\n__________________________________________________________________________________________\n\n";
    cout << left;
    cout << "| " << setw(17) << "Account Number";
    cout << "| " << setw(11) << "Pin Code";
    cout << "| " << setw(22) << "Client Name";
    cout << "| " << setw(14) << "Phone";
    cout << "| " << setw(12) << "Balance";
    cout << "\n__________________________________________________________________________________________\n\n";

    for (const stClient& Client : vClients)
    {
        ShowClient(Client);
    }
    
    cout << "\n__________________________________________________________________________________________\n\n";

    GoBackToMainMenue();
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
            GoBackToMainMenue();
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
        GoBackToMainMenue();
        return false;
    }

}

stClient ChangeClientDetails(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;
    Client.PinCode = ReadText("\nEnter Pin Code       : ");
    Client.FullName = ReadText("Enter Full Name      : ");
    Client.Phone = ReadText("Enter Phone          : ");
    Client.AccountBalance = ReadPositiveDoubleNumber("Enter Account Balance: ");

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
            GoBackToMainMenue();
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
        GoBackToMainMenue();
        return false;
    }

    GoBackToMainMenue();
}

void FindClientScreen(string AccountNumber, vector<stClient>& vClients)
{
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nThe following are the client details:\n";
        PrintClientCard(Client);
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
    }

    GoBackToMainMenue();
}

void ApplyUserChoice(enMode UserChoice, vector<stClient>& vClients)
{
    switch (UserChoice)
    {
    case enMode::Show:
        ShowClientsList(vClients);
        break;
    case enMode::Add:
        AddClients(vClients);
        break;
    case enMode::Delete:
        ShowHeader(enMode::Delete);
        DeleteClientByAccountNumber(ReadText("Enter Account Number: "), vClients);
        break;
    case enMode::Update:
        ShowHeader(enMode::Update);
        UpdateClientByAccountNumber(ReadText("Enter Account Number: "), vClients);
        break;
    case enMode::Find:
        ShowHeader(enMode::Find);
        FindClientScreen(ReadText("Enter Account Number: "), vClients);
        break;
    }
}

void StartProgram()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    enMode UserChoice;

    do
    {
        ShowMainMenueScreen();
        UserChoice = (enMode)ReadNumberInRange("Choose what do you want to do ?", 1, 6);

        ApplyUserChoice(UserChoice, vClients);

    } while (UserChoice != enMode::Exit);
    
    ShowHeader(enMode::Exit);
    system("pause>0");
}

int main()
{
    StartProgram();

    return 0;
}