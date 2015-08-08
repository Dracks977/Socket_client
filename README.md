This project is a test for learn github

# Socket_client

Client se connectant a un serveur TCP/IP, qui est un autre projet
C'est en fait une classe client qui facilite la connexion a un serveur distant.
C'est ici la base d'un projet qui sera a but musical. En effet, de dernier pourra, lorsqu'un client ecoute une musique, transmettre le flux audio a un autre client, afin que les deux personnes ecoutent la meme musique.

##What is ready ?

Actually, people can connect to a server, chat and receive musique from server.
Some command can already be use, but a lot of other will be soon arrived !


##Comment l'utiliser

Actually, for include the sources to your project, you have to include "client.h" in your app, and add "client.cpp" to your project.

##Client

  In main function, you can for exemple have


```C++
    #include "client.h"
    int main(int argc, char* argv[])
    {
        Client c;
        c.connexionAuServeur();
        c.envoyerMessage();
        return 0;
    }
```    
