#include "Communicator.h"

#define PORT 8826
#define IFACE 0
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__)
#define BUFFER_SIZE 1000
#define CODE_INDEX 0
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

// recive client request, build the RequestInfo
void Communicator::handleNewClient(SOCKET clientSocket)
{
	// recive client request and build the RequestInfo
	// [code] [contet size] [contet]
	// 1bytes   4 bytes	    size bytes
	char clientMSG[BUFFER_SIZE] = { NULL };
	recv(clientSocket, clientMSG, BUFFER_SIZE, NULL);
	size_t len = MESSAGE_INDEX;
	len += strlen(clientMSG + len);

	RequestInfo reqInfo;
	reqInfo.id = (unsigned int)clientMSG[CODE_INDEX];
	reqInfo.receivalTime = std::time(0); // get time now
	reqInfo.buffer.resize(len);
	std::copy(clientMSG, clientMSG + len, reqInfo.buffer.begin()); //convert the message to Buffer type 

	//printing the requset of the user
	if (reqInfo.id == LOGIN_CODE)
	{
		LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(reqInfo.buffer);
		cout << "client: {username: " << req.username << ", password: " << req.password << "}" << endl;
	}
	if (reqInfo.id == SIGNUP_CODE)
	{
		SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(reqInfo.buffer);
		cout << "client: {username: " << req.username << ", password: " << req.password << ", mail: " << req.email << "}" << endl;
	}
	//handling the request
	RequestResult reqResult = this->m_clients[clientSocket]->handleRequest(reqInfo);

	// send response to client
	std::string serverMSG = std::string(std::begin(reqResult.response), std::end(reqResult.response));
	send(clientSocket, serverMSG.c_str(), serverMSG.length(), NULL);

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
		this->m_clients[client_socket] = new LoginRequestHandler(); 

		// create new thread for client	and detach from it
		std::thread tr(&Communicator::handleNewClient, this, client_socket);
		tr.detach();
	}
}
