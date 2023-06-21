#pragma once
#include <iostream>

class LoggedUser
{
public:
	LoggedUser(std::string username) :
		_username(username){}

	std::string getUsername()
	{
		return this->_username;
	}
	bool operator==(const LoggedUser user) const { return this->_username == user._username; }
private:
	std::string _username;
};