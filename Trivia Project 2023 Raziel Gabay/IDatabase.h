#pragma once

#include <iostream>
#include "io.h"

#define DB_NAME "triviaDB.sqlite"


class IDatabase
{
public:
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual int doesUserExist(std::string username) = 0;
	virtual int doesPasswordMatch(std::string passOne, std::string passTwo) = 0;
	virtual int addNewUser(std::string username, std::string paswword, std::string mail) = 0;
	virtual std::string getPasswordFromUsername(std::string username) = 0;
};