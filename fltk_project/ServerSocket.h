#pragma once
#include <WinSock2.h>
#include "ClientSocket.h"
#include <memory>
#include <vector>
struct ServerSocket
{
	ServerSocket(int _port, std::string _version);
	~ServerSocket();
	std::shared_ptr<ClientSocket> accept();
	std::string get_version() { return version; }
	void set_version(std::string _version) { version = _version; }

private:
	std::string version;
	SOCKET m_socket;
	std::vector<ClientSocket*> clients;
	//ServerSocket(const ServerSocket& _copy);
	//ServerSocket& operator=(const ServerSocket& _assign);
	
};