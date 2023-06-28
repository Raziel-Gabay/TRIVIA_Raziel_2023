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
		result.newHandler = nullptr;
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}
}

RequestResult LoginRequestHandler::login(RequestInfo info)
{
	RequestResult result;

	LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
	std::cout << "client: {username: " << req.username << ", password: " << req.password << "}" << std::endl;
	if (!this->m_loginManager.isUserLogged(req.username) && this->m_loginManager.doesUserExist(req.username))
	{
		this->m_loginManager.login(req.username, req.password);
		if (this->m_loginManager.isUserLogged(req.username))
		{
			LoginResponse logResponse;
			result.response = JsonResponsePacketSerializer::serializeResponse(logResponse);
			result.newHandler = this->m_handlerFactory.createMenuRequestHandler(req.username);
		}
		else
		{
			ErrorResponse errorResponse;
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
			result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		}
	}
	//if it's not valid
	else
	{
		result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}

RequestResult LoginRequestHandler::signup(RequestInfo info)
{
	RequestResult result;

	SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
	std::cout << "client: {username: " << req.username << ", password: " << req.password << ", mail: " << req.email << "}" << std::endl;
	if (!this->m_loginManager.doesUserExist(req.username))
	{
		result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		this->m_loginManager.signup(req.username, req.password, req.email);
		if (this->m_loginManager.doesUserExist(req.username))
		{
			SignupResponse signupResponse;
			result.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);
		}
		else
		{
			ErrorResponse errorResponse;
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		}
	
	}
	//if it's not valid
	else
	{
		ErrorResponse errorResponse;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}
	