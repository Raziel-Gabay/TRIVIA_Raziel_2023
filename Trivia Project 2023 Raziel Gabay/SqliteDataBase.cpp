#include "SqliteDataBase.h"

SqliteDataBase::SqliteDataBase()
{
	SqliteDataBase::open();
}

SqliteDataBase::~SqliteDataBase()
{
	SqliteDataBase::close();
}

bool SqliteDataBase::open()
{
	file_exist = _access(dbFileName.c_str(), 0);
	res = sqlite3_open(dbFileName.c_str(), &db);
	if (res != SQLITE_OK)
	{
		close();
		std::cout << "Failed to open DB" << std::endl;
		return -1;
	}
	if (file_exist != 0) {
		//Users Table
		sqlStatement = R"(CREATE TABLE USERS (ID INTEGER PRIMARY
			KEY AUTOINCREMENT NOT NULL,
			Username TEXT NOT NULL, Password TEXT NOT NULL, Mail TEXT);)";
		if (!checks())
		{
			return false;
		}
	}
}

void SqliteDataBase::close()
{
	sqlite3_close(db);
	db = nullptr;
}

int SqliteDataBase::doesUserExist(std::string username)
{
	ss = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";
	sqlStatement = ss.c_str();
	return checks();
}

int SqliteDataBase::doesPasswordMatch(std::string passOne, std::string passTwo)
{
	return (passOne == passTwo);
}

int SqliteDataBase::addNewUser(std::string username, std::string paswword, std::string mail)
{
	ss = "INSERT INTO USERS (Username, Password, Mail) VALUES ('" + username + "', '" + paswword + "', '" + mail + "');";
	sqlStatement = ss.c_str();
	return checks();
}

std::string SqliteDataBase::getPasswordFromUsername(std::string username)
{
	std::string password;
	password.clear();
	ss = "SELECT Password FROM USERS WHERE USERNAME = '" + username + "';";
	sqlStatement = ss.c_str();
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, PasswordCallback, &password, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return password;
}

bool SqliteDataBase::checks()
{
	bool doesExsist = false;
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, callbackExist, &doesExsist, errMessage);
	if (!doesExsist ||res != SQLITE_OK)
		return false;
	else return true;
}

int SqliteDataBase::PasswordCallback(void* data, int argc, char** argv, char** azColName)
{
	auto password = static_cast<std::string*>(data);
	for (int i = 0; i < argc; i++) {

		if (std::string(azColName[i]) == "Password") {
			*password = argv[i];
		}
	}
	return 0;
}

int SqliteDataBase::callbackExist(void* data, int argc, char** argv, char** azColName)
{
	bool* isExist = (bool*)data;
	*isExist = true;
	return 0;
}

