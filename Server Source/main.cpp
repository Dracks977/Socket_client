#include "serveur.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

Server server("romain", 5567, 5568);
void* functionThread(void* data)
{
	
	server.acceptClient();
	cout << "Le thread fonctionne !" << endl;
	return NULL;
}

int main()
{
	u_short port;
	string pseudo;
	pthread_t thread;
	int valeur;
	cout << "Port: ";
	cin >> port;
	cout << "Pseudo: ";
	cin >> pseudo;

	server.start();
	pthread_create(&thread, NULL, functionThread, (void*)&server);
	server.listenClient();
	return 0;
}