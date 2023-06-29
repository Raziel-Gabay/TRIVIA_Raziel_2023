#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user, Room room)
	: m_user(user), m_room(room), m_roomManager(handlerFactory.getRoomManager()), m_handlerFactory(handlerFactory)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.id == CLOSE_ROOM_CODE || requestInfo.id == START_GAME_CODE ||
		requestInfo.id == GET_ROOM_STATE_CODE)
	{
		return true;
	}
	return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (isRequestRelevant(requestInfo))
	{
		if (requestInfo.id == CLOSE_ROOM_CODE)
		{
			return closeRoom(requestInfo);
		}
		if (requestInfo.id == START_GAME_CODE)
		{
			return startGame(requestInfo);
		}
		if (requestInfo.id == GET_ROOM_STATE_CODE)
		{
			return getRoomState(requestInfo);
		}
	}
	else
	{
		RequestResult result;
		result.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user,this->m_room);
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}
}

Room RoomAdminRequestHandler::getRoom()
{
	return this->m_room;
}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo)
{
	RequestResult result;
	result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	CloseRoomResponse closeRoomResponse;
	result.response = JsonResponsePacketSerializer::serializeResponse(closeRoomResponse);
	return result;
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo)
{
	return RequestResult();
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo)
{
	RequestResult result;
	result.newHandler = this;

	GetRoomStateResponse getRoomStateResponse;
	getRoomStateResponse.hasGameBegun = this->m_roomManager.getRoomState(this->m_room.getRoomData().id);
	getRoomStateResponse.players = this->m_room.getAllUsers();
	getRoomStateResponse.questionCount = this->m_room.getRoomData().numOfQuestionsInGame;
	getRoomStateResponse.answerTimeout = this->m_room.getRoomData().timePerQuestion;
	result.response = JsonResponsePacketSerializer::serializeResponse(getRoomStateResponse);
	return result;
}
