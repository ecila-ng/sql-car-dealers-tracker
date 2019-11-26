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
int find(char cmd2, MYSQL*, MYSQL);
int deleteWhat(char cmd2, MYSQL*, MYSQL);
int summarize(MYSQL*, MYSQL);


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

//This function is the main function 
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
	cout << "----- CAR DEALERS MANAGER-----" << endl;

	//-------CREATE CAR TABLE------------------------
	string myQuery = "create table if not exists ";
	myQuery += carTable;
	myQuery += " (carVIN char(17) NOT NULL, carMiles int NOT NULL, dealerName char(100) NOT NULL, carPrice int NOT NULL, ";
	myQuery += "PRIMARY KEY(carVIN), ";
	myQuery += "CONSTRAINT FK_dealerName ";
	myQuery += "FOREIGN KEY(dealerName) REFERENCES ";
	myQuery += dealerTable;
	myQuery += " (dealerName) ON DELETE CASCADE ON UPDATE CASCADE); ";
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
					cout << "Error: dealer name / manufacturer name not exists. " << endl;
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
			break;
		case 'l':
			cin >> cmd2; //reads in second command
			status = list(cmd2, conn, mysql);

			if (status != 0) {
				//Explain why
				cout << mysql_error(&mysql) << endl;
			}
			break;

		case 'f':
			cin >> cmd2; //reads in second command
			status = find(cmd2, conn, mysql);

			if (status != 0) {
				//Explain why
				cout << mysql_error(&mysql) << endl;
			}
			break;

		case 'd':
			cin >> cmd2; //reads in second command
			status = deleteWhat(cmd2, conn, mysql);

			if (status != 0) {
				//Explain why
				cout << mysql_error(&mysql) << endl;
			}
			break;

		case 's':
			status = summarize(conn, mysql);

			if (status != 0) {
				//Explain why
				cout << mysql_error(&mysql) << endl;
			}
			break;

		case 'q':
			flag = true;
			cout << "Exiting --------------------------------------------------";
			break;
			// if there is an error
		default:
			cout << "Wrong command";
			cout << endl;
			break;
		}
	}
	return 0;
}

//This function adds a car into the system
int addCar(string carVIN, int carMiles, string dealerName, int carPrice, MYSQL* conn, MYSQL mysql) {
	int status;

	string myQuery = "insert into ";
	myQuery += carTable;
	myQuery += " VALUES('";
	myQuery += carVIN;
	myQuery += "',  '";
	string miles = to_string(carMiles);
	myQuery += miles;
	myQuery += "',  '";
	myQuery += dealerName;
	myQuery += "',  '";
	string price = to_string(carPrice);
	myQuery += price;
	myQuery += "');";
	status = mysql_query(conn, myQuery.c_str());

	return status;
}

//This function adds a manufacturer into the system
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

//This function adds a dealer into the system
int addDealer(string dealerName, int zipCode, string dealerNum, MYSQL* conn, MYSQL mysql) {
	int status;
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
//This function lists all of the car or all of the dealers.
int list(char cmd2, MYSQL* conn, MYSQL mysql) {
	int status;

	string myQuery;

	MYSQL_RES* result;
	MYSQL_ROW row;

	switch (cmd2) {
		//list cars
	case 'c':
		myQuery = "select * from " + carTable;
		myQuery += " order by carVIN;";
		break;

		//list dealers
	case 'd':
		myQuery = "select * from " + dealerTable;
		myQuery += " order by zipCode, dealerName;";
		break;
	case 'm':
		myQuery = "select * from " + manTable + ";";
		break;

	default:
		cout << "Wrong command" << endl;
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
			cout << setw(20) << left << row[0] << setw(15) << left << row[1] << setw(15) << left << row[2] << setw(20) << row[3] << endl;
		}
		break;
	case 'd':
		for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result)) {
			//string temp = row[2];
			//string num = "(" + temp.substr(0, 3) + ")" + temp.substr(3, 3) + "-" + temp.substr(6, 4);
			cout << setw(20) << left << row[0] << setw(15) << left << row[1] << setw(15) << row[2] << endl;
		}
		break;

	case 'm': //LIST ALL MANUFACTURERS - FOR DEVELOPMENT PURPOSES
		for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result)) {
			cout << setw(20) << left << row[0] << setw(15) << left << row[1] << endl;
		}
		break;
	}

	mysql_free_result(result);
	return status;
}

