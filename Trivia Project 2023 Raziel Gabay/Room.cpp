#include "Room.h"

Room::Room()
	: m_metadata{ 0, "", 0, 0, 0, 0 }, m_users{}
{
}

Room::Room(RoomData metadata, std::vector<LoggedUser> users)
	:m_metadata(metadata), m_users(users)
{
}

void Room::addUser(LoggedUser lu)
{
	bool flag = true;
	for (auto it = this->m_users.begin(); it != this->m_users.end(); ++it)
	{
		if (*it == lu)
		{
			flag == false;
		}
	}
	if (flag) this->m_users.push_back(lu);
}

void Room::removeUser(LoggedUser lu)
{
	for (auto it = this->m_users.begin(); it != this->m_users.end();)
	{
		if (*it == lu)
		{
			it = this->m_users.erase(it);
		}
		else
		{
			++it;
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

void Room::setIsActive(bool isActive)
{
	m_metadata.isActive = isActive;
}
