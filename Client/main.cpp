#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>

using namespace std;
using namespace sf;

void send (TcpSocket& recipient)
{
    sf::Packet data;
    string message;
    cout<<"What's your message?"<<endl;
    cin.ignore();
    getline(cin, message);
    data<<message;
    recipient.send(data);
    cout<<"message sent !"<<endl;
}

sf::Socket::Status receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout)
{
    sf::SocketSelector selector;
    selector.add(socket);
    if (selector.wait(timeout))
        return socket.receive(packet);
    else
        return sf::Socket::NotReady;
}

int main()
{
    sf::TcpSocket servor;
    sf::Packet data;
    string str, sender;
    const IpAddress adress = "127.0.0.1";
    const unsigned short port = 53000;

    cout<<"bienvenu sur ce logiciel de chat!"<<endl<<endl;

    if (servor.connect(adress, port) != Socket::Done)   ///connection attempt
    {
        cout<<"connection error. Impossible to communicate with the servor"<<endl;
    }
    else
    {
        cout<<"You've succesfull connected. Use 'SPACE' to write a message to the other"<<endl;
    }

    do
    {
        cout<<"entrer 'actualise' pour verifier les messages ou autre chose pour en envoyer"<<endl;
        cin>>str;
        if (str != "actualise")
        {
           send(servor);    ///envoyer un message
        }
        while (receiveWithTimeout(servor, data, seconds(0.1f)) == Socket::Done)    ///si on a recu un message
        {
            data>>str;
            data>>sender;   ///recuperer les donnees : expediteur puis message
            cout<<"Vous avez recu un message : '"<<sender<<"' says '"<<str<<"'"<<endl;
        }
    }
    while (true);   ///an infinite loop

    system("PAUSE");
    return 0;
}
