#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;


struct strUser {
	string UserName;
	string Password;
	int Permissions;
	bool MarkForDelete = false;
};


enum enMainMenuPermissions { eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64 };
enum enTransactionsMenuOptions { eDeposit = 1, eWithdraw = 2, eTotalBalance = 3, eMainMenuBack = 4 };
enum enManageUsersMenuOptions { eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenu = 6 };
enum enMainMenuOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogout = 8 };


const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";


strUser CurrentUser;


void ShowMainMenu();
void GoBackToMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu(); 
bool CheckAccessPermission(enMainMenuPermissions Permission);
void Login();


struct strClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};


vector <string> SplitString(string S1, string Delim)
{
	vector <string> vString;
	size_t pos = 0;
	string sWord;// define a string variable  
	// use find() function to get the position of the delimiters  
	while ((pos = S1.find(Delim)) != std::string::npos) {
		sWord = S1.substr(0, pos);// store the word   
		if (sWord != "") {
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
	}
	if (S1 != "") {
		vString.push_back(S1); // it adds last word of the string.    
	}
	return vString;
}



strClient ConvertLineToRecord(string Line, string Separator = "#//#") {
	strClient Client;
	vector <string> vClientData;
	vClientData = SplitString(Line, Separator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	return Client;
}
string ConvertRecordToLine(strClient Client, string Separator = "#//#") {
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}


strUser ConvertUserLineToRecord(string Line, string Separator = "#//#") 
{ 
	strUser User; 
	vector<string> vUserData;     
	vUserData = SplitString(Line, Separator);     
	User.UserName = vUserData[0];     
	User.Password = vUserData[1];     
	User.Permissions = stoi(vUserData[2]); 
	return User; 
}
strUser ConvertUserLineToRecord2(string Line, string Separator = "#//#") { 
	strUser User; 
	vector<string> vUserData;     
	vUserData = SplitString(Line, Separator);     
	User.UserName = vUserData[0];     
	User.Password = vUserData[1];     
	User.Permissions = stoi(vUserData[2]); 
	return User; 
}
string ConvertUserRecordToLine(strUser User, string Seperator = "#//#") { 
	string stClientRecord = "";     
	stClientRecord += User.UserName + Seperator;     
	stClientRecord += User.Password + Seperator;     
	stClientRecord += to_string(User.Permissions); 
	return stClientRecord; 
}




short ReadMenuOption(string Message1, string Message2, short From, short To) {
	short Choice = 0;
	do {
		cout << Message1 << endl;
		cin >> Choice;
		while (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << Message1 << endl;
			cin >> Choice;
		}
	} while (Choice < From || Choice > To);
	return Choice;
}

void ShowAccessDeniedMessage() {
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}
vector <strClient> LoadClientsDataFromFile(string FileName) {
	vector <strClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		strClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}         
		MyFile.close();
	}
	return vClients;
}
void PrintClientRecordLine(strClient Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void ShowAllClientsScreen() {

	if (!CheckAccessPermission(enMainMenuPermissions::pListClients)) 
	{ 
		ShowAccessDeniedMessage(); 
		return; 
	}
	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (strClient Client : vClients) {
			PrintClientRecordLine(Client);
			cout << endl;
		}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}




bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
	vector <strClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open()) {
		string Line;
		strClient Client;
		while (getline(MyFile, Line)) {
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber) {
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}
strClient ReadNewClient() {
	strClient Client;
	cout << "Enter Account Number? ";
	// Usage of std::ws will extract allthe whitespace character    
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}
	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}
void AddDataLineToFile(string FileName, string stDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open()) {
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}
void AddNewClient() {
	strClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}
void AddNewClients() {
	char AddMore = 'Y';
	do {
		//system("cls");
		cout << "Adding New Client:\n\n";
		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}
void ShowAddNewClientsScreen() {

	if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClients)) { 
		ShowAccessDeniedMessage(); 
		return; 
	}
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n-----------------------------------\n";
	AddNewClients();
}




