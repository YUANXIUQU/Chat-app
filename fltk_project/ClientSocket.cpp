#include "ClientSocket.h"
#include <ws2tcpip.h>
#include <stdexcept>
#include <string>
#include <iostream>
ClientSocket::ClientSocket()
	:m_socket(INVALID_SOCKET)
{
	m_closed= false;
}

ClientSocket::ClientSocket(const std::string& _server, int _port):m_socket(INVALID_SOCKET)
{
	m_closed = false;
	addrinfo hints = { 0 };
	addrinfo* result = NULL;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(_server.c_str(), std::to_string(_port).c_str(), &hints, &result))
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to get address information");
	}

	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_socket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to create socket");
	}

	if (connect(m_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to connect to server");
	}

	u_long mode = 1;
	if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to set non-blocking");
	}
	freeaddrinfo(result);
}

void ClientSocket::chageserver(const std::string& _server, int _port)
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);

		addrinfo hints = { 0 };
		addrinfo* result = NULL;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (getaddrinfo(_server.c_str(), std::to_string(_port).c_str(), &hints, &result))
		{
			freeaddrinfo(result);
		}

		m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (m_socket == INVALID_SOCKET)
		{
			freeaddrinfo(result);
			throw std::runtime_error("Failed to create socket");
		}

		if (connect(m_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
		{
			freeaddrinfo(result);
			throw std::runtime_error("Failed to connect to server");
		}

		u_long mode = 1;
		if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
		{
			throw std::runtime_error("Failed to set non-blocking");
		}
		freeaddrinfo(result);
	}
	
}

ClientSocket::~ClientSocket()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
	}
}

bool ClientSocket::receive(std::string& _message)
{
	char buffer[128] = { 0 };
	int bytes = ::recv(m_socket, buffer, sizeof(buffer) - 1, 0);
	if(bytes == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() == CLOSECHANNEL)
		{
			throw std::runtime_error("Read failed");
		}
		
		return false;
	}
	else if (bytes == 0)
	{
		m_closed = true;
		return false;
	}
	_message = buffer;
	return true;
}

void ClientSocket::send(const std::string& _message)
{
	int bytes = ::send(m_socket, _message.c_str(), _message.length(), 0);
	if (bytes == SOCKET_ERROR) 
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			throw std::runtime_error("Read failed");
		}
	}
}
bool ClientSocket::closed()
{
	return m_closed;
}

void ClientSocket::close_socket()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	m_closed = true;
	std::cout << "close socket";
}