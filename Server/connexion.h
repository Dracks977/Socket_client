#ifndef CONNEXION
#define CONNEXION

#include <winsock2.h>
#include <iostream>

int connexion(u_short port, SOCKET &sock, SOCKADDR_IN &sin);

#endif