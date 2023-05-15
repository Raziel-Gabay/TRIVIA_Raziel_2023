#pragma once

#include "json.hpp"
#include <vector>

typedef struct ErrorResponse
{
	std::string message;
} ErrorResponse;

typedef struct LoginResponse
{
	unsigned int status;
} LoginResponse;

typedef struct SignupResponse
{
	unsigned int status;
} SignupResponse;

typedef std::vector<char> Buffer; // The Buffer hold the data we want to hold or recive

class JsonResponsePacketSerializer
{
public:
	Buffer serializeResponse( ErrorResponse);
	Buffer serializeResponse( LoginResponse);
	Buffer serializeResponse( SignupResponse);
};