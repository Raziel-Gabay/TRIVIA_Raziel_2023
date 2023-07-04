#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user, Room& room)
	: m_user(user), m_room(room), m_roomManager(handlerFactory.getRoomManager()), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.id == LEAVE_ROOM_CODE || requestInfo.id == GET_ROOM_STATE_CODE)
	{
		return true;
	}
	return false;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (isRequestRelevant(requestInfo))
	{
		if (requestInfo.id == LEAVE_ROOM_CODE)
		{
			return leaveRoom(requestInfo);
		}
		if (requestInfo.id == GET_ROOM_STATE_CODE)
		{
			return getRoomState(requestInfo);
		}
	}
	else
	{
		RequestResult result;
		result.newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(this->m_user,this->m_room);
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}
}

LoggedUser RoomMemberRequestHandler::getUser()
{
	return this->m_user;
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo)
{
	RequestResult result;
	result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);

	this->m_room.removeUser(this->m_user);

	LeaveRoomResponse leaveRoomResponse;
	result.response = JsonResponsePacketSerializer::serializeResponse(leaveRoomResponse);
	return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo)
{
	RequestResult result;
	result.newHandler = this;

	GetRoomStateResponse getRoomStateResponse;
	getRoomStateResponse.hasGameBegun = !this->m_roomManager.getRoomState(this->m_room.getRoomData().id);
	getRoomStateResponse.players = this->m_room.getAllUsers();
	getRoomStateResponse.questionCount = this->m_room.getRoomData().numOfQuestionsInGame;
	getRoomStateResponse.answerTimeout = this->m_room.getRoomData().timePerQuestion;
	result.response = JsonResponsePacketSerializer::serializeResponse(getRoomStateResponse);
	return result;
}
