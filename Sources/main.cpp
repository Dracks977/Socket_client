/*!
* \file main.cpp
* \brief Fichier main.cpp du programme
* \author Romain
* \version 0.1
* \date 3 juillet 2015
*/

#include "client.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

/*! \fn int main(int argc, char* argv[])
*\brief Fonction main
*
*Fonction principale dans laquelle est cree l'objet client
*
*/

int main(int argc, char* argv[])
{

	char ip[20];
	u_short port;
	string pseudo;

	cout << "================================Chat socket TCP=================================" << endl;
	cout << "Adresse IP du serveur: ";
	cin.getline(ip, 19);
	cout << "Sur le port: ";
	cin >> port;
	cout << "Pseudo: ";
	cin >> pseudo;


	Client c(port, ip, pseudo);
	c.connexionAuServeur();
	c.threadReceiveMessage();
	c.envoieMessage();

	system("Pause");
	return 0;
}