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
	file_exist = _access(dbFileName.c_str(), 0);
	res = sqlite3_open(dbFileName.c_str(), &db);
	if (res != SQLITE_OK)
	{
		close();
		std::cout << "Failed to open DB" << std::endl;
		return -1;
	}
	if (file_exist != 0) {
		//Users Table
		sqlStatement = R"(CREATE TABLE USERS (ID INTEGER PRIMARY
			KEY AUTOINCREMENT NOT NULL,
			Username TEXT NOT NULL, Password TEXT NOT NULL, Mail TEXT);)";
		if (!checks())
		{
			return false;
		}
		//QUESTIONS Table
		sqlStatement = R"(CREATE TABLE QUESTIONS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, QUESTION TEXT NOT NULL,
					CorrectAnswer TEXT NOT NULL, AnswerTwo TEXT NOT NULL,AnswerThree TEXT NOT NULL, AnswerFour TEXT NOT NULL);)";
		if (!checks())
		{
			return false;
		}
		TenQuestionInstaliize();

		//STATISTICS Table
		sqlStatement = R"(CREATE TABLE STATISTICS (Username TEXT NOT NULL,AverageAnswerTime FLOAT,
					NumOfCorrectAnswers INT, NumOfTotalAnswers INT,NumOfPlayerGames INT, PlayerScore INT);)";
		if (!checks())
		{
			return false;
		}

	}
}

void SqliteDataBase::close()
{
	sqlite3_close(db);
	db = nullptr;
}

int SqliteDataBase::doesUserExist(std::string username)
{
	ss = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";
	sqlStatement = ss.c_str();
	return checks();
}

int SqliteDataBase::doesPasswordMatch(std::string passOne, std::string passTwo)
{
	return (passOne == passTwo);
}

int SqliteDataBase::addNewUser(std::string username, std::string paswword, std::string mail)
{
	ss = "INSERT INTO USERS (Username, Password, Mail) VALUES ('" + username + "', '" + paswword + "', '" + mail + "');";
	sqlStatement = ss.c_str();
	return checks();
}

std::string SqliteDataBase::getPasswordFromUsername(std::string username)
{
	std::string password;
	password.clear();
	ss = "SELECT Password FROM USERS WHERE USERNAME = '" + username + "';";
	sqlStatement = ss.c_str();
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, PasswordCallback, &password, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return password;
}

std::list<Question> SqliteDataBase::getQuestions(int num)
{
	std::list<Question> ql;
	int count;
	ss = "SELECT COUNT(*) FROM QUESTIONS;";
	sqlStatement = ss.c_str();
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, CountCallback, &count, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	if (num > count)
	{
		throw "The number of question you ask for is bigger then what we have in this DataBase";
	}
	else
	{
		ss = "SELECT * FROM QUESTION WHERE ID BETWEEN 0 AND " + std::to_string(num) + ";";
		sqlStatement = ss.c_str();
		errMessage = nullptr;
		res = sqlite3_exec(db, sqlStatement, QuestionCallback, &ql, errMessage);
		if (res != SQLITE_OK)
		{
			throw errMessage;
		}
		return ql;
	}

}

float SqliteDataBase::getPlayerAverageAnswerTime(std::string username)
{
	float averageAnswerTime;
	ss = "SELECT AverageAnswerTime FROM STATISTICS WHERE Username = '" + username + "';";
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, AverageAnswerTimeCallback, &averageAnswerTime, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return averageAnswerTime;
}

int SqliteDataBase::getNumOfCorrectAnswers(std::string username)
{
	int numOfCorrectAnswers;
	ss = "SELECT NumOfCorrectAnswers FROM STATISTICS WHERE Username = '" + username + "';";
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, CountCallback, &numOfCorrectAnswers, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return numOfCorrectAnswers;
}

int SqliteDataBase::getNumOfTotalAnswers(std::string username)
{
	int numOfTotalAnswers;
	ss = "SELECT NumOfTotalAnswers FROM STATISTICS WHERE Username = '" + username + "';";
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, CountCallback, &numOfTotalAnswers, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return numOfTotalAnswers;
}

int SqliteDataBase::getNumOfPlayerGames(std::string username)
{
	int numOfPlayerGames;
	ss = "SELECT NumOfPlayerGames FROM STATISTICS WHERE Username = '" + username + "';";
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, CountCallback, &numOfPlayerGames, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return numOfPlayerGames;
}

int SqliteDataBase::getPlayerScore(std::string username)
{
	int playerScore;
	ss = "SELECT PlayerScore FROM STATISTICS WHERE Username = '" + username + "';";
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, CountCallback, &playerScore, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return playerScore;
}

std::vector<std::string> SqliteDataBase::getHighScores()
{
	std::vector<std::string> highscores;
	ss = "SELECT Username FROM STATISTICS ORDER BY PlayerScore DESC LIMIT 5;";
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, HighscoreCallback, &highscores, errMessage);
	if (res != SQLITE_OK)
	{
		throw errMessage;
	}
	return highscores;
}

bool SqliteDataBase::checks()
{
	bool doesExsist = false;
	errMessage = nullptr;
	res = sqlite3_exec(db, sqlStatement, callbackExist, &doesExsist, errMessage);
	if (!doesExsist ||res != SQLITE_OK)
		return false;
	else return true;
}

int SqliteDataBase::CountCallback(void* data, int argc, char** argv, char** azColName)
{
	int* count = static_cast<int*>(data);
	*count = atoi(argv[0]);
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
	auto password = static_cast<std::string*>(data);
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
			*answerTime = atof(argv[i]);
		}
	}
	return 0;
}

int SqliteDataBase::HighscoreCallback(void* data, int argc, char** argv, char** azColName)
{
	auto highscoreUsers = static_cast<std::vector<std::string>*>(data);
	highscoreUsers->push_back(argv[0]);
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

		std::string ss = "INSERT INTO QUESTIONS (QUESTION, CorrectAnswer, AnswerTwo, AnswerThree, AnswerFour) VALUES ('" + q + "', '" + ca + "', '" + a2 + "', '" + a3 + "', '" + a4 + "');";

		sqlStatement = ss.c_str();
		checks();
	}

}

