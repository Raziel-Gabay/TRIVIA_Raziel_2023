#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

#define LOGIN_CODE 100
#define SIGNUP_CODE 101

class RequestHandlerFactory;
class LoginManager;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory);
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
	RequestHandlerFactory& m_handlerFactory;
	LoginManager& m_loginManager;


	RequestResult login(RequestInfo info);
	RequestResult signup(RequestInfo info);
};