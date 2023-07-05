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
	_file_exist = _access(_dbFileName.c_str(), 0);
	_res = sqlite3_open(_dbFileName.c_str(), &_db);
	if (_res != SQLITE_OK)
	{
		close();
		std::cout << "Failed to open DB" << std::endl;
		return -1;
	}
	if (_file_exist != 0) {
		//Users Table
		_sqlStatement = R"(CREATE TABLE USERS (ID INTEGER PRIMARY
			KEY AUTOINCREMENT NOT NULL,
			Username TEXT NOT NULL, Password TEXT NOT NULL, Mail TEXT);)";
		_errMessage = nullptr;
		_res = sqlite3_exec(_db, _sqlStatement, nullptr, nullptr, _errMessage);
		if (_res != SQLITE_OK)
			return false;

		//QUESTIONS Table
		_sqlStatement = R"(CREATE TABLE QUESTIONS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, QUESTION TEXT NOT NULL,
					CorrectAnswer TEXT NOT NULL, AnswerTwo TEXT NOT NULL,AnswerThree TEXT NOT NULL, AnswerFour TEXT NOT NULL);)";
		_errMessage = nullptr;
		_res = sqlite3_exec(_db, _sqlStatement, nullptr, nullptr, _errMessage);
		if (_res != SQLITE_OK)
			return false;


		//STATISTICS Table
		_sqlStatement = R"(CREATE TABLE STATISTICS (Username TEXT NOT NULL,AverageAnswerTime FLOAT,
					NumOfCorrectAnswers INT, NumOfTotalAnswers INT,NumOfPlayerGames INT, PlayerScore INT);)";
		_errMessage = nullptr;
		_res = sqlite3_exec(_db, _sqlStatement, nullptr, nullptr, _errMessage);
		if (_res != SQLITE_OK)
			return false;

		TenQuestionInstaliize();
	}
}

void SqliteDataBase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
}

int SqliteDataBase::doesUserExist(std::string username)
{
	_sqlStatementStr = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";
	_sqlStatement = _sqlStatementStr.c_str();
	return checks();
}

int SqliteDataBase::doesPasswordMatch(std::string passOne, std::string passTwo)
{
	return (passOne == passTwo);
}

int SqliteDataBase::addNewUser(std::string username, std::string paswword, std::string mail)
{
	if (!mail.empty())
		_sqlStatementStr = "INSERT INTO USERS (Username, Password, Mail) VALUES ('" + username + "', '" + paswword + "', '" + mail + "');";
	else
		_sqlStatementStr = "INSERT INTO USERS (Username, Password) VALUES ('" + username + "', '" + paswword + "');";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, NULL, nullptr, _errMessage);
	if (_res != SQLITE_OK)
		return false;
	else
	{
		_sqlStatementStr = "INSERT INTO STATISTICS(Username) values('" + username + "');";
		_sqlStatement = _sqlStatementStr.c_str();
		_errMessage = nullptr;
		_res = sqlite3_exec(_db, _sqlStatement, NULL, nullptr, _errMessage);
		if (_res != SQLITE_OK)
			return false;
		else return true;
	}
}

std::string SqliteDataBase::getPasswordFromUsername(std::string username)
{
	std::string password;
	_sqlStatementStr = "SELECT Password FROM USERS WHERE USERNAME = '" + username + "';";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, PasswordCallback, &password, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	return password;
}

std::list<Question> SqliteDataBase::getQuestions(int num)
{
	std::list<Question> ql;
	int count;
	_sqlStatementStr = "SELECT COUNT(*) FROM QUESTIONS;";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, CountCallback, &count, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	if (num > count)
	{
		throw "The number of question you ask for is bigger then what we have in this DataBase";
	}
	else
	{
		_sqlStatementStr = "SELECT * FROM QUESTION WHERE ID BETWEEN 0 AND " + std::to_string(num) + ";";
		_sqlStatement = _sqlStatementStr.c_str();
		_errMessage = nullptr;
		_res = sqlite3_exec(_db, _sqlStatement, QuestionCallback, &ql, _errMessage);
		if (_res != SQLITE_OK)
		{
			throw _errMessage;
		}
		return ql;
	}

}

