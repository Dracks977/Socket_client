#ifndef ECOUTE
#define ECOUTE

#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <string>

class Server
{
public:
	Server();
	int start(u_short port, SOCKET &sock, SOCKADDR_IN &sin);

private:
	SOCKET *m_sockServer;
	SOCKADDR_IN m_sin;
	std::string *m_pseudo;
	long *m_port;
	long *m_portMusic;
	char *m_buffer;
	char *m_bufferMusic;
};


int ecoute(SOCKET sock,SOCKADDR_IN sin, std::string pseudo);

#endif