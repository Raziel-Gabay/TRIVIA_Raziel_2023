#include "LoginRequestHandler.h"


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory)
	:m_handlerFactory(handlerFactory), m_loginManager(handlerFactory.getLoginManager())
{
}

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
	if (isRequestRelevant(reqInfo))
	{
		if (reqInfo.id == LOGIN_CODE)
		{
			return login(reqInfo);
		}
		if (reqInfo.id == SIGNUP_CODE)
		{
			return signup(reqInfo);
		}
	}
	else
	{
		RequestResult result;
		result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}
}

RequestResult LoginRequestHandler::login(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this->m_handlerFactory.createLoginRequestHandler();

	LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
	if (!this->m_loginManager.isUserLogged(req.username))
	{
		this->m_loginManager.login(req.username, req.password);
		LoginResponse logResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(logResponse);
	}
	//if it's not valid
	else
	{
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}

RequestResult LoginRequestHandler::signup(RequestInfo info)
{
	RequestResult result;
	result.newHandler = this->m_handlerFactory.createLoginRequestHandler();

	SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
	if (!this->m_loginManager.doesUserExist(req.username))
	{
		this->m_loginManager.signup(req.username, req.password, req.email);
		SignupResponse signupResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);
	}
	//if it's not valid
	else
	{
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}
	