float SqliteDataBase::getPlayerAverageAnswerTime(std::string username)
{
	float averageAnswerTime;
	_sqlStatementStr = "SELECT AverageAnswerTime FROM STATISTICS WHERE Username = '" + username + "';";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, AverageAnswerTimeCallback, &averageAnswerTime, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	return averageAnswerTime;
}

int SqliteDataBase::getNumOfCorrectAnswers(std::string username)
{
	int numOfCorrectAnswers;
	_sqlStatementStr = "SELECT NumOfCorrectAnswers FROM STATISTICS WHERE Username = '" + username + "';";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, CountCallback, &numOfCorrectAnswers, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	return numOfCorrectAnswers;
}

int SqliteDataBase::getNumOfTotalAnswers(std::string username)
{
	int numOfTotalAnswers;
	_sqlStatementStr = "SELECT NumOfTotalAnswers FROM STATISTICS WHERE Username = '" + username + "';";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, CountCallback, &numOfTotalAnswers, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	return numOfTotalAnswers;
}

int SqliteDataBase::getNumOfPlayerGames(std::string username)
{
	int numOfPlayerGames;
	_sqlStatementStr = "SELECT NumOfPlayerGames FROM STATISTICS WHERE Username = '" + username + "';";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, CountCallback, &numOfPlayerGames, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	return numOfPlayerGames;
}

int SqliteDataBase::getPlayerScore(std::string username)
{
	int playerScore;
	_sqlStatementStr = "SELECT PlayerScore FROM STATISTICS WHERE Username = '" + username + "';";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, CountCallback, &playerScore, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	return playerScore;
}

std::vector<std::string> SqliteDataBase::getHighScores()
{
	std::vector<std::string> highscores;
	_sqlStatementStr = "SELECT Username,PlayerScore FROM STATISTICS ORDER BY PlayerScore DESC LIMIT 3;";
	//_sqlStatementStr = "SELECT Username FROM Users Where Username IN (SELECT Username FROM STATISTICS ORDER BY PlayerScore DESC LIMIT 3);";
	_sqlStatement = _sqlStatementStr.c_str();
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, HighscoreCallback, &highscores, _errMessage);
	if (_res != SQLITE_OK)
	{
		throw _errMessage;
	}
	std::cout << _errMessage;
	return highscores;
}

bool SqliteDataBase::checks()
{
	bool doesExsist = false;
	_errMessage = nullptr;
	_res = sqlite3_exec(_db, _sqlStatement, callbackExist, &doesExsist, _errMessage);
	if (!doesExsist ||_res != SQLITE_OK)
		return false;
	else return true;
}

int SqliteDataBase::CountCallback(void* data, int argc, char** argv, char** azColName)
{
	int* count = static_cast<int*>(data);
	if (argv[0] == NULL)
		*count = 0;
	else
	{
		*count = atoi(argv[0]);
	}
	return 0;
}

int SqliteDataBase::QuestionCallback(void* data, int argc, char** argv, char** azColName)
{
	auto ql = static_cast<std::list<Question>*>(data);
	std::string qStr;
	std::vector<std::string> pa;
	for (int i = 0; i < argc; i++) {

		if (std::string(azColName[i]) == "QUESTION") {
			qStr = argv[i];
		}
		if (std::string(azColName[i]) == "CorrectAnswer") {
			pa.push_back(argv[i]);
		}
		if (std::string(azColName[i]) == "AnswerTwo") {
			pa.push_back(argv[i]);
		}
		if (std::string(azColName[i]) == "AnswerThree") {
			pa.push_back(argv[i]);
		}
		if (std::string(azColName[i]) == "AnswerFour") {
			pa.push_back(argv[i]);
		}
	}
	ql->push_back(Question(qStr, pa));
	return 0;
}

int SqliteDataBase::PasswordCallback(void* data, int argc, char** argv, char** azColName)
{
	std::string* password = static_cast<std::string*>(data);
	for (int i = 0; i < argc; i++) {

		if (std::string(azColName[i]) == "Password") {
			*password = argv[i];
		}
	}
	return 0;
}

