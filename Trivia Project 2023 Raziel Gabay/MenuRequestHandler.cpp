#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user)
	:m_handlerFactory(handlerFactory), m_roomManager(handlerFactory.getRoomManager()), m_statisticsManager(handlerFactory.getStatisticsManager()), m_user(user)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.id == CREATE_ROOM_CODE || requestInfo.id == GET_ROOMS_CODE ||
		requestInfo.id == GET_PLAYERS_IN_ROOM_CODE || requestInfo.id == JOIN_ROOM_CODE ||
		requestInfo.id == GET_HIGHSCORE_CODE || requestInfo.id == GET_PERSONAL_STATS_CODE || 
		requestInfo.id == LOGOUT_CODE)
	{
		return true;
	}
	return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (isRequestRelevant(requestInfo))
	{
		if (requestInfo.id == CREATE_ROOM_CODE)
		{
			return createRoom(requestInfo);
		}
		if (requestInfo.id == GET_ROOMS_CODE)
		{
			return getRooms(requestInfo);
		}
		if (requestInfo.id == GET_PLAYERS_IN_ROOM_CODE)
		{
			return getPlayersInRoom(requestInfo);
		}
		if (requestInfo.id == JOIN_ROOM_CODE)
		{
			return joinRoom(requestInfo);
		}
		if (requestInfo.id == GET_HIGHSCORE_CODE)
		{
			return getHighScore(requestInfo);
		}
		if (requestInfo.id == GET_PERSONAL_STATS_CODE)
		{
			return getPersonalStats(requestInfo);
		}
		if (requestInfo.id == LOGOUT_CODE)
		{
			return signout(requestInfo);
		}
	}
	else
	{
		RequestResult result;
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user.getUsername());
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}
}

RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	RequestResult result;
	this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername());
	result.newHandler = this->m_handlerFactory.createLoginRequestHandler();

	LogoutResponse logoutResponse;
	result.response = JsonResponsePacketSerializer::serializeResponse(logoutResponse);

	return result;
}

RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this;


	GetRoomsResponse getRoomsResponse;
	getRoomsResponse.rooms = this->m_roomManager.getRooms();
	result.response = JsonResponsePacketSerializer::serializeResponse(getRoomsResponse);

	return result;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this;

	GetPlayersInRoomRequest req = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);

	GetPlayersInRoomResponse getPlayerInRoomResponse = { this->m_roomManager.getRoom(req.roomid).getAllUsers() };
	result.response = JsonResponsePacketSerializer::serializeResponse(getPlayerInRoomResponse);

	return result;
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this;

	GetPersonalStatsResponse getPersonalStatsResponse;
	getPersonalStatsResponse.statistics =  this->m_statisticsManager.getUserStatistics(this->m_user.getUsername());
	result.response = JsonResponsePacketSerializer::serializeResponse(getPersonalStatsResponse);

	return result;
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this;

	GetHighScoreResponse getHighScoreResponse;
	getHighScoreResponse.statistics = this->m_statisticsManager.getHighScore();
	result.response = JsonResponsePacketSerializer::serializeResponse(getHighScoreResponse);

	return result;
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this;

	JoinRoomRequest req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);

	this->m_roomManager.getRoom(req.roomid).addUser(this->m_user);

	JoinRoomResponse joinRoomResponse;
	result.response = JsonResponsePacketSerializer::serializeResponse(joinRoomResponse);

	return result;
}

RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this;

	CreateRoomRequest req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);

	RoomData rd;
	if (this->m_roomManager.getRooms().empty())
	{
		rd.id = 1;
	}
	else
	rd.id = this->m_roomManager.getRooms().begin()->id + 1;
	rd.name = req.roomName;
	rd.isActive = true; 
	rd.maxPlayers = req.maxUsers;
	rd.numOfQuestionsInGame = req.questionCount;
	rd.timePerQuestion = req.answerTimeout;
	this->m_roomManager.createRoom(this->m_user, rd);

	CreateRoomResponse createRoomResponse;
	result.response = JsonResponsePacketSerializer::serializeResponse(createRoomResponse);

	return result;
}
