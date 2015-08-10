#include "serveur.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

typedef struct data{
	Server server;
	pthread_mutex_t mutex;

}data;


void* threadAcceptClient(void* p_data)
{
	data *dataServer = (data*)p_data;
	dataServer->server.acceptClient();
	return NULL;
}

void* threadSendMessage(void* p_data)
{
	data *dataServer = (data*)p_data;
	dataServer->server.sendMessage();
	return NULL;
}

int main()
{
	u_short port;
	string pseudo;
	pthread_t thread;
	data serverData;
	
	int valeur;
	cout << "Port: ";
	cin >> port;
	cout << "Pseudo: ";
	cin >> pseudo;

	serverData.mutex = PTHREAD_MUTEX_INITIALIZER;
	serverData.server.setPseudo(pseudo);
	serverData.server.setPort(port);

	serverData.server.start();
	pthread_create(&thread, NULL, threadAcceptClient, (void*)&serverData);
	pthread_create(&thread, NULL, threadSendMessage, (void*)&serverData);
	serverData.server.listenClient();
	return 0;
}