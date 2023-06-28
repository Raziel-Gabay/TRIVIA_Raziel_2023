#pragma once
#include "LoginRequestHandler.h"
#include "IDatabase.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "MenuRequestHandler.h"
#include "StatisticsManager.h"

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory();
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();

	MenuRequestHandler* createMenuRequestHandler(LoggedUser);
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
	IDatabase*& getDataBase();
private:
	LoginManager m_loginManager;
	IDatabase* m_database;
	RoomManager m_roommManager;
	StatisticsManager m_statisticsManager;
};

