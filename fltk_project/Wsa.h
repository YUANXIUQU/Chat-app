#pragma once
#include <winsock2.h>
struct Wsa
{
	Wsa();
	~Wsa();
private:
	WSADATA m_wsadata;
	Wsa(const Wsa& _Copy);
	Wsa& operator=(const Wsa& _assign);
};