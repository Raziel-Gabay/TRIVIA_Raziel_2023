#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory()
	:m_loginManager(LoginManager()), m_roommManager(RoomManager()), m_statisticsManager(StatisticsManager())
{
	m_database = new SqliteDataBase;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	LoginRequestHandler* ptr = new LoginRequestHandler(*this);
	return ptr;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
	MenuRequestHandler* ptr = new MenuRequestHandler(*this, user);
	return ptr;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return this->m_statisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return this->m_roommManager;
}

IDatabase*& RequestHandlerFactory::getDataBase()
{
	return this->m_database;
}
