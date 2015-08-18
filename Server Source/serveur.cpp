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
	m_resultat = new int;
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
	m_resultat = new int;

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
	listen(*m_sockServer, 6);
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

int Server::listenClient() 
{
	fd_set st;
	timeval time;

	time.tv_sec = 0;
	time.tv_usec = 500;

		while (*m_message != "/off")
		{
			
			for (list<connectedClient>::iterator it = listeClient.begin(); it != listeClient.end(); it++)
			{
				FD_ZERO(&st);
				FD_SET(it->socket, &st);
				*m_resultat = select(it->socket, &st, NULL, NULL, &time);
				if (*m_resultat == -1)
				{
					*m_erreur = WSAGetLastError();
					cout << "Erreur lors du select" << endl;
				}
				else if (FD_ISSET(it->socket, &st))
				{
					if (recv(it->socket, m_buffer, NOMBRE_OCTET, 0) > 0)
					{
						cout << '\r' << it->pseudo << ">" << m_buffer << endl;
						cout << *m_pseudo << ">";
					}
					else
					{
						*m_erreur = WSAGetLastError();
						cout << "Erreur lors de la reception du message provenant de " << it->pseudo << endl;
					}
				}

			}

		}
	
	return 0;
}

int Server::sendMessage()
{
	while (*m_message != "/off")
	{
		getline(cin, *m_message);
		for (list<connectedClient>::iterator it = listeClient.begin(); it != listeClient.end(); it++)
		{
			if (send(it->socket, m_message->c_str(), NOMBRE_OCTET, 0) != NOMBRE_OCTET)
			{
				cout << "Impossible d'envoyer le message a " << it->pseudo << " ! Erreur: " << WSAGetLastError() << endl;
			}
		}
			
		cout << *m_pseudo << ">";
		if (*m_message == "/music")
			sendMusic();
	}
	return 0;
}


int Server::acceptClient()
{
	cout << "Le serveur nomme " << *m_pseudo << " ecoute maintenant les connexions entrantes" << endl;

	u_long arg=0;
	connectedClient client;

	int sinsize = sizeof(m_cSin);
	while (*m_message != "/off")
	{
		client.socket = accept(*m_sockServer, (SOCKADDR *)&m_cSin, &sinsize);
		if (client.socket != INVALID_SOCKET)
		{
			cout << '\r' << "Nouveau client" << endl;
			recv(client.socket, m_buffer, 30, 0);
			send(client.socket, m_pseudo->c_str(), 30, 0);
			client.pseudo = "test";
			listeClient.push_back(client);
		}
	}
	return 0;
}

void Server::setPort(u_short port)
{
	*m_port = port;
}
void Server::setMusicPort(u_short port)
{
	*m_portMusic = port;
}
void Server::setPseudo(string pseudo)
{
	*m_pseudo = pseudo;
}

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