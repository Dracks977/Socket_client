This project is a test for learning use github

# Socket_client

Client se connectant a un serveur TCP/IP, qui est un autre projet
C'est en fait une classe client qui facilite la connexion a un serveur distant.

##Qu'est-ce qui est deja fini

On peut se connecter a un serveur distant, envoyer un message ainsi qu'en recevoir, et recevoir une musique

##Comment l'utiliser

Pour l'utiliser, il suffit d'ajouter echange.cpp et echange.h au projet, et d'inclure "echange.h" pour acceder aux fonctions

##Client

  Le programme permet pour le moment de se connecter a un serveur en quelques lignes


```C++
    #include "echange.h"
    int main(int argc, char* argv[])
    {
        Client c;
        c.connexion au serveur();
        c.envoyerMessage();
      
        return 0;
    }
```    
