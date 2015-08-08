#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "serveur.h"
#include <fstream>

#define NOMBRE_OCTET 2048
using namespace std;





Server::Server()
{
	m_sockServer = new SOCKET(AF_INET, SOCK_STREAM, 0);

}

int Server::start(u_short port, SOCKET &sock, SOCKADDR_IN &sin)
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



void musique()
{
	cout << "Fonction musique" << endl;
	SOCKET sockMusique;
	SOCKET csockMusique;
	SOCKADDR_IN sinMusique;
	SOCKADDR_IN csinMusique;

	sockMusique = socket(AF_INET, SOCK_STREAM, 0);
	sinMusique.sin_addr.s_addr = INADDR_ANY;
	sinMusique.sin_family = AF_INET;
	sinMusique.sin_port = htons(5568);
	bind(sockMusique, (SOCKADDR *)&sinMusique, sizeof(sinMusique));
	listen(sockMusique, 0);

	cout << "Parametrage termine" << endl;

	int sinMsize = sizeof(csinMusique);
	csockMusique = accept(sockMusique, (SOCKADDR *)&csinMusique, &sinMsize);

	cout << "Connexion reussie" << endl;

	ifstream fichier("Blaster.mp3", ifstream::binary);
	filebuf *pbuf = fichier.rdbuf();

	fichier.seekg(0, fichier.end);
	long size = fichier.tellg();
	double taille = size;
	fichier.seekg(0);
	char bufferM[NOMBRE_OCTET];


	double tourBoucle = 0;


	sprintf(bufferM, "%ld", size);
	send(csockMusique, bufferM, NOMBRE_OCTET, 0);

	cout << "Debut de l'envoie de la musique" << endl;

	while (pbuf->sgetc() != EOF)
	{
		tourBoucle ++;
		for (int i = 0; i < NOMBRE_OCTET; i++)
			bufferM[i] = pbuf->sbumpc();
		cout << '\r' << "Progression : " << (100/taille)*(tourBoucle*NOMBRE_OCTET)
			<< "     octets: " << tourBoucle*NOMBRE_OCTET;
		send(csockMusique, bufferM, NOMBRE_OCTET, 0);
		Sleep(40);
	}
	cout << endl << "Fin de la reception" << endl;
	closesocket(sockMusique);
	closesocket(csockMusique);
}

int ecoute(SOCKET sock,SOCKADDR_IN sin, string pseudo)
{
	SOCKET csock;
	SOCKADDR_IN csin;
	char buffer[255];
	string message;
	cout << "Le serveur nomme " << pseudo << " ecoute maintenant les connexions entrantes" << endl;
	int sinsize = sizeof(csin);
	csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
    send(csock, pseudo.c_str(), 30, 0);
	cout << "Connexion entrante" << endl;

	while (1)
	{
		getline(cin, message);

		send(csock, message.c_str(), sizeof(message), 0);
		cout << pseudo << ">";
		if (message == "/music")
			musique();
	}
	cout << "OK !" << endl;
	system("PAUSE");
	closesocket(sock);
	return 0;
}