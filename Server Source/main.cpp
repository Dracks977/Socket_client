#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include "connexion.h"
#include "ecoute.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;
int main()
{
	SOCKET sock;
	SOCKADDR_IN sin;
	u_short port;
	char pseudo[30];
	cout << "Port: ";
	cin >> port;
	cout << "Pseudo: ";
	cin >> pseudo;
	connexion(port, sock, sin);
	ecoute(sock, sin, pseudo);
	return 0;
}