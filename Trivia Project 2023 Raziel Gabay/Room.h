#pragma once

#include <iostream>
#include <vector>
#include "LoggedUser.h"
struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

class Room
{
public:
	Room();		
	Room(RoomData metadata, std::vector<LoggedUser> users);
	void addUser(LoggedUser lu);
	void removeUser(LoggedUser lu);
	std::vector<std::string> getAllUsers();
	RoomData getRoomData();
	void setIsActive(bool isActive);
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
};

