#include "Room.h"

Room::Room(RoomData metadata, std::vector<LoggedUser> users)
	:m_metadata(metadata), m_users(users)
{
}

void Room::addUser(LoggedUser lu)
{
	this->m_users.push_back(lu);
}

void Room::removeUser(LoggedUser lu)
{
	for (auto it = this->m_users.begin(); it != this->m_users.end(); ++it)
	{
		if (*it == lu)
		{
			this->m_users.erase(it);
		}
	}
}

std::vector<std::string> Room::getAllUsers()
{
	std::vector<std::string> allUsers;
	for (auto it = this->m_users.begin(); it != this->m_users.end(); ++it)
	{
		allUsers.push_back(it->getUsername());
	}
	return allUsers;
}

RoomData Room::getRoomData()
{
	return this->m_metadata;
}
