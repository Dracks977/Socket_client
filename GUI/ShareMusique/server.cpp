
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "server.h"

#define NOMBRE_OCTET 2048
using namespace std;

Server::Server() : m_sockServer(0), m_sockMusic(0), m_pseudo(0), m_port(0), m_portMusic(0), m_buffer(0), m_bufferMusic(0), m_error(0)
{
    m_sockServer = new SOCKET;
    m_sockMusic = new SOCKET;
    m_pseudo = new string;
    m_message = new string;
    m_port = new u_short;
    m_portMusic = new u_short;
    m_buffer = new char[NOMBRE_OCTET];
    m_bufferMusic = new char[NOMBRE_OCTET];
    m_error = new int;
    m_resultat = new int;
}

Server::Server(std::string pseudo, u_short port, u_short portMusic) : m_sockServer(0), m_sockMusic(0), m_pseudo(0), m_port(0), m_portMusic(0), m_buffer(0), m_bufferMusic(0), m_error(0)
{
    m_sockServer = new SOCKET;
    m_sockMusic = new SOCKET;
    m_pseudo = new string;
    m_message = new string;
    m_port = new u_short;
    m_portMusic = new u_short;
    m_buffer = new char[NOMBRE_OCTET];
    m_bufferMusic = new char[NOMBRE_OCTET];
    m_error = new int;
    m_resultat = new int;

    *m_pseudo = pseudo;
    *m_port = port;
    *m_portMusic = portMusic;
}

int Server::start()
{
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0) //If there is an error during WSAStartup
    {
        cout << "Erreur de la fonction WSAStarup dans le constructeur de la classe client" << endl;
        *m_error = WSAGetLastError();
        cout << "Erreur " << *m_error << endl;
        return *m_error;
    }
    else//Else, if there is any error
    {
        *m_sockServer = socket(AF_INET, SOCK_STREAM, 0);
        m_sin.sin_addr.s_addr = INADDR_ANY;
        m_sin.sin_family = AF_INET;
        m_sin.sin_port = htons(*m_port);
        bind(*m_sockServer, (SOCKADDR *)&m_sin, sizeof(m_sin));
        listen(*m_sockServer, 6);//The server is listening
        cout << "Ecoute du port: " << *m_port << endl;
        return 0;
    }
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

void Server::receiveMessage()
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
                if (*m_resultat == -1)//If there is an error during the select
                {
                    *m_error = WSAGetLastError();
                    cout << "Erreur lors du select" << endl;
                }
                else if (FD_ISSET(it->socket, &st)) //Else if there is any error and a message is received
                {
                    if (recv(it->socket, m_buffer, NOMBRE_OCTET, 0) > 0) //If there is any error during recv
                    {
                        cout << '\r' << it->pseudo << ">" << m_buffer << endl;
                        cout << *m_pseudo << ">";
                    }
                    else//Else if there is an error
                    {
                        *m_error = WSAGetLastError();
                        cout << "Erreur lors de la reception du message provenant de " << it->pseudo << endl;
                    }
                }
            }
        }
}

void Server::sendMessage()
{
    while (*m_message != "/off")
    {
        getline(cin, *m_message);
        for (list<connectedClient>::iterator it = listeClient.begin(); it != listeClient.end(); it++)
        {
            if (send(it->socket, m_message->c_str(), NOMBRE_OCTET, 0) != NOMBRE_OCTET)
            {
                cout << "Impossible d'envoyer le message a " << it->pseudo << " ! Erreur: " << *m_error << endl;
                *m_error = WSAGetLastError();
            }
        }

        cout << *m_pseudo << ">";
        if (*m_message == "/music")
            sendMusic();
    }
}


int Server::acceptClient()
{
    cout << "Le serveur nomme " << *m_pseudo << " ecoute maintenant les connexions entrantes" << endl;

    m_lastClient.pseudo = new char[30];
    int sinsize = sizeof(m_cSin);
    while (*m_message != "/off")
    {
        m_lastClient.socket = accept(*m_sockServer, (SOCKADDR *)&m_cSin, &sinsize);
        if (m_lastClient.socket != INVALID_SOCKET)
        {
            cout << '\r' << "Nouveau client" << endl;
            recv(m_lastClient.socket, m_lastClient.pseudo, 30, 0);
            send(m_lastClient.socket, m_pseudo->c_str(), 30, 0);
            listeClient.push_back(m_lastClient);
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

int Server::getError()
{
    return *m_error;
}

u_short Server::getPort()
{
    return *m_port;
}

u_short Server::getPortMusic()
{
    return *m_portMusic;
}
char* Server::getBufer()
{
    return m_buffer;
}

void* threadServerAcceptClient(void* p_data)
{
    Serverdata *dataServer = (Serverdata*)p_data;
    dataServer->server.acceptClient();
    return NULL;
}

void* threadServerSendMessage(void* p_data)
{
    Serverdata *dataServer = (Serverdata*)p_data;
    dataServer->server.sendMessage();
    return NULL;
}
void* threadServerReceiveMessage(void* p_data)
{
    Serverdata *dataServer = (Serverdata*)p_data;
    dataServer->server.receiveMessage();
    return NULL;
}
