#include "StatisticsManager.h"

StatisticsManager::StatisticsManager()
{
    m_database = new SqliteDataBase();
}

std::vector<std::string> StatisticsManager::getHighScore()
{
    return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
    std::vector<std::string> userStats;
    userStats.push_back(std::to_string(this->m_database->getPlayerAverageAnswerTime(username)));
    userStats.push_back(std::to_string(this->m_database->getNumOfCorrectAnswers(username)));
    userStats.push_back(std::to_string(this->m_database->getNumOfTotalAnswers(username)));
    userStats.push_back(std::to_string(this->m_database->getNumOfPlayerGames(username)));
    userStats.push_back(std::to_string(this->m_database->getPlayerScore(username)));

    return userStats;
}
