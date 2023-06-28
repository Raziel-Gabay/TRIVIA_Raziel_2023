#pragma once

#include "IDatabase.h"
#include "SqliteDataBase.h"
#include <vector>
class StatisticsManager
{
public:
	StatisticsManager();
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string username);
private:
	IDatabase* m_database;
};

