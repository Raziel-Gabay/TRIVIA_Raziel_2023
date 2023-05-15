#include "Server.h"

void Server::run()
{
	// open main thread 
	std::thread connectorThread(&Communicator::startHandleRequests, m_communicator);
	connectorThread.detach();

	std::string input;
	while ( input != "EXIT")
		std::cin >> input;
}