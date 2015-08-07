#include "echange.h"

using namespace std;


Client::Client() : m_sock(0), m_sockMusique(0), m_port(0), m_portMusique(0), m_ipServeur(0), m_pseudo(0), m_message(0), m_pseudoServeur(0),
m_buffer(0), m_bufferMusique(0), m_resultat(0), m_erreur(0)
{
	m_erreur = new int;
	m_resultat = new int;
	m_bufferMusique = new char[512];
	m_buffer = new char[512];
	m_pseudoServeur = new char[30];
	m_message = new string;
	m_pseudo = new string;
	m_ipServeur = new string;
	m_portMusique = new u_short;
	m_port = new u_short;
	m_sock = new SOCKET;

	*m_erreur = 0;
	*m_resultat = 0;
	*m_pseudo = DEFAULT_PSEUDO;
	*m_ipServeur = DEFAULT_IP;
	*m_portMusique = DEFAULT_PORT_MUSIQUE;
	*m_port = DEFAULT_PORT;
	m_sockMusique = new SOCKET;

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "Erreur de la fonction WSAStarup dans le constructeur de la classe client" << endl;
		*m_erreur = WSAGetLastError();
		cout << "Erreur " << *m_erreur << endl;
	}

	*m_sock = socket(AF_INET, SOCK_STREAM, 0);
	m_sin.sin_addr.s_addr = inet_addr(m_ipServeur[0].c_str());
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(*m_port);

	*m_sockMusique = socket(AF_INET, SOCK_STREAM, 0);
	m_sinMusique.sin_addr.s_addr = inet_addr(m_ipServeur[0].c_str());
	m_sinMusique.sin_family = AF_INET;
	m_sinMusique.sin_port = htons(*m_portMusique);  
}

Client::Client(u_short port, string ip, string pseudo) : m_sock(0), m_sockMusique(0), m_port(0), m_portMusique(0), m_ipServeur(0), m_pseudo(0),
													m_message(0), m_pseudoServeur(0), m_buffer(0), m_bufferMusique(0), m_resultat(0), m_erreur(0)
{
	m_erreur = new int;
	m_resultat = new int;
	m_bufferMusique = new char[512];
	m_buffer = new char[512];
	m_pseudoServeur = new char[30];
	m_message = new string;
	m_pseudo = new string;
	m_ipServeur = new string;
	m_portMusique = new u_short;
	m_port = new u_short;
	m_sock = new SOCKET;
	m_sockMusique = new SOCKET;

	*m_erreur = 0;
	*m_resultat = 0;
	*m_pseudo = pseudo;
	*m_ipServeur = ip;
	*m_portMusique = DEFAULT_PORT_MUSIQUE;
	*m_port = port;

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "Erreur de la fonction WSAStarup dans le constructeur de la classe client" << endl;
		*m_erreur = WSAGetLastError();
		cout << "Erreur " << *m_erreur << endl;
	}

	*m_sock = socket(AF_INET, SOCK_STREAM, 0);
	m_sin.sin_addr.s_addr = inet_addr(m_ipServeur[0].c_str());
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(*m_port);

	*m_sockMusique = socket(AF_INET, SOCK_STREAM, 0);
	m_sinMusique.sin_addr.s_addr = inet_addr(m_ipServeur[0].c_str());
	m_sinMusique.sin_family = AF_INET;
	m_sinMusique.sin_port = htons(*m_portMusique);
}

Client::~Client()
{

	cout << "Fermeture du programme" << endl;

	closesocket(*m_sock);
	closesocket(*m_sockMusique);
	WSACleanup();

	delete m_sock;
	delete m_sockMusique;
	delete m_port;
	delete m_portMusique;
	delete m_ipServeur;
	delete m_message;
	delete m_pseudoServeur;
	delete m_buffer;
	delete m_bufferMusique;
	delete m_resultat;
	delete m_erreur;
}

int Client::connexionAuServeur()
{
	
	if (*m_sock == INVALID_SOCKET)
	{
		cout << "Erreur de la fonction socket dans la methode 'connectToServer' de la classe client";
		*m_erreur = WSAGetLastError();
		cout << "Erreur " << *m_erreur << endl;
        
		return *m_erreur;
	}
    cout << endl << "Connexion au serveur " << *m_ipServeur << " sur le port " << *m_port << endl;
	if (connect(*m_sock, (SOCKADDR *)&m_sin, sizeof(m_sin)) == 0)
	{
		recv(*m_sock, m_pseudoServeur, 30, 0);
	    cout << "Connexion etablie avec " << *m_pseudoServeur << endl;

		return 0;
	}
	else
	{
		system("color 4");
		cout << "Impossiblde de se connecter au port " << *m_port << " du serveur " << *m_ipServeur << endl;
		*m_erreur = WSAGetLastError();
		cout << "Erreur " << *m_erreur << endl;

		return *m_erreur;
	}	
}

