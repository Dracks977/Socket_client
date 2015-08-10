#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "serveur.h"

#define NOMBRE_OCTET 2048
using namespace std;

Server::Server() : m_sockServer(0), m_sockMusic(0), m_cSock(0), m_pseudo(0), m_port(0), m_portMusic(0), m_buffer(0), m_bufferMusic(0)
{
	m_sockServer = new SOCKET;
	m_sockMusic = new SOCKET;
	m_cSock = new SOCKET;
	m_pseudo = new string;
	m_message = new string;
	m_port = new u_short;
	m_portMusic = new u_short;
	m_buffer = new char[NOMBRE_OCTET];
	m_bufferMusic = new char[NOMBRE_OCTET];
	m_erreur = new int;
}

Server::Server(std::string pseudo, u_short port, u_short portMusic) : m_sockServer(0), m_sockMusic(0), m_cSock(0), m_pseudo(0), m_port(0), m_portMusic(0), m_buffer(0), m_bufferMusic(0)
{
	m_sockServer = new SOCKET;
	m_sockMusic = new SOCKET;
	m_cSock = new SOCKET;
	m_pseudo = new string;
	m_message = new string;
	m_port = new u_short;
	m_portMusic = new u_short;
	m_buffer = new char[NOMBRE_OCTET];
	m_bufferMusic = new char[NOMBRE_OCTET];
	m_erreur = new int;

	*m_pseudo = pseudo;
	*m_port = port;
	*m_portMusic = portMusic;
}

int Server::start()
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "Erreur de la fonction WSAStarup dans le constructeur de la classe client" << endl;
		*m_erreur = WSAGetLastError();
		cout << "Erreur " << *m_erreur << endl;
		return *m_erreur;
	}
	*m_sockServer = socket(AF_INET, SOCK_STREAM, 0);
	m_sin.sin_addr.s_addr = INADDR_ANY;
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(*m_port);
	bind(*m_sockServer, (SOCKADDR *)&m_sin, sizeof(m_sin));
	listen(*m_sockServer, 5);
	cout << "Ecoute du port: " << *m_port << endl;
	return 0;
}

int Server::sendMusic()
{
	cout << "Fonction musique" << endl;
	SOCKET csockMusique;
	SOCKADDR_IN csinMusique;

	*m_sockMusic = socket(AF_INET, SOCK_STREAM, 0);
	m_sinMusic.sin_addr.s_addr = INADDR_ANY;
	m_sinMusic.sin_family = AF_INET;
	m_sinMusic.sin_port = htons(5568);
	bind(*m_sockMusic, (SOCKADDR *)&m_sinMusic, sizeof(m_sinMusic));
	listen(*m_sockMusic, 0);

	cout << "Parametrage termine" << endl;

	int sinMsize = sizeof(csinMusique);
	csockMusique = accept(*m_sockMusic, (SOCKADDR *)&csinMusique, &sinMsize);

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
		tourBoucle++;
		for (int i = 0; i < NOMBRE_OCTET; i++)
			bufferM[i] = pbuf->sbumpc();
		cout << '\r' << "Progression : " << (100 / taille)*(tourBoucle*NOMBRE_OCTET)
			<< "     octets: " << tourBoucle*NOMBRE_OCTET;
		send(csockMusique, bufferM, NOMBRE_OCTET, 0);
		Sleep(40);
	}
	cout << endl << "Fin de la reception" << endl;
	closesocket(*m_sockMusic);
	closesocket(csockMusique);

	return 0;
}

void Server::listenClient()
{
	string message;

	while (1)
	{
		getline(cin, message);

		send(*m_cSock, message.c_str(), sizeof(message), 0);
		cout << *m_pseudo << ">";
		if (message == "/music")
			sendMusic();
	}

}

int Server::acceptClient()
{
    
	

	cout << "Le serveur nomme " << *m_pseudo << " ecoute maintenant les connexions entrantes" << endl;

	int sinsize = sizeof(m_cSin);
	while (*m_message != "/off")
	{
		*m_cSock = accept(*m_sockServer, (SOCKADDR *)&m_cSin, &sinsize);
		if (*m_cSock != INVALID_SOCKET)
		{
			cout << "Nouveau client" << endl;
			recv(*m_sockServer, m_buffer, NOMBRE_OCTET, 0);
			send(*m_cSock, m_pseudo->c_str(), 30, 0);
			listeClient.push_back(*m_cSock);
		}
	}
	return 0;
}