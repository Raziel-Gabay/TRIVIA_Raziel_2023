#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "io.h"
#include <list>
#include "Question.h"

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
	virtual std::list<Question> getQuestions(int) = 0;
	virtual float getPlayerAverageAnswerTime(std::string username) = 0;
	virtual int getNumOfCorrectAnswers(std::string) = 0;
	virtual int getNumOfTotalAnswers(std::string) = 0;
	virtual int getNumOfPlayerGames(std::string username) = 0;
	virtual int getPlayerScore(std::string username) = 0;
	virtual std::vector<std::string> getHighScores() = 0;
};