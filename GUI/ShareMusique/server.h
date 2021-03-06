#ifndef SERVER_H
#define SERVER_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>
#include <list>
#include <utility>
#include <stdlib.h>


#define NOMBRE_OCTET 2048

typedef struct connectedClient{
    int ID;
    char* pseudo;
    SOCKET socket;

}connectedClient;
struct Serverdata;

void* threadServerAcceptClient(void* p_data);
void* threadServerSendMessage(void* p_data);
void* threadServerReceiveMessage(void* p_data);

/*! \class Server
*
*La classe gerant les connexions et les echanges de donnees avec les clients
*/
class Server
{
public:
    /*!
    * \brief Constructeur de la classe
    *
    *
    *
    */
    Server();
    Server(std::string pseudo, u_short port, u_short portMusic);
    int start();
    /*!
    * \brief Methode permettant de recevoir les messages.
    *
    *Methode sous forme de boucle qui verifie qu'un client n'a pas envoye de message, et permet donc la reception de celui-ci en cas d'envoie de
    *la part d'un client
    *
    */
    void receiveMessage();
    /*!
    * \brief Methode permettant d'accepter des client
    *
    *Methode sous forme de boucle qui verifie constament que de nouveaux client ne tentent pas de se connecter
    *
    */
    int acceptClient();
    /*!
    * \brief Methode pour envoyer de la musique
    *
    *Methode permettant l'envoie d'une musique
    *
    *
    */
    int sendMusic();
    /*!
    * \brief Methode pour envoyer de la musique
    *
    *Methode permettant l'envoie d'une musique
    *
    *
    */
    void sendMessage();
    /*!
    * \brief Methode pour changer le port
    *
    *Methode permettant de changer le port utiliser pour envoyer et recevoir des messages
    *
    *\param port Nouveau port.
    *
    */
    void setPort(u_short port);
    /*!
    * \brief Methode pour changer dle port musique
    *
    *Methode permettant de changer le port utiliser pour transfere la musique
    *
    *\param port Nouveau port.
    *
    */
    void setMusicPort(u_short port);
    /*!
    * \brief Methode pour changer le pseudo
    *
    *Methode permettant de changer le pseudo du serveur
    *
    *\param pseudo Nouveau Pseudo qui va remplacer l'ancien.
    *
    */
    void setPseudo(std::string pseudo);
    int getError();
    u_short getPort();
    u_short getPortMusic();
    char* getBufer();


private:
    SOCKET *m_sockServer;/*!< Socket principale du serveur*/
    SOCKET *m_sockMusic;/*!< Socket permettant d'envoyer de la musique*/
    SOCKADDR_IN m_sin;
    SOCKADDR_IN m_sinMusic;
    SOCKADDR_IN m_cSin;
    connectedClient m_lastClient;/*!< Donnees du dernier client s'etant connecte*/
    std::string *m_pseudo;/*!< Pseudo de l'utilisateur du serveur*/
    std::string *m_message;/*!< Message saisi et envoye par le serveur*/
    u_short *m_port;/*!< Port  principal qu'ecoute le serveur*/
    u_short *m_portMusic;/*!< Port permettant d'envoyer de la musique*/
    char *m_buffer;/*!< Buffer contenant les messages recus*/
    char *m_bufferMusic;/*!< Buffer contenant les donnees du ficier musique a envoyer*/
    int *m_error;/*!< Variable contenant la derniere erreur rencontree*/
    int *m_resultat;/*!< Variable contenant le resultat de la derniere action*/
    std::list<connectedClient> listeClient;/*!< List des client conectes*/
};

typedef struct Serverdata{
    Server server;
    pthread_mutex_t mutex;

}Serverdata;




#endif