string ReadClientAccountNumber() {
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}
bool FindClientByAccountNumber(string AccountNumber, vector <strClient> vClients, strClient& Client) {
	for (strClient C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			Client = C;
			return true;
		}
	}
	return false;
}
void PrintClientCard(strClient Client) {
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code     : " << Client.PinCode;
	cout << "\nName         : " << Client.Name;
	cout << "\nPhone        : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <strClient>& vClients) {
	for (strClient& C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
vector <strClient> SaveClientsDataToFile(string FileName, vector <strClient> vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open()) {
		for (strClient C : vClients) {
			if (C.MarkForDelete == false) { 
				//we only write records that are not marked for delete.                  
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector <strClient>& vClients) {
	strClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer; 
		if (Answer == 'y' || Answer == 'Y') {
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);
			//Refresh Clients 
			vClients = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}
void ShowDeleteClientScreen() {

	if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient)) { 
		ShowAccessDeniedMessage(); 
		return; 
	}
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";
	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}




strClient ChangeClientRecord(string AccountNumber) {
	strClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}
bool UpdateClientByAccountNumber(string AccountNumber, vector <strClient>& vClients) {
	strClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			for (strClient& C : vClients) {
				if (C.AccountNumber == AccountNumber) {
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Updated Successfully.";
			return true;
		}
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}
void ShowUpdateClientScreen() {

	if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClients)) { 
		ShowAccessDeniedMessage(); 
		return; 
	}
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";

	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}




void ShowFindClientScreen() {

	if (!CheckAccessPermission(enMainMenuPermissions::pFindClient)) { 
		ShowAccessDeniedMessage(); 
		return; 
	}
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";

	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	strClient Client;
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}










bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <strClient>& vClients) {
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		for (strClient& C : vClients) {
			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
				return true;
			}
		}
		return false;
	}
}
void ShowDepositScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen.";
	cout << "\n-----------------------------------\n";
	strClient Client;
	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}




void ShowWithDrawScreen() {
	cout << "\n-----------------------------------\n";     
	cout << "\tWithdraw Screen.";     
	cout << "\n-----------------------------------\n"; 
	strClient Client; vector <strClient> vClients = LoadClientsDataFromFile(ClientsFileName); 
	string AccountNumber = ReadClientAccountNumber(); 
	while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) { 
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";         
		AccountNumber = ReadClientAccountNumber(); 
	}     
	PrintClientCard(Client); 
	double Amount = 0;     
	cout << "\nPlease enter withdraw amount? ";     
	cin >> Amount; 
	//Validate that the amount does not exceeds the balance
	while (Amount > Client.AccountBalance)     
	{         
		cout <<"\nAmount Exceeds the balance, you can withdraw up to : "<< Client.AccountBalance << endl;         
		cout <<"Please enter another amount? ";         
		cin >> Amount;     
	}     
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients); 
} 




void PrintClientRecordBalanceLine(strClient Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;     
	cout << "| " << setw(40) << left << Client.Name;     
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void ShowTotalBalances() {
	
vector <strClient> vClients = LoadClientsDataFromFile(ClientsFileName);     
cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";     
cout << "\n_______________________________________________________";     
cout << "_________________________________________\n" << endl;     
cout << "| " << left << setw(15) << "Accout Number";     
cout << "| " << left << setw(40) << "Client Name";     
cout << "| " << left << setw(12) << "Balance";     
cout << "\n_______________________________________________________";     
cout << "_________________________________________\n" << endl; 
double TotalBalances = 0; 
if (vClients.size() == 0)         
	cout << "\t\t\t\tNo Clients Available In the System!"; 
else
	for(strClient Client : vClients) { 
		PrintClientRecordBalanceLine(Client);             
	TotalBalances += Client.AccountBalance;             
	cout << endl; 
	}     
cout << "\n_______________________________________________________";     
cout << "_________________________________________\n" << endl;     
cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;
}
void ShowTotalBalanceScreen() {
	ShowTotalBalances();
}




void GoBackToTransactionsMenu() {
	cout << "Press any key to go back to Transactions Menu ...";
	system("pause>0");
	ShowTransactionsMenu();
}
void PerformTransactionsOptions(enTransactionsMenuOptions TransactionsOptions) {
	switch (TransactionsOptions) {
	case enTransactionsMenuOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eWithdraw:
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eTotalBalance:
		system("cls");
		ShowTotalBalanceScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eMainMenuBack:
		ShowMainMenu();
	}
}
void ShowTransactionsMenu() {

	if (!CheckAccessPermission(enMainMenuPermissions::pTranactions)) { 
		ShowAccessDeniedMessage();         
		GoBackToMainMenu(); 
		return; 
	}
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balance.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "===========================================\n";

	PerformTransactionsOptions((enTransactionsMenuOptions)ReadMenuOption("Choose what do you want to do? [1 to 4]? \n", "Invalid number, Choose what do you want to do? [1 to 4]?\n", 1, 4));
}










bool CheckAccessPermission(enMainMenuPermissions Permission) { 
	if (CurrentUser.Permissions == enMainMenuPermissions::eAll) 
		return true; 
	if ((Permission & CurrentUser.Permissions) == Permission) 
		return true; 
	else
		return false; 
}

vector <strUser> LoadUsersDataFromFile(string FileName) {
	vector <strUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		strUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUsers.push_back(User);
		}
		MyFile.close();
	}
	return vUsers;
}
void PrintUserRecordLine(strUser User) { 
	cout << "| " << setw(15) << left << User.UserName;     
	cout << "| " << setw(10) << left << User.Password;     
	cout << "| " << setw(40) << left << User.Permissions; 
}
void ShowAllUsersScreen() { 
	vector <strUser> vUsers = LoadUsersDataFromFile(UsersFileName);     
	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";     
	cout << "\n_______________________________________________________";     
	cout << "_________________________________________\n" << endl;     
	cout << "| " << left << setw(15) << "User Name";     
	cout << "| " << left << setw(10) << "Password";     
	cout << "| " << left << setw(40) << "Permissions";     
	cout << "\n_______________________________________________________";     
	cout << "_________________________________________\n" << endl; 
	if (vUsers.size() == 0)         
		cout << "\t\t\t\tNo Users Available In the System!"; 
	else
		for(strUser User : vUsers) { 
			PrintUserRecordLine(User);             
			cout << endl; 
		}     
	cout << "\n_______________________________________________________";     
	cout << "_________________________________________\n" << endl; 
}
void ShowListUsersScreen() { 
	ShowAllUsersScreen(); 
}




