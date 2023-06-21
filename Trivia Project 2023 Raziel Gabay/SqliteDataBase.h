#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <iostream>
#include <Windows.h>
#include <WinINet.h>
#include <vector>
#include <string>
#include "json.hpp"

#pragma comment(lib, "wininet.lib")

using json = nlohmann::json;

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
	virtual std::list<Question> getQuestions(int) override;
	virtual float getPlayerAverageAnswerTime(std::string username) override;
	virtual int getNumOfCorrectAnswers(std::string) override;
	virtual int getNumOfTotalAnswers(std::string) override;
	virtual int getNumOfPlayerGames(std::string username) override;
	virtual int getPlayerScore(std::string username) override;
	virtual std::vector<std::string> getHighScores() override;
	bool checks();
	static int CountCallback(void* data, int argc, char** argv, char** azColName);
	static int QuestionCallback(void* data, int argc, char** argv, char** azColName);
	static int PasswordCallback(void* data, int argc, char** argv, char** azColName);
	static int AverageAnswerTimeCallback(void* data, int argc, char** argv, char** azColName);
	static int HighscoreCallback(void* data, int argc, char** argv, char** azColName);
	static int callbackExist(void* data, int argc, char** argv, char** azColName);
	void TenQuestionInstaliize();


private:
	sqlite3* db;
	std::string dbFileName = DB_NAME;
	int file_exist;
	int res;
	const char* sqlStatement;
	std::string ss; // sqlStatement as String
	char** errMessage;

	//private methods
	void TenQuestionInstaliize();
};