int SqliteDataBase::AverageAnswerTimeCallback(void* data, int argc, char** argv, char** azColName)
{
	auto answerTime = static_cast<float*>(data);
	for (int i = 0; i < argc; i++) {

		if (std::string(azColName[i]) == "AverageAnswerTime") {
			if (argv[i] == NULL)
				*answerTime = 0;
			else
			{
				*answerTime = std::stof(argv[i]);
			}
		}
	}
	return 0;
}

int SqliteDataBase::HighscoreCallback(void* data, int argc, char** argv, char** azColName)
{
	auto highscoreUsers = static_cast<std::vector<std::string>*>(data);
	for (int i = 0; i < argc; i++)
		{
			if (std::string(azColName[i]) == "Username")
				highscoreUsers->push_back(argv[i]);
			if (std::string(azColName[i]) == "PlayerScore")
			{
				if (argv[i] == NULL)
					highscoreUsers->push_back("0");
				else
				{
					highscoreUsers->push_back(argv[i]);
				}
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

void SqliteDataBase::TenQuestionInstaliize()
{
	std::vector<std::string> questions = {
		"How many cookies does it take to be considered an expert in cookieology?",
		"What is the preferred mode of transportation for a unicorn?",
		"Why did the chicken cross the road?",
		"What's the best way to make a million dollars?",
		"Why don't scientists trust atoms?",
		"What did the ocean say to the beach?",
		"Why did the scarecrow win an award?",
		"How do you catch a squirrel?",
		"Why don't skeletons fight each other?",
		"What's the most reliable way to predict the weather?"
	};

	std::vector<std::string> correctAnswers = {
		"As many as you can eat without getting a stomachache",
		"Rainbow-colored roller skates",
		"To prove it wasn't chicken-hearted",
		"Start with two million dollars",
		"Because they make up everything",
		"Nothing, it just waved",
		"Because it was outstanding in its field",
		"Climb a tree and act like a nut",
		"They don't have the guts",
		"Look outside and see if it's raining cats and dogs"
	};

	std::vector<std::string> incorrectAnswers1 = {
		"42 cookies",
		"A unicorn prefers to fly with its majestic wings.",
		"To get to the other side",
		"Win the lottery",
		"Because they're too small to see",
		"It said, 'I'm really deep'",
		"Because it was stuffed",
		"Set up a squirrel trap",
		"They don't have the stomach for it",
		"Ask a fortune teller"
	};

	std::vector<std::string> incorrectAnswers2 = {
		"The answer is confidential. Only certified cookieologists know.",
		"Unicorns can teleport wherever they want.",
		"To visit its friend on the other side",
		"Invent the next big thing",
		"Because they're always splitting up",
		"It said, 'I'm feeling a bit salty'",
		"Because it was outstanding in multiple fields",
		"Chase it around in circles",
		"They don't have the backbone",
		"Check the nearest rainbow"
	};

	std::vector<std::string> incorrectAnswers3 = {
		"Cookieology is a hoax. There's no such thing as expertise in cookies.",
		"Unicorns ride on magical clouds.",
		"To catch the worm on the other side",
		"Become a famous celebrity",
		"Because they can't be trusted",
		"It said, 'I'm feeling a bit blue'",
		"Because it was outstanding in all categories",
		"Wait for it to come to you",
		"They don't have the courage",
		"Stick your head out the window and see"
	};

	// Iterate over the questions and insert them into the database
	for (int i = 0; i < questions.size(); ++i) {
		std::string q = questions[i];
		std::string ca = correctAnswers[i];
		std::string a2 = incorrectAnswers1[i];
		std::string a3 = incorrectAnswers2[i];
		std::string a4 = incorrectAnswers3[i];

		_sqlStatementStr = "INSERT INTO QUESTIONS (QUESTION, CorrectAnswer, AnswerTwo, AnswerThree, AnswerFour) VALUES (\"" + q + "\", \"" + ca + "\", \"" + a2 + "\", \"" + a3 + "\", \"" + a4 + "\");";

		_sqlStatement = _sqlStatementStr.c_str();
		_errMessage = nullptr;
		_res = sqlite3_exec(_db, _sqlStatement, NULL, nullptr, _errMessage);
		if (_res != SQLITE_OK)
			std::cout << _errMessage << std::endl;
			throw _errMessage;
	}

}

