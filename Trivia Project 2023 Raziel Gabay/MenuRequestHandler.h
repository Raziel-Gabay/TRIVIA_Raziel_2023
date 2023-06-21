#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "StatisticsManager.h"


class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user);
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
	//fields
	LoggedUser m_user;
	RoomManager& m_roomManager;
	StatisticsManager& m_statisticsManager;
	RequestHandlerFactory& m_handlerFactory;

	//private methods
	RequestResult signout(RequestInfo info);
	RequestResult getRooms(RequestInfo info);
	RequestResult getPlayersInRoom(RequestInfo info);
	RequestResult getPersonalStats(RequestInfo info);
	RequestResult getHighScore(RequestInfo info);
	RequestResult joinRoom(RequestInfo info);
	RequestResult createRoom(RequestInfo info);

};