//This function finds either all cars from a specific manufacturer or a specific zipcode.
//All cars listed include dealer name, phone number and all the info about the car. 
int find(char cmd2, MYSQL* conn, MYSQL mysql) {
	int status;
	string what;
	cin >> what;

	string myQuery;

	MYSQL_RES* result;
	MYSQL_ROW row;

	switch (cmd2) {
	case 'm':
		myQuery = "SELECT car_t.carMiles, car_t.carPrice, car_t.dealerName,";
		myQuery += " dealer_t.dealerNum FROM car_t, dealer_t ";
		myQuery += "WHERE LEFT(carVIN, 3) = (SELECT manCode FROM man_t WHERE manName =\"";
		myQuery += what;
		myQuery += "\") AND car_t.dealerName = dealer_t.dealerName";
		myQuery += " ORDER BY carPrice DESC, carMiles ASC, dealer_t.dealerName;";
		break;
	case 'z':
		myQuery = "SELECT man_t.manName, car_t.carMiles, car_t.carPrice, car_t.dealerName, ";
		myQuery += "dealer_t.dealerNum FROM dealer_t, car_t, man_t ";
		myQuery += "WHERE dealer_t.dealerName = car_t.dealerName ";
		myQuery += "AND man_t.manName = (SELECT man_t.manName FROM man_t WHERE LEFT(car_t.carVIN,3) = man_t.manCode) ";
		myQuery += "AND dealer_t.dealerName = (SELECT dealerName FROM dealer_t WHERE zipCode =";
		myQuery += what;
		myQuery += ") ";
		myQuery += "ORDER BY man_t.manName ASC, car_t.carPrice DESC, dealer_t.dealerName;";
		break;
	default:
		cout << "Wrong command" << endl;
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
	case'm':
		for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result)) {
			string temp = row[3];
			string num = "(" + temp.substr(0, 3) + ")" + temp.substr(3, 3) + "-" + temp.substr(6, 4);

			cout << what << ": ";
			cout << row[0] << " miles, ";
			cout << "$" << row[1];
			cout << ": " << row[2] << "[";
			cout << num << "]" << endl;
		}
		break;
	case 'z':
		for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result)) {
			string temp = row[4];
			string num = "(" + temp.substr(0, 3) + ")" + temp.substr(3, 3) + "-" + temp.substr(6, 4);

			cout << row[0] << ": ";
			cout << row[1] << " miles, ";
			cout << "$" << row[2];
			cout << ": " << row[3];
			cout << "[" << num << "]" << endl;
		}
		break;
	}
	mysql_free_result(result);
	return status;
}
//This function remove either a car (from a VIN) or a dealer with all the cars they're selling------------------
int deleteWhat(char cmd2, MYSQL* conn, MYSQL mysql) {
	int status;
	string what;
	cin >> what;

	string myQuery;

	MYSQL_RES* result;
	MYSQL_ROW row;

	switch (cmd2) {
	case'c':
		myQuery = "DELETE FROM car_t";
		myQuery += " WHERE carVIN = \"";
		myQuery += what;
		myQuery += "\";";
		break;
	case 'd':
		myQuery = "DELETE car_t, dealer_t FROM car_t INNER JOIN dealer_t ";
		myQuery += "ON car_t.dealerName = dealer_t.dealerName ";
		myQuery += "WHERE car_t.dealerName = \"";
		myQuery += what;
		myQuery += "\";";
		break;
	case 'a': //DELETE ALL ROWS - FOR DEVELOPMENT PURPOSE
		myQuery = "DELETE FROM dealer_t";
		break;
	default:
		cout << "Wrong command" << endl;
		break;
	}
	status = mysql_query(conn, myQuery.c_str());

	if (status != 0) {
		//Explain why
		cout << mysql_error(&mysql) << endl;
	}

	return status;
}

//This function simply print out one line for each manufacturer and -----------------------------
//includes the average price of the cars made by that manufacturer.

int summarize(MYSQL* conn, MYSQL mysql) {
	int status;
	string myQuery;

	MYSQL_RES* result;
	MYSQL_ROW row;

	myQuery = "SELECT man_t.manName, AVG(car_t.carPrice) ";
	myQuery += "FROM man_t, car_t ";
	myQuery += "WHERE LEFT(carVIN, 3) = man_t.manCode ";
	myQuery += "GROUP BY man_t.manName;";

	status = mysql_query(conn, myQuery.c_str());

	if (status != 0) {
		//Explain why
		cout << mysql_error(&mysql) << endl;
	}

	result = mysql_store_result(conn);


	for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result)) {
		cout << setw(20) << left << row[0] << setw(15) << left << row[1] << endl;;
	}

	mysql_free_result(result);
	return status;
}