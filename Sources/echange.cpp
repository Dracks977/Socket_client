#include "echange.h"

using namespace std;


Client::Client() : m_erreur(0),
				   m_pseudo(DEFAULT_PSEUDO),
				   m_port(DEFAULT_PORT),
				   m_portMusique(DEFAULT_PORT_MUSIQUE),
				   m_ipServeur(DEFAULT_IP)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "Erreur de la fonction WSAStarup dans le constructeur de la classe client" << endl;
		m_erreur = WSAGetLastError();
		cout << "Erreur " << m_erreur << endl;
	}

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	m_sin.sin_addr.s_addr = inet_addr(m_ipServeur.c_str());
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(m_port);

	m_sockMusique = socket(AF_INET, SOCK_STREAM, 0);
	m_sinMusique.sin_addr.s_addr = inet_addr(m_ipServeur.c_str());
	m_sinMusique.sin_family = AF_INET;
	m_sinMusique.sin_port = htons(m_portMusique);  
}

Client::Client(u_short port, string ip, string pseudo) : m_erreur(0),
														 m_pseudo(pseudo),
														 m_port(port),
														 m_portMusique(DEFAULT_PORT_MUSIQUE),
														 m_ipServeur(ip)
														 

{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "Erreur de la fonction WSAStarup dans le constructeur de la classe client" << endl;
		m_erreur = WSAGetLastError();
		cout << "Erreur " << m_erreur << endl;

	}

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	m_sin.sin_addr.s_addr = inet_addr(m_ipServeur.c_str());
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(m_port);

	m_sockMusique = socket(AF_INET, SOCK_STREAM, 0);
	m_sinMusique.sin_addr.s_addr = inet_addr(m_ipServeur.c_str());
	m_sinMusique.sin_family = AF_INET;
	m_sinMusique.sin_port = htons(m_portMusique);

}

Client::~Client()
{
	closesocket(m_sock);
	WSACleanup();
}

int Client::connexionAuServeur()
{
	
	if (m_sock == INVALID_SOCKET)
	{
		cout << "Erreur de la fonction socket dans la methode 'connectToServer' de la classe client";
		m_erreur = WSAGetLastError();
		cout << "Erreur " << m_erreur << endl;
        
		return m_erreur;
	}
    cout << endl << "Connexion au serveur: " << m_ipServeur << " sur le port " << m_port << endl;
	if (connect(m_sock, (SOCKADDR *)&m_sin, sizeof(m_sin)) == 0)
	{
		recv(m_sock, m_pseudoServeur, 30, 0);
	    cout << "Connexion etablie avec " << m_pseudoServeur << endl;

		return 0;
	}
	else
	{
		system("color 4");
		cout << "Impossiblde de se connecter au port " << m_port << " du serveur " << m_ipServeur << endl;
		m_erreur = WSAGetLastError();
		cout << "Erreur " << m_erreur << endl;

		return m_erreur;
	}
		
}

int Client::envoieMessage()
{
	thread (&Client::recevoirMessage, this).detach();

	if (m_erreur == 0)//On verifie que le socket n'a pas rencontre d'erreur avant cela
	{
		while (commande() != QUITTER )//On verifie que l'utilisateur n'ai pas saisie '/quit'
		{
			getline(cin, m_message);
			if (send(m_sock, m_message.c_str(), sizeof(m_message), 0) != sizeof(m_message))
			{
				cout << "Impossible d'envoyer le message a " << m_ipServeur << " ! Erreur: " << WSAGetLastError() << endl;
			}
			cout << m_pseudo << ">";
		}

		return 0;
	}
	return 1;
}

void Client::recevoirMessage()
{
	if (m_erreur == 0)//On verifie que le socket n'a pas rencontre d'erreur avant cela
	{
		while (commande() != QUITTER && commandeServeur() != SERVEUR_OFF)
		{
			m_resultat = recv(m_sock, m_buffer, 4096, 0);
			commandeServeur();
			if (m_resultat > 0)
			{
				cout << endl << m_pseudoServeur << ">" << m_buffer << endl;
				cout << m_pseudo << ">" << m_message;
			}
		}
	}
}

