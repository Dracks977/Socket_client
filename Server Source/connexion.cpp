#include "connexion.h"

using namespace std;

int connexion(u_short port, SOCKET &sock, SOCKADDR_IN &sin)
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	bind(sock, (SOCKADDR *)&sin, sizeof(sin));
	listen(sock, 0);
	cout << "Ecoute du port: " << port << endl;
	return 0;
}