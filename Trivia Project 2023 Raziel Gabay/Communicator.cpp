#include "Communicator.h"

#define PORT 8826
#define IFACE 0
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__)
#define BUFFER_SIZE 50
using std::endl;
using std::cout;
using std::cin;


Communicator::Communicator()
{
	m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
	bindAndListen();
}
Communicator::~Communicator()
{
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;
	// again stepping out to the global namespace
	if (::bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	TRACE("binded");

	if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	TRACE("listening...");
}


void Communicator::handleNewClient(SOCKET clientSocket)
{
	// send message to client
	std::string serverMSG = "hello";
	send(clientSocket, serverMSG.c_str(), serverMSG.length(), NULL);
	
	// recive message to client
	char clientMSG[BUFFER_SIZE] = {NULL};
	recv(clientSocket, clientMSG, BUFFER_SIZE, NULL);
	cout << "client:" << clientMSG << endl;

	// close the socket
	closesocket(clientSocket);
}

void Communicator::startHandleRequests()
{
	while (true)
	{
		SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		TRACE("Client accepted !");
		//this->m_clients[client_socket] = new LoginRequestHandler(); // ???????????????

		// create new thread for client	and detach from it
		std::thread tr(&Communicator::handleNewClient, this, client_socket);
		tr.detach();
		
		
	}
}
