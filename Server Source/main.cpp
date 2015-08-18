#include "serveur.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;



int main()
{
	u_short port;
	string pseudo;
	pthread_t thread1;
	pthread_t thread2;
	data serverData;
	
	cout << "Port: ";
	cin >> port;
	cout << "Pseudo: ";
	cin >> pseudo;

	serverData.mutex = PTHREAD_MUTEX_INITIALIZER;
	serverData.server.setPseudo(pseudo);
	serverData.server.setPort(port);

	serverData.server.start();
	pthread_create(&thread1, NULL, threadAcceptClient, (void*)&serverData);
	pthread_create(&thread2, NULL, threadSendMessage, (void*)&serverData);
	serverData.server.listenClient();

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	return 0;
}