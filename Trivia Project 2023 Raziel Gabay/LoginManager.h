#pragma once

#include "SqliteDataBase.h"
#include "LoggedUser.h"
#include <vector>

class LoginManager
{
public:
	LoginManager();
	void signup(std::string username, std::string password, std::string mail);
	void login(std::string username, std::string password);
	void logout(std::string username);
	bool isUserLogged(std::string username);
	bool doesUserExist(std::string username);

private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUser;
};