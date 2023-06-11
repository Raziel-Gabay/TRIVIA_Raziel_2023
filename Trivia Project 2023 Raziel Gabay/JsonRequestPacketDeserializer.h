#pragma once

#include "json.hpp"
#include <vector>
#include <bitset>

#define MESSAGE_START_INDEX 5

using json = nlohmann::json;

typedef struct LoginRequest
{
	std::string username;
	std::string password;
} LoginRequest;

typedef struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
} SignupRequest;

typedef std::vector<unsigned char> Buffer; // The Buffer hold the data we want to hold or recive


class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(Buffer);
	static SignupRequest deserializeSignupRequest(Buffer);
	static json deserializeRequest(Buffer);

};

