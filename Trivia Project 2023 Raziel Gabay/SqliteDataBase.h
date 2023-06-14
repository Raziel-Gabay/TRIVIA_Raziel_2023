#pragma once
#include "IDatabase.h"
#include "sqlite3.h"

class SqliteDataBase : public IDatabase
{
public:
	SqliteDataBase();
	~SqliteDataBase();

	virtual bool open()  override;
	virtual void close() override;
	virtual int doesUserExist(std::string username) override;
	virtual int doesPasswordMatch(std::string passOne, std::string passTwo) override;
	virtual int addNewUser(std::string username, std::string paswword, std::string mail) override;
	virtual std::string getPasswordFromUsername(std::string username) override;
	bool checks();
	static int PasswordCallback(void* data, int argc, char** argv, char** azColName);
	static int callbackExist(void* data, int argc, char** argv, char** azColName);


private:
	sqlite3* db;
	std::string dbFileName = DB_NAME;
	int file_exist;
	int res;
	const char* sqlStatement;
	std::string ss; // sqlStatement as String
	char** errMessage;
};

