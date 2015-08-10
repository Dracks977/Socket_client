#ifndef SERVER_H
#define SERVER_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>
#include <list>


#define NOMBRE_OCTET 2048


class Server
{
public:
	Server();
	Server(std::string pseudo, u_short port, u_short portMusic);
	int start();
	void listenClient();
	int acceptClient();
	int sendMusic();

private:
	SOCKET *m_sockServer;
	SOCKET *m_sockMusic;
	SOCKET *m_cSock;
	SOCKADDR_IN m_sin;
	SOCKADDR_IN m_sinMusic;
	SOCKADDR_IN m_cSin;
	std::string *m_pseudo;
	std::string *m_message;/*!< Message saisi et envoye par le client*/
	u_short *m_port;
	u_short *m_portMusic;
	char *m_buffer;
	char *m_bufferMusic;
	int *m_erreur;


	std::list<SOCKET> listeClient;
};

#endif