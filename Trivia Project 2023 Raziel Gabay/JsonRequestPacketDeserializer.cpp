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