bool UserExistsByUsername(string Username, string FileName) {
	fstream MyFile;     
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())     
	{ 
		string Line; 
		strUser User; 
		while (getline(MyFile, Line))         
		{   
			User = ConvertUserLineToRecord(Line); 
			if (User.UserName == Username)             
			{                 
				MyFile.close(); 
				return true;             
			}         
		}         
		MyFile.close();     
	} 
	return false; 
}
int ReadPermissionsToSet() {
	int Permissions = 0; 
	char Answer = 'n';     
	cout << "\nDo you want to give full access? y/n? ";     
	cin >> Answer; 
	if (Answer == 'y' || Answer == 'Y') { 
		return -1; 
	}
	cout << "\nDo you want to give access to : \n ";     
	cout << "\nShow Client List? y/n? ";     
	cin >> Answer; 
	if (Answer == 'y' || Answer == 'Y') { 
		Permissions += enMainMenuPermissions::pListClients; 
	}     
	cout << "\nAdd New Client? y/n? ";     
	cin >> Answer; 
	if (Answer == 'y' || Answer == 'Y') { 
		Permissions += enMainMenuPermissions::pAddNewClient; 
	}     
	cout << "\nDelete Client? y/n? ";     
	cin >> Answer; 
	if (Answer == 'y' || Answer == 'Y') { 
		Permissions += enMainMenuPermissions::pDeleteClient; 
	}     
	cout << "\nUpdate Client? y/n? ";     
	cin >> Answer; if (Answer == 'y' || Answer == 'Y') { 
		Permissions += enMainMenuPermissions::pUpdateClients; 
	}     
	cout << "\nFind Client? y/n? ";     
	cin >> Answer; if (Answer == 'y' || Answer == 'Y') { 
		Permissions += enMainMenuPermissions::pFindClient; 
	}     
	cout << "\nTransactions? y/n? ";     
	cin >> Answer; 
	if (Answer == 'y' || Answer == 'Y') { 
		Permissions += enMainMenuPermissions::pTranactions; 
	}
	cout << "\nManage Users? y/n? ";     
	cin >> Answer; if (Answer == 'y' || Answer == 'Y') { 
		Permissions += enMainMenuPermissions::pManageUsers; 
	} 
	return Permissions;
}
strUser ReadNewUser() {
	strUser User;     
	cout << "Enter Username? "; 
	// Usage of std::ws will extract allthe whitespace character    
	getline(cin >> ws, User.UserName); 
	while (UserExistsByUsername(User.UserName, UsersFileName))     
	{         
		cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? "; 
		getline(cin >> ws, User.UserName);     
	}     cout << "Enter Password? ";     
	getline(cin, User.Password);     
	User.Permissions = ReadPermissionsToSet(); 
	return User; 
}
void AddNewUser() { 
	strUser User;     
	User = ReadNewUser();     
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User)); 
}
void AddNewUsers() {
	char AddMore = 'Y'; 
	do { 
		//system("cls");        
		cout << "Adding New User:\n\n";         
		AddNewUser();         
		cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";         
		cin >> AddMore;     
	} while (toupper(AddMore) == 'Y'); 
} 
void ShowAddNewUserScreen() { 
	cout << "\n-----------------------------------\n";     
	cout << "\tAdd New User Screen";     
	cout << "\n-----------------------------------\n";     
	AddNewUsers(); 
} 




