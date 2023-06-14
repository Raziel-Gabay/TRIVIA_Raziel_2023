#pragma once

#include <iostream>
#include <WinSock2.h>
#include <mutex>
#include <condition_variable>
#include <map>
#include <string>
//#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "LoginRequestHandler.h"

class Communicator
{
public:
	Communicator();
	~Communicator();
	void startHandleRequests();
private:
	//fields
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory m_handlerFactory;
	//std::mutex m_clientsMutex;

	//methods
	void bindAndListen();
	void handleNewClient(SOCKET);

};