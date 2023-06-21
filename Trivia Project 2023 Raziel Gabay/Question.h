#pragma once
#include <iostream>
#include <vector>
class Question
{
public:
	Question() = default;
	Question(std::string question, std::vector<std::string>  possibleAnswers);
	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	int getCorrectAnswerId();
private:
	std::string m_question;
	std::vector<std::string>  m_possibleAnswers;
};