string ReadUserName() { 
	string Username = "";     
cout << "\nPlease enter Username? ";     
cin >> Username; 
return Username; 
}
bool FindUserByUsername(string Username, vector <strUser> vUsers, strUser& User) { 
	for (strUser U : vUsers) { 
		if (U.UserName == Username) { 
			User = U; 
			return true; 
		} 
	} 
	return false; 
}
void PrintUserCard(strUser User) { 
	cout << "\nThe following are the user details:\n";     
	cout << "-----------------------------------";     
	cout << "\nUsername    : " << User.UserName;     
	cout << "\nPassword    : " << User.Password;     
	cout << "\nPermissions : " << User.Permissions;     
	cout << "\n-----------------------------------\n"; 
}
bool MarkUserForDeleteByUsername(string Username, vector <strUser>& vUsers) { 
	for (strUser& U : vUsers) { 
		if (U.UserName == Username) { 
			U.MarkForDelete = true; 
			return true; 
		} 
	} 
	return false; 
}
vector <strUser> SaveUsersDataToFile(string FileName, vector<strUser> vUsers) {
	fstream MyFile;     
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine; 
	if (MyFile.is_open()) { 
		for (strUser U : vUsers) { 
			if (U.MarkForDelete == false) { 
				//we only write records that are not marked for delete.      
				DataLine = ConvertUserRecordToLine(U);    
				MyFile << DataLine << endl;             
			}         
		}         
		MyFile.close();     
	} 
	return vUsers; 
}
bool DeleteUserByUsername(string Username, vector <strUser>& vUsers) {
	if (Username == "Admin") { 
		cout << "\n\nYou cannot Delete This User."; 
		return false; 
	} 
	strUser User; 
	char Answer = 'n'; 
	if (FindUserByUsername(Username, vUsers, User)) {
		PrintUserCard(User);      
		cout << "\n\nAre you sure you want delete this User? y/n ? ";         
		cin >> Answer; 
		if (Answer == 'y' || Answer == 'Y') {
			MarkUserForDeleteByUsername(Username, vUsers);    
			SaveUsersDataToFile(UsersFileName, vUsers); //Refresh Clients 
			vUsers = LoadUsersDataFromFile(UsersFileName);             
			cout << "\n\nUser Deleted Successfully."; 
			return true;         
		}     
	} 
	else {         
		cout << "\nUser with Username (" << Username << ") is Not Found!"; 
		return false;     
	} 
}
void ShowDeleteUserScreen() { 
	cout << "\n-----------------------------------\n";     
	cout << "\tDelete Users Screen";     
	cout << "\n-----------------------------------\n"; 
	vector <strUser> vUsers = LoadUsersDataFromFile(UsersFileName); 
	string Username = ReadUserName();     
	DeleteUserByUsername(Username, vUsers); 
}




strUser ChangeUserRecord(string Username) { 
	strUser User;     
	User.UserName = Username;     
	cout << "\n\nEnter Password? ";     
	getline(cin >> ws, User.Password);     
	User.Permissions = ReadPermissionsToSet(); 
	return User; 
}
bool UpdateUserByUsername(string Username, vector <strUser>& vUsers) { 
	strUser User; 
	char Answer = 'n'; 
	if (FindUserByUsername(Username, vUsers, User)) { 
		PrintUserCard(User);         
		cout << "\n\nAre you sure you want update this User? y/n ? ";         
		cin >> Answer; 
		if (Answer == 'y' || Answer == 'Y') { 
			for (strUser& U : vUsers) { 
				if (U.UserName == Username) { 
					U = ChangeUserRecord(Username); 
					break; 
				} 
			}    
			SaveUsersDataToFile(UsersFileName, vUsers);             
			cout << "\n\nUser Updated Successfully."; 
			return true; 
		} 
	} 
	else { 
		cout << "\nUser with Account Number (" << Username << ") is Not Found!"; 
		return false; 
	} 
}
void ShowUpdateUserScreen() { 
	cout << "\n-----------------------------------\n";     
	cout << "\tUpdate Users Screen";     
	cout << "\n-----------------------------------\n"; 
	vector <strUser> vUsers = LoadUsersDataFromFile(UsersFileName); 
	string Username = ReadUserName();     
	UpdateUserByUsername(Username, vUsers); 
}




