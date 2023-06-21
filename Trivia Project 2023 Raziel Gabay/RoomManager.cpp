#include "RoomManager.h"

void RoomManager::createRoom(LoggedUser user, RoomData data)
{
	std::vector<LoggedUser> users;
	users.push_back(user);
	this->m_rooms[data.id] = Room((data),users);
}

void RoomManager::deleteRoom(int ID)
{
	//this->m_rooms.erase(this->m_rooms[ID])
	for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); ++it)
	{
		if (it->first == ID)
		{
			this->m_rooms.erase(it);
		}
	}
}

unsigned int RoomManager::getRoomState(int ID)
{
	return this->m_rooms[ID].getRoomData().isActive;
}

std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> rooms;
	for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); ++it)
	{
		rooms.push_back(it->second.getRoomData());
	}
	return rooms;
}

Room& RoomManager::getRoom(int ID)
{
	return this->m_rooms[ID];
}
