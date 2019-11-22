//Author: Tran Nguyen

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <my_global.h>
#include <mysql.h>

#include <conio.h>

#include <iomanip>

using namespace std;

int addCar(string carVIN, int carMiles, string dealerName, int carPrice, MYSQL*, MYSQL);
int addMan(string manCode, string manName, MYSQL*, MYSQL);
int addDealer(string dealerName, int zipCode, string dealerNum, MYSQL*, MYSQL);
int list(char cmd2, MYSQL*, MYSQL);



string carTable = "car_t";
string manTable = "man_t";
string dealerTable = "dealer_t";

//This function reads in a password without echoing it to the screen
string myget_passwd() {
	string passwd;

	for (;;) {
		char ch;
		ch = _getch();           // get char WITHIOUT echo!
		if (ch == 13 || ch == 10) // if done ...
			break;           //  stop reading chars!
		cout << '*';  // dump * to screen
		passwd += ch;   // addd char to password
	}
	cin.sync(); // flush anything else in the buffer (remaining newline)
	cout << endl;  // simulate the enter that the user pressed

	return passwd;
}

int main() {
	//-------------- START SQL CONNECTION-------------------
	// mysql connection and query variables
	MYSQL* conn, // actual mysql connection
		mysql;   // local mysql data
	MYSQL_RES* res; // mysql query results
	MYSQL_ROW row;  // one row of a table (result)

	// strings for mysql hostname, userid, ...
	string db_host;
	string db_user;
	string db_password;
	string db_name;

	// set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; cout.flush();
	mysql_init(&mysql);
	cout << "Done!" << endl;

	// get user credentials and mysql server info
	cout << "Enter MySQL database hostname (or IP adress):";
	cin >> db_host; //localhost

	cout << "Enter MySQL database username:";
	cin >> db_user; //root

	cout << "Enter MySQL database password:";
	db_password = myget_passwd();
	db_name = db_user;

	// go out and connect to the mysql server
	cout << "Connecting to remote DB ..."; cout.flush();
	conn = mysql_real_connect(&mysql,
		db_host.c_str(), db_user.c_str(), db_password.c_str(), db_user.c_str(),
		0, 0, 0); // last three are usually 0's

// if we couldn't setup the connection ...
	if (conn == NULL)
	{
		// print out the error message as to why ...
		cout << mysql_error(&mysql) << endl;
		return 1; // ... and exit the program. 
	}
	else
		cout << "DB connection established" << endl;

	int status;

	//-------CREATE CAR TABLE------------------------
	string myQuery = "create table if not exists ";
	myQuery += carTable;
	myQuery += " (carVIN char(17) NOT NULL, carMiles int NOT NULL, dealerName char(100) NOT NULL, carPrice int NOT NULL, ";
	myQuery += "PRIMARY KEY(carVIN), ";
	myQuery += "FOREIGN KEY(dealerName) REFERENCES ";
	myQuery += dealerTable;
	myQuery += " (dealerName) ";
	myQuery += ");";
	status = mysql_query(conn, myQuery.c_str());

	// If the query didn't work
	if (status != 0) {
		//Explain why
		cout << mysql_error(&mysql) << endl;
	}

	//------CREATE MANUFACTURER TABLE-------------------------
	myQuery = "create table if not exists ";
	myQuery += manTable;
	myQuery += " (manCode char(3) NOT NULL, manName char(100) NOT NULL, ";
	myQuery += "PRIMARY KEY(manCode)); ";
	status = mysql_query(conn, myQuery.c_str());

	// If the query didn't work
	if (status != 0) {
		//Explain why
		cout << mysql_error(&mysql) << endl;
	}

	//--------CREATE DEALER TABLE----------------------------
	myQuery = "create table if not exists ";
	myQuery += dealerTable;
	myQuery += " (dealerName char(100) NOT NULL, zipCode int, dealerNum char(10) NOT NULL, ";
	myQuery += "PRIMARY KEY(dealerName)); ";
	status = mysql_query(conn, myQuery.c_str());

	// If the query didn't work
	if (status != 0) {
		//Explain why
		cout << mysql_error(&mysql) << endl;
	}
	//---------------------------------------------------------------------

	bool flag = false; //boolean to end the program

	char cmd;
	char cmd2;

	//While it's still running
	while (!flag) {
		//Variables
		string carVIN;
		int carMiles;
		string dealerName;
		int carPrice;
		string manCode;
		string manName;
		int zipCode;
		string dealerNum;


		cout << ">>";
		cin >> cmd; //reads in first command

		switch (cmd) {
		case 'a':
			cin >> cmd2; //reads in second command
			cin.get(); //skips space

			switch (cmd2) {
				//add a car
			case 'c':
				cin >> carVIN;
				cin.get();
				cin >> carMiles;
				cin.get();
				cin >> dealerName;
				cin.get();
				cin >> carPrice;

				status = addCar(carVIN, carMiles, dealerName, carPrice, conn, mysql);

				// If the query didn't work
				if (status != 0) {
					//Explain why
					cout << mysql_error(&mysql) << endl;
				}
				break;

				//add a manufacturer
			case 'm':
				cin >> manCode;
				cin.get();
				cin >> manName;

				status = addMan(manCode, manName, conn, mysql);
				if (status != 0) {
					//Explain why
					cout << mysql_error(&mysql) << endl;
				}
				break;

				//add a dealer
			case 'd':				
				cin >> dealerName;
				cin.get();
				cin >> zipCode;
				cin.get();
				cin >> dealerNum;

				status = addDealer(dealerName, zipCode, dealerNum, conn, mysql);

				if (status != 0) {
					//Explain why
					cout << mysql_error(&mysql) << endl;
				}
				break;
			}
		case 'l':
			cin >> cmd2; //reads in second command
			status = list(cmd2, conn, mysql);

			if (status != 0) {
				//Explain why
				cout << mysql_error(&mysql) << endl;
			}
			break;

		//case 'f':
		//case 'd':
		//case 's':
		//case 'q':

		}



	}
	return 0;
}

