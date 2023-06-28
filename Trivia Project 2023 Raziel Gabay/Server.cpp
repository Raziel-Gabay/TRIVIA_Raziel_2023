#include "Server.h"


Server::Server()
	:m_handlerFactory(RequestHandlerFactory()), m_communicator(Communicator(m_handlerFactory)), m_database(m_handlerFactory.getDataBase())
{
}
void Server::run()
{
	// open main thread 
	std::thread connectorThread(&Communicator::startHandleRequests, m_communicator);
	connectorThread.detach();

	std::string input;
	while ( input != "EXIT")
		std::cin >> input;
}