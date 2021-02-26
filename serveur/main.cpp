#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>
#include <list>

using namespace std;
using namespace sf;

bool newConnection(SocketSelector& selector, TcpListener& listener, vector <TcpSocket*>& clients)
{
    /**TcpSocket newClient;
    if (listener.accept(newClient) == sf::Socket::Done)
    {
        cout<<"new connection"<<endl;
        selector.add(newClient);
        clients.push_back(&newClient); ///add the new client to the clients list
        return true;
    }
    return false;**/

    TcpSocket* client = new TcpSocket;
    if(listener.accept(*client) == sf::Socket::Done)
    {
        clients.push_back(client);
        selector.add(*client);
        cout<<"new connection"<<endl;
        return true;
    }
    else
    {
        delete client;
        cout<<"connection atempt error"<<endl;
        return false;
    }
}

Packet receive(SocketSelector& selector, vector <TcpSocket*>& sockets)
{
    sf::Packet data;
    IpAddress sender;
    for (unsigned i=0; i<sockets.size(); i++)    /// on a reçu quelque chose sur l'une des sockets mais on sait pas laquelle. il faut toutes les tester
        if (selector.isReady(*sockets[i]))  /// cette socket est prête, on peut recevoir (ou accepter une connexion, si c'est un listener)
        {
            sockets[i]->receive(data);
            sender = sockets[i]->getRemoteAddress();
            string str;
            if (data>>str)
                cout<<"data received ! : '"<<str<<"' from "<<sender<<endl;
            else
                cout<<"impossible to read the data"<<endl;
            data<<sender.toString();
                    }
    return data;
}

void send (Packet& data, vector <TcpSocket*>& sockets)
{
    for (unsigned i=0; i<sockets.size(); i++)
        sockets[i]->send(data);///send the data to all clients
    cout<<"sent data"<<endl;
}

int main()
{
    sf::TcpListener listener;
    vector <TcpSocket*> clients;    ///store pointers on the clients' sockets
    sf::SocketSelector selector;
    sf::Packet data;    /// recieved data
    const unsigned short port = 53000;

    listener.listen(port); /// listen to a port
    selector.add(listener);

    do
    {
        if (selector.wait())///controler si on a recu un message bloque tant qu'aucune des sockets n'est prete
        {
            if(selector.isReady(listener))
            {
                newConnection(selector, listener, clients);
            }
            else
            {
                IpAddress sender;
                data = receive(selector, clients);///receive the message
                send(data, clients);///and send it to all clients
            }
        }
    }
    while(true);    ///infinite loop
    system("PAUSE");

    return 0;
}

//code du tuto officiel
/**int main()
{
    // Create a socket to listen to new connections
    sf::TcpListener listener;
    listener.listen(53000);

    // Create a list to store the future clients
    std::list<sf::TcpSocket*> clients;

    // Create a selector
    sf::SocketSelector selector;

    // Add the listener to the selector
    selector.add(listener);

    // Endless loop that waits for new connections
    while (true)
    {
        // Make the selector wait for data on any socket
        if (selector.wait())
        {
            // Test the listener
            if (selector.isReady(listener))
            {
                // The listener is ready: there is a pending connection
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    // Add the new client to the clients list
                    clients.push_back(client);

                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*client);
                }
                else
                {
                    // Error, we won't get a new connection, delete the socket
                    delete client;
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done)
                        {
                            for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                            {
                                sf::TcpSocket& client = **it;
                                client.send(packet);

                            }

                            string message;
                            packet>>message;
                            cout<<"recieve data : "<<message<<endl;
                        }
                    }
                }
            }
        }
    }
       system("PAUSE");

return 0;
}**/
