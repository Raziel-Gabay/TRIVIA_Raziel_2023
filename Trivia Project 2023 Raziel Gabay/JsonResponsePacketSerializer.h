#pragma once

#include "json.hpp"
#include <vector>
#include <list>
#include <map>
#include <bitset>
#include <variant>
#include <ios>
#include "Room.h"

using json = nlohmann::json;

#define MESSAGE_INDEX 5u
#define ERROR_CODE 99
#define LOGIN_CODE 100
#define SIGNUP_CODE 101
#define LOGOUT_CODE 102
#define GET_ROOMS_CODE 103
#define GET_PLAYERS_IN_ROOM_CODE 104
#define GET_HIGHSCORE_CODE 105
#define GET_PERSONAL_STATS_CODE 106
#define JOIN_ROOM_CODE 107
#define CREATE_ROOM_CODE 108


typedef std::vector<unsigned char> Buffer; // The Buffer hold the data we want to hold or recive

struct ErrorResponse
{
	std::string message = "ERROR";
};

struct LoginResponse
{
	unsigned int status = 1;
};

struct SignupResponse
{
	unsigned int status = 1;
};

struct LogoutResponse
{
	unsigned int status = 1;
};

struct GetRoomsResponse
{
	unsigned int status = 1;
	std::vector<RoomData> rooms;
};


struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};

struct GetHighScoreResponse
{
	unsigned int status = 1;
	std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse
{
	unsigned int status = 1;
	std::vector<std::string> statistics;
};

struct JoinRoomResponse
{
	unsigned int status = 1;
};

struct CreateRoomResponse
{
	unsigned int status = 1;
};



class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(ErrorResponse);
	static Buffer serializeResponse(LoginResponse);
	static Buffer serializeResponse(SignupResponse);
	static Buffer serializeResponse(LogoutResponse);
	static Buffer serializeResponse(GetRoomsResponse);
	static Buffer serializeResponse(GetPlayersInRoomResponse);
	static Buffer serializeResponse(GetHighScoreResponse);
	static Buffer serializeResponse(GetPersonalStatsResponse);
	static Buffer serializeResponse(JoinRoomResponse);
	static Buffer serializeResponse(CreateRoomResponse);
	static Buffer serializeResponse(std::string response, char code);
};