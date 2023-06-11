#pragma once

#include "IRequestHandler.h"

#define LOGIN_CODE 100
#define SIGNUP_CODE 101

class LoginRequestHandler : public IRequestHandler
{
public:
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;
};