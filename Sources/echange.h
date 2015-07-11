#ifndef ECHANGE_H
#define ECHANGE_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
/*!
 * \file echange.h
 * \brief Client pour se connecter a un serveur TCP/IP
 * \author Romain
 * \version 0.1
 * \date 3 juillet 2015
 */

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>

//Valeur par defaut pour la connexion au serveur
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 5567
#define DEFAULT_PORT_MUSIQUE 5568
#define DEFAULT_PSEUDO "client"

/*! \enum Commandes
 *\brief Valeur des commandes
 */
enum Commandes
{
	NO_COMMANDE = 0, /*!< Le message n'est pas une commande (0) */
	QUITTER = 101, /*!< Le client a saisit \a /quit (101) */
	REDEMARER = 102, /*!< Le client a saisit a /restart (102) */
	PSEUDO = 103, /*!< Le client a saisit  \a /pseudo + \c NouveauPseudo  (103) */
	SAVE = 104, /*<! Le client a saisit /a /save (404) */
	LISTE = 104, /*!< Le client a saisit \a /liste (104)*/
	MUSIQUE = 105, /*!< Le client a saisit \a /music (105)*/
    SERVEUR_OFF = 901,/*!< Le serveur a envoye \a /off  (901)*/
	SERVEUR_KICK = 902, /*!< Le serveur a envoye \a /kick (902)*/
	SERVEUR_MUSIQUE = 903 /*!< Le serveur a envoye \a /music (903)*/
};

/*! \class Client
*
*La classe gerant les connexions et les echanges de donnees avec un serveur
*/

class Client
{
public:
	 /*!
     * \brief Constructeur
	 *
	 *Constructeur de la classe client
	 *
	 *\param port : port du serveur distant
	 *\param ip : Adresse IP du serveur distant
	 *\param pseudo : pPseudo du client 
	 */
	Client(u_short port, std::string ip, std::string pseudo);
	/*!
	* \brief Constructeur par defaut de la classe CLient
	*
	*Constructeur qui attribut les valeur par defaut au client\n
	*On a donc
	*\code
	m_erreur = 0;
	m_pseudo = DEFAULT_PSEUDO //client
	m_port = DEFAULT_PORT //5567
	m_portMusique = DEFAULT_PORT_MUSIQUE //5568
	m_ipServeur = DEFAULT_IP //127.0.0.1
	*\endcode
	*
	*\param port : port du serveur distant
	*\param ip : Adresse IP du serveur distant
	*\param pseudo : pPseudo du client
	*/
	Client();
	/*!
	* \brief Connexion au serveur
	*
	*Methode qui permet de connecter le client au serveur\n
	*Exemple
	*\code
	#include <winsock2.h>

	int main()
	{
	 Client c;
	 c.connexionServeur();
	 return 0;
	}
	*\endcode
	*
	*\param port Port auquel doit se connecter le client
	*\param ip Ip auquel doit se connecter le client
	*\param pseudo Pseudo sous lequel est identifie le client
	*
	*\return 0 si la connexion s'est bien etablie,
	*le code de l'erreur sinon
	*/
	
	int connexionAuServeur();
	/*!
	* \brief Recuperation du socket permettant la connexion
	*
	*Methode qui permet de recuperer le socket du client
	*
	*\return le socket client
	*/
	SOCKET getSocket() const;
	/*!
	* \brief Recuperation du pseudo du client
	*
	*Methode qui permet de recupperer le pseudo du client
	*
	*\return le pseudo du client
	*/
	std::string getPseudo() const;
	/*!
	* \brief Recuperation de l'adresse IP du serveur
	*
	*Methode qui permet de recupperer l'adresse IP du serveur auquel est connecte le client
	*
	*\return le pseudo du client
	*/
	std::string getIpServeur() const;
	/*!
	* \brief Recuperation de la derniere erreur rencontree
	*
	*Methode qui permet de recupperer la valeur de la derniere erreur rencontree
	*
	*
	*\return  Renvoie la variable contenant la valeur de la derniere erreur
	*/
	int getErreur() const;
	/*!
	* \brief Verification d'une commande
	*
	*Methode qui permet de verifier si un message est une commande
	*
	*\return  Cette methode renvoie les valeur de l'enumeration \c Commandes
	*/
	int commande();
	/*!
	* \brief Verification d'une commande venant du serveur
	*
	*Methode qui permet de verifier si un message venant du serveur est une commande
	*
	*\return  Cette methode renvoie les valeur de l'enumeration \c Commandes
	*/
	int commandeServeur();
	/*!
	* \brief Envoie des messages client
	*
	*\param message Message a envoyer au serveur
	*
	*Boucle permettant la saisie du message par l'utilisateur et son envoie tant que l'utilisateur de rentre pas \a quit
	*Cette fonction demande egalement a la methode client::commande() si le message n'est pas une commande
	*\return  Renvoie 1 lorsque l'utilisateur saisie '/quit'
	*
	*/
	int envoieMessage();
	/*!
	* \brief Reception des messages serveur
	*
	*Boucle permettant la reception de message de la part du serveur
	*De plus, cette methode lance un second thread, dans lequel est lancee la methode client::recevoirMessage(), evitant ainsi le bloquage du a la fonction \c recv()
	*
	*/
	void recevoirMessage();
	/*!
	* \brief Sauvegarde des informations de connexions
	*
	*Methode qui permet de sauvegarder les options de connexion dans un fichier .tsc
	*
	*\return Renvoie 0 si tout se passe correctement\n
	*Sinon renvoie 1 si le fichier n'a pas pu etre ouvert
	*/
	int sauvegardeParametre();
	/*!
	* \brief Sauvegarde des informations de connexions
	*
	*Methode qui permet de charger les parametres de connexions a partir d'un fichier .tsc
	*
	*\param fichier Chemin du fichier.tsc dans lequel sont stockees les valeurs sauvegardees
	*\return Renvoie 0 si tout se passe correctement\n
	*Sinon renvoie 1 si le fichier n'a pas pu etre ouvert
	*/
	int chargerParametre();
	/*!
	* \brief Reception d'une musique
	*
	*Methode qui permet la reception d'un fichier au format mp3
	*
	*\return Renvoie 0 si tout se passe correctement\n
	*Sinon renvoie 1 si le fichier n'a pas pu etre ouvert 
	*ou la valeur de WSAGetLastError() si l'erreur est du au reseau 
	*/
	int recevoirMusique();
	/*!
	* \brief Destructeur de la classe client
	*
	*Destructeur de la classe client
	*
	*/
	~Client();

private:
	SOCKET m_sock; /*!< Socket de connexion*/
	SOCKET m_sockMusique; /*!< Socket permettant la reception de musique*/
	SOCKADDR_IN m_sin;
	SOCKADDR_IN m_sinMusique;
	u_short m_port;/*!< Port du serveur */
	u_short m_portMusique;/*!< Port du serveur pour la reception de musique*/
	std::string m_ipServeur;/*!< Adresse IP du serveur*/
	std::string m_pseudo;/*!< Pseudo du client*/
	std::string m_message;/*!< Message envoye par le client*/
	char *m_pseudoServeur;/*!< Nom envoye par le serveur*/
	char *m_buffer;/*!< Buffer utiliser pour la reception des messages*/
	char *m_bufferMusique;/*!< Buffer utiliser pour la reception de fichier musique*/
	int *m_resultat;/*!< Variable utilisee pour obtenir le resultat des fonction send(), recv(), ou connect()*/
	int *m_erreur; /*!< Variable renvoyant le code de la derniere erreur*/
};

#endif   // ECHANGE_H