int Client::recevoirMusique()
{
	cout << "Fonction musique" << endl;
	//Connexion au port diffusant la musique
	if (m_sockMusique == INVALID_SOCKET)
	{
		cout << "Erreur de la fonction socket dans la methode 'recevoirMusique' de la classe client";
		m_erreur = WSAGetLastError();
		cout << "Erreur " << m_erreur << endl;

		return m_erreur;
	}
	cout << endl << "Connexion au serveur: " << m_ipServeur << " sur le port " << m_portMusique << endl;
	if (connect(m_sockMusique, (SOCKADDR *)&m_sinMusique, sizeof(m_sinMusique)) == 0)
	{
		cout << "Connexion etablie " << endl;
	}
	else
	{
		system("color 4");
		cout << "Impossiblde de se connecter au port " << m_portMusique << " du serveur " << m_ipServeur << endl;
		m_erreur = WSAGetLastError();
		cout << "Erreur " << m_erreur << endl;

		return m_erreur;
	}
	//Recuperation de la taille du fichier
	char bufferM[512];
	long size;
	stringstream convertion;
	m_resultat = recv(m_sockMusique, bufferM, 512, 0);
	convertion << bufferM;
	convertion >> size;
	cout << "Taille: " << size << "octets" << endl;
    //Ouverture du fichier d'ecriture
	ofstream fichierEcriture("temporaire.mp3", ofstream::binary | ios::app);
	//Si le fichier ne s'ouvre pas
	if (!fichierEcriture)
	{
		cout << "Impposible d'acceder au fichier temporaire" << endl;
		return 1;
	}
	else
	{
		cout << "Debut de la reception de la musique" << endl;
		//On recoit les paquets tant que le fichier n'est pas complet
		for (int i = 0; i < size; i += 512)
		{
			i++;
			m_resultat = recv(m_sockMusique, bufferM, 512, 0);
			fichierEcriture.write(bufferM, 512);
		}
		cout << "Fin de la reception" << endl;
		return 0;
	}
}
	


int Client::commande()
{
	if (m_message == "/quit")
		return QUITTER;
	else if (m_message == "/reboot")
		return REDEMARER;
	else if (m_message == "/save")
	{
		sauvegardeParametre();
		return SAVE;
	}
	else if (m_message == "/liste")
		return LISTE;
	else
		return NO_COMMANDE;
}

int Client::commandeServeur()
{
	if (m_buffer[0] != '/')
		return NO_COMMANDE;
	else if (strstr(m_buffer, "/music") != NULL)
	{
		recevoirMusique();
		return SERVEUR_MUSIQUE;
	}

}

int Client::sauvegardeParametre()
{
	ofstream fichierSauvegarde("data.tsc");
	if (!fichierSauvegarde)
	{
		cout << "Impposible d'acceder au fichier de sauvegarde" << endl;
		return 1;
	}
	else
	{
		fichierSauvegarde << m_ipServeur;
		fichierSauvegarde << endl;
		fichierSauvegarde << m_port<< endl;
		fichierSauvegarde << m_pseudo << endl;
		return 0;
	}
}

int Client::chargerParametre()
{
	ifstream fichierSauvegarde("data.tsc");
	if (!fichierSauvegarde)
	{
		cout << "Impposible d'acceder au fichier de sauvegarde" << endl;
		return 1;
	}
	else
	{
		string ligneLue;
		//Lecture de l'ip du serveur
		fichierSauvegarde >> ligneLue;
		for (int i(0); i < sizeof(ligneLue); ++i)
			m_ipServeur[i] = ligneLue[i];
		//Lecture du port
		fichierSauvegarde >> m_port;
		//Lecture du pseudo
		fichierSauvegarde >> m_pseudo;
		return 0;
	}
}

SOCKET Client::getSocket() const
{
	return m_sock;
}

string Client::getPseudo() const
{
	return m_pseudo;
}

string Client::getIpServeur() const
{
	return m_ipServeur;
}

int Client::getErreur() const
{
	return m_erreur;
}
