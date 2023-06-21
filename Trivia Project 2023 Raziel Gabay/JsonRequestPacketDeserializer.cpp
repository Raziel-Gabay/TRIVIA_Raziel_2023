#include "JsonRequestPacketDeserializer.h"


LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer loginRequest)
{
	json messageJson = JsonRequestPacketDeserializer::deserializeRequest(loginRequest);
	return LoginRequest{messageJson["username"],messageJson["password"]};
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer signupRequest)
{
	json messageJson = JsonRequestPacketDeserializer::deserializeRequest(signupRequest);
	return SignupRequest{ messageJson["username"],messageJson["password"], messageJson["mail"]};
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(Buffer getPlayerRequest)
{
	json messageJson = JsonRequestPacketDeserializer::deserializeRequest(getPlayerRequest);
	return GetPlayersInRoomRequest{ messageJson["id"]};
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(Buffer joinRoomRequest)
{
	json messageJson = JsonRequestPacketDeserializer::deserializeRequest(joinRoomRequest);
	return JoinRoomRequest{ messageJson["id"] };
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(Buffer createRoomRequest)
{
	json messageJson = JsonRequestPacketDeserializer::deserializeRequest(createRoomRequest);
	return CreateRoomRequest{ messageJson["name"] , messageJson["maxPlayers"], messageJson["numOfQuestionsInGame"],messageJson["timePerQuestion"]};
}

json JsonRequestPacketDeserializer::deserializeRequest(Buffer request)
{
	std::string message;
	for (auto iter = request.begin() + MESSAGE_START_INDEX; iter != request.end(); ++iter)
	{
		message += *iter;
	}

	json messageJson = json::parse(message);

	return messageJson;
}