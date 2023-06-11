#pragma once

#include "json.hpp"
#include <vector>
#include <bitset>

using json = nlohmann::json;

#define MESSAGE_INDEX 5u
#define ERROR_CODE 99
#define LOGIN_CODE 100
#define SIGNUP_CODE 101

typedef std::vector<unsigned char> Buffer; // The Buffer hold the data we want to hold or recive

typedef struct ErrorResponse
{
	std::string message = "ERROR";
} ErrorResponse;

typedef struct LoginResponse
{
	unsigned int status = 1;
} LoginResponse;

typedef struct SignupResponse
{
	unsigned int status = 1;
} SignupResponse;


class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse( ErrorResponse);
	static Buffer serializeResponse( LoginResponse);
	static Buffer serializeResponse( SignupResponse);
	static Buffer serializeResponse(std::string response, char code);
};