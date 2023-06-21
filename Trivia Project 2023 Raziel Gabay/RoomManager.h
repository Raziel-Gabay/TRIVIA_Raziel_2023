#pragma once

#include "SqliteDataBase.h"
#include "LoggedUser.h"
#include "Room.h"
#include <vector>
#include <map>

class RoomManager
{
public:
	void createRoom(LoggedUser , RoomData);
	void deleteRoom(int ID);
	unsigned int getRoomState(int ID);
	std::vector<RoomData> getRooms();
	Room& getRoom(int ID);
private:
	std::map<unsigned int, Room> m_rooms; //unsigned int = roomID
};

