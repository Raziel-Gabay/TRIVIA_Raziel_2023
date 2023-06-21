#include "Question.h"

Question::Question(std::string question, std::vector<std::string> possibleAnswers)
    :m_question(question), m_possibleAnswers(possibleAnswers)
{
}

std::string Question::getQuestion()
{
    return this->m_question;
}

std::vector<std::string> Question::getPossibleAnswers()
{
    return this->m_possibleAnswers;
}