int Client::envoieMessage()
{
	thread (&Client::recevoirMessage, this).detach();

	if (*m_erreur == 0)//On verifie que le socket n'a pas rencontre d'erreur avant cela
	{
		while (commandeEnvoyee() != QUITTER )//On verifie que l'utilisateur n'ai pas saisie '/quit'
		{
			getline(cin, *m_message);
			if (send(*m_sock, m_message[0].c_str(), sizeof(*m_message), 0) != sizeof(*m_message))
			{
				cout << "Impossible d'envoyer le message a " << m_ipServeur << " ! Erreur: " << WSAGetLastError() << endl;
			}
			cout << *m_pseudo << ">";
		}

		return QUITTER;
	}
	else
	  return 1;
}

void Client::recevoirMessage()
{
	if (*m_erreur == 0)//On verifie que le socket n'a pas rencontre d'erreur avant cela
	{
		while (commandeEnvoyee() != QUITTER && commandeRecue() != SERVEUR_OFF)
		{
			*m_resultat = recv(*m_sock, m_buffer, 4096, 0);
			if (*m_resultat > 0)
			{
				cout << endl << *m_pseudoServeur << ">" << *m_buffer << endl;
				cout << *m_pseudo << ">";
			}
		}
	}
}

int Client::recevoirMusique()
{
	for (int i = 0; i < 512; ++i)
		m_buffer[i] = 0;
	cout << "Fonction musique" << endl;
	//Connexion au port diffusant la musique
	if (*m_sockMusique == INVALID_SOCKET)
	{
		cout << "Erreur de la fonction socket dans la methode 'recevoirMusique' de la classe client";
		*m_erreur = WSAGetLastError();
		cout << "Erreur " << *m_erreur << endl;

		return *m_erreur;
	}
	cout << endl << "Connexion au serveur: " << *m_ipServeur << " sur le port " << *m_portMusique << endl;
	if (connect(*m_sockMusique, (SOCKADDR *)&m_sinMusique, sizeof(m_sinMusique)) == 0)
	{
		cout << "Connexion etablie " << endl;
	}
	else
	{
		system("color 4");
		cout << "Impossiblde de se connecter au port " << *m_portMusique << " du serveur " << *m_ipServeur << endl;
		*m_erreur = WSAGetLastError();
		cout << "Erreur " << *m_erreur << endl;

		return *m_erreur;
	}

	//Recuperation de la taille du fichier
	long size;
	stringstream convertion;
	*m_resultat = recv(*m_sockMusique, m_bufferMusique, NOMBRE_OCTET, 0);
	convertion << m_bufferMusique;
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
		for (int i = 0; i < size; i += NOMBRE_OCTET)
		{
			i++;
			*m_resultat = recv(*m_sockMusique, m_bufferMusique, NOMBRE_OCTET, 0);
			fichierEcriture.write(m_bufferMusique, NOMBRE_OCTET);
		}
		cout << "Fin de la reception" << endl;
		for (int i = 0; i < 512; ++i)
			m_bufferMusique[i] = 0;
		closesocket(*m_sockMusique);
		return 0;
	}
}
	
int Client::commandeEnvoyee()
{
	if (*m_message == "/quit")
		return QUITTER;
	else if (*m_message == "/reboot")
	{
		reconnexion();
		return REDEMARER;
	}
	else if (*m_message == "/save")
	{
		sauvegardeParametre();
		return SAVE;
	}
	else if (*m_message == "/liste")
		return LISTE;
	else if (*m_message == "/connect")
	{
		connexionAuServeur();
		return CONNEXION;
	}
	else
		return NO_COMMANDE;
}

int Client::commandeRecue()
{
	if (m_buffer[0] != '/')
		return NO_COMMANDE;
	else if (m_buffer[1] == 'm' && m_buffer[2] == 'u' && m_buffer[3] == 's' && m_buffer[4] == 'i' && m_buffer[5] == 'c')
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
		fichierSauvegarde << *m_pseudo << endl;
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
		fichierSauvegarde >> *m_port;
		//Lecture du pseudo
		fichierSauvegarde >> *m_pseudo;
		return 0;
	}
}

int Client::deconnexion()
{
	closesocket(*m_sock);
	return 0;
}

int Client::reconnexion()
{
	cout << "Deconnexion du serveur" << endl;
	deconnexion();
	cout << "Tentative de reconnexion au serveur" << endl;
	connexionAuServeur();
	return 0;
}

SOCKET Client::getSocket() const
{
	return* m_sock;
}

string Client::getPseudo() const
{
	return *m_pseudo;
}

string Client::getIpServeur() const
{
	return *m_ipServeur;
}

int Client::getErreur() const
{
	return *m_erreur;
}