void ShowFindUserScreen() { 
	cout << "\n-----------------------------------\n";     
	cout << "\tFind User Screen";     
	cout << "\n-----------------------------------\n"; 
	vector <strUser> vUsers = LoadUsersDataFromFile(UsersFileName); 
	strUser User; 
	string Username = ReadUserName(); 
	if (FindUserByUsername(Username, vUsers, User))         
		PrintUserCard(User); 
	else        
		cout << "\nUser with Username [" << Username << "] is not found!"; 
}




void GoBackToManageUsersMenu() { 
	cout << "\n\nPress any key to go back to Transactions Menu...";     
	system("pause>0");     
	ShowManageUsersMenu(); 
}




void PerfromManageUsersMenuOption(enManageUsersMenuOptions ManageUsersMenuOption) {
	switch (ManageUsersMenuOption) {
	case enManageUsersMenuOptions::eListUsers:
	{
		system("cls");
		ShowListUsersScreen();
		GoBackToManageUsersMenu();
		break;
	}
	case enManageUsersMenuOptions::eAddNewUser:
	{
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageUsersMenu();
		break;
	}
	case enManageUsersMenuOptions::eDeleteUser:
	{
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersMenu();
		break;
	}
	case enManageUsersMenuOptions::eUpdateUser:
	{
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUsersMenu();
		break;
	}
	case enManageUsersMenuOptions::eFindUser:
	{
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenu();
		break;
	}
	case enManageUsersMenuOptions::eMainMenu:
	{
		ShowMainMenu();
	}
	}
}
void ShowManageUsersMenu() {
	if (!CheckAccessPermission(enMainMenuPermissions::pManageUsers)) { 
		ShowAccessDeniedMessage();         
		GoBackToMainMenu(); 
		return; 
	}     
	system("cls");     
	cout << "===========================================\n";     
	cout << "\t\tManage Users Menu Screen\n";     
	cout << "===========================================\n";     
	cout << "\t[1] List Users.\n";     
	cout << "\t[2] Add New User.\n";     
	cout << "\t[3] Delete User.\n";     
	cout << "\t[4] Update User.\n";     
	cout << "\t[5] Find User.\n";     
	cout << "\t[6] Main Menu.\n";    
	cout << "===========================================\n";

	PerfromManageUsersMenuOption((enManageUsersMenuOptions)ReadMenuOption("Choose what do you want to do? [1 to 6]?\n", "Invalid number, Choose what do you want to do? [1 to 6]?\n", 1, 6));
}










void ShowEndScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";
}




void GoBackToMainMenu() {
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	ShowMainMenu();
}




void PerfromMainMenuOption(enMainMenuOptions MainMenuOption) {
	switch (MainMenuOption) {
	case enMainMenuOptions::eListClients:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eAddNewClient: 
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eTransactions:
		system("cls");
		ShowTransactionsMenu();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eManageUsers:
		system("cls");
		ShowManageUsersMenu();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eLogout:
		system("cls");
		cout << "\n\nLogging out...\n";
		Login();  // Go back to login screen
		break;
	}
}
void ShowMainMenu() {
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";

	PerfromMainMenuOption((enMainMenuOptions)ReadMenuOption("Choose what do you want to do? [1 to 8]? \n", "Invalid number, Choose what do you want to do? [1 to 7]?\n", 1, 8));
}










bool FindUserByUsernameAndPassword(string Username, string Password, strUser& User) { 
	vector <strUser> vUsers = LoadUsersDataFromFile(UsersFileName); 
	for (strUser U : vUsers) { 
		if (U.UserName == Username && U.Password == Password) { 
			User = U; 
			return true; 
		} 
	} 
	return false; 
}
bool  LoadUserInfo(string Username, string Password) {
	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}
void Login() {

	bool LoginFaild = false; 
	string Username, Password; 
	do { 
		system("cls");         
		cout << "\n---------------------------------\n";         
		cout << "\tLogin Screen";         
		cout << "\n---------------------------------\n"; 
		if (LoginFaild) { 
			cout << "Invlaid Username/Password!\n"; 
		}         
		cout << "Enter Username? ";         
		cin >> Username;         
		cout << "Enter Password? ";         
		cin >> Password;         
		LoginFaild = !LoadUserInfo(Username, Password); 
	} while (LoginFaild);     
	ShowMainMenu();
}


int main()
{
	Login();
	return 0;
}