#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "serveur.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	
	u_short port;
	string pseudo;
	cout << "Port: ";
	cin >> port;
	cout << "Pseudo: ";
	cin >> pseudo;

	Server server(pseudo, port, 5568);
	server.start();
	server.listenClient();
	return 0;
}