int addCar(string carVIN, int carMiles, string dealerName, int carPrice, MYSQL* conn, MYSQL mysql) {
	int status;

	string myQuery = "insert into ";
	myQuery += carTable;
	myQuery += " VALUES('";
	myQuery += carVIN;
	myQuery += "',  '";
	myQuery += carMiles;
	myQuery += "',  '";
	myQuery += dealerName;
	myQuery += "',  '";
	myQuery += carPrice;
	myQuery += "');";

	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int addMan(string manCode, string manName, MYSQL* conn, MYSQL mysql) {
	int status;

	string myQuery = "insert into ";
	myQuery += manTable;
	myQuery += " VALUES('";
	myQuery += manCode;
	myQuery += "',  '";
	myQuery += manName;
	myQuery += "');";

	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int addDealer(string dealerName, int zipCode, string dealerNum, MYSQL* conn, MYSQL mysql) {
	int status;
	cout << dealerName << "/" << zipCode << "/" << dealerNum;
	string myQuery = "insert into ";
	myQuery += dealerTable;
	myQuery += " VALUES('";
	myQuery += dealerName;
	myQuery += "', '";
	string zip = to_string(zipCode); 
	myQuery += zip;
	myQuery += "', '";
	myQuery += dealerNum;
	myQuery += "');";
	
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int list(char cmd2, MYSQL* conn, MYSQL mysql) {
	int status;

	string myQuery;

	MYSQL_RES* result;
	MYSQL_ROW row;

	switch (cmd2) {
		//list cars
	case 'c':
		myQuery = "select * from " + carTable + " ;";
		break;

		//list dealers
	case 'd':
		myQuery = "select * from " + dealerTable + " ;";
		break;

	default:
		cout << "Table not exists" << endl;
		break;
	}
	status = mysql_query(conn, myQuery.c_str());

	if (status != 0) {
		//Explain why
		cout << mysql_error(&mysql) << endl;
	}

	result = mysql_store_result(conn);

	//Print out the info received
	switch (cmd2) {
	case'c':
		for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result)) {
			cout << setw(20) << row[0] << setw(0) << row[1] << endl;
		}
		break;
	case 'd':
		for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result)) {
			cout << row[0] << setw(10) << row[1] << setw(20) << row[2] << endl;

		}

	}

	mysql_free_result(result);
	return status;
}
