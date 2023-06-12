#include "LoginRequestHandler.h"


bool LoginRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	if (reqInfo.id == LOGIN_CODE || reqInfo.id == SIGNUP_CODE)
	{
		return true;
	}
	return false;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;
	result.newHandler = new LoginRequestHandler();
	if (isRequestRelevant(reqInfo))
	{
		if (reqInfo.id == LOGIN_CODE)
		{

			LoginResponse logResponse;
			result.response = JsonResponsePacketSerializer::serializeResponse(logResponse);
		}
		if (reqInfo.id == SIGNUP_CODE)
		{
			SignupResponse signupResponse;
			result.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);
		}
	}
	else
	{
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}
	