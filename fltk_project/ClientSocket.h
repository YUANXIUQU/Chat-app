#pragma once
#include <winsock2.h>
#include <string>
#include <vector>
class ClientSocket
{
private:
	friend struct ServerSocket;
	SOCKET m_socket;
	//ClientSocket(const ClientSocket& _copy);
	//ClientSocket& operator=(const ClientSocket& _assign);
	bool m_closed;
public:
	ClientSocket();
	ClientSocket(const std::string& _server, int _port);
	~ClientSocket();
	bool receive(std::string& _message);
	void send(const std::string& _message);// no const
	bool closed();
	void close_socket();
	void chageserver(const std::string& _server, int _port);

};