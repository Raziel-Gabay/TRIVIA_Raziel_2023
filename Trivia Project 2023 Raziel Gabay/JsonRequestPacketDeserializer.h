#pragma once

#include "json.hpp"
#include <iostream>
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

struct GetPlayersInRoomRequest
{
	unsigned int roomid;
};

struct JoinRoomRequest
{
	unsigned int roomid;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};


typedef std::vector<unsigned char> Buffer; // The Buffer hold the data we want to hold or recive


class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(Buffer);
	static SignupRequest deserializeSignupRequest(Buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(Buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(Buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(Buffer);
	static json deserializeRequest(Buffer);

};


