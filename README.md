C++ socket
===================

This is a program make to learn C++ socket.

----------

<i class="icon-refresh"></i>**Client**
------------

The initialization is very easy. You just have to create a *Client* object, call the connection method, and finaly lauch the function for start the listening of the standard input .

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

<i class="icon-sitemap"></i>**Server**
------------

The server isn't aivailable on the repository, but it's in work.
