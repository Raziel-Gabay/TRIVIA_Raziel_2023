#include "LoginManager.h"

LoginManager::LoginManager()
{

	m_database = new SqliteDataBase;
}

void LoginManager::signup(std::string username, std::string password, std::string mail)
{
		this->m_database->addNewUser(username, password, mail);
}

void LoginManager::login(std::string username, std::string password)
{
	if (this->m_database->doesUserExist(username))
	{
		if (this->m_database->doesPasswordMatch(password, this->m_database->getPasswordFromUsername(username)))
		{
			this->m_loggedUser.push_back(LoggedUser(username));
		}
	}
}

void LoginManager::logout(std::string username)
{
	this->m_loggedUser.erase(std::remove(this->m_loggedUser.begin(), this->m_loggedUser.end(), LoggedUser(username)), this->m_loggedUser.end());
}

bool LoginManager::isUserLogged(std::string username)
{
	return std::find(this->m_loggedUser.begin(),this->m_loggedUser.end(), LoggedUser(username)) != this->m_loggedUser.end();
}

bool LoginManager::doesUserExist(std::string username)
{
 	return this->m_database->doesUserExist(username);
}

