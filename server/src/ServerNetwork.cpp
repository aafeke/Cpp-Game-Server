#include <iostream>
#include <string>

#include "NetworkServices.h"
#include "server/include/ServerNetwork.h"

ServerNetwork::ServerNetwork(void) {
    // Sockets for the server
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;
    
    ListenSocket = NetworkServices::sockets_create(std::string("0.0.0.0"), true);

    if (ListenSocket == INVALID_SOCKET) {
        std::cerr << "ServerNetwork ListenSocket init error, terminating.\n";
        exit(1);
    }

    std::cout << "listening for connections at: 0.0.0.0:" << DEFAULT_PORT << '\n';
}

ServerNetwork::~ServerNetwork(void) {
    NetworkServices::sockets_close(this->ClientSocket);
    this->ClientSocket = INVALID_SOCKET;

    NetworkServices::sockets_close(this->ListenSocket);
    this->ListenSocket = INVALID_SOCKET;

    // Close session sockets 
    for (auto& i : this->sessions) {
        if (i.second != INVALID_SOCKET) {
            NetworkServices::sockets_close(i.second);
            i.second = INVALID_SOCKET;
        }
    }
}

// Accept new connections
bool ServerNetwork::acceptNewClient(unsigned int &id) {
    // If client is waiting accept the connection and save the socket
    ClientSocket = NetworkServices::sockets_accept(ListenSocket);
    
    if(ClientSocket != INVALID_SOCKET) {

        // NAGLE -----
        // Insert new client into session id table
        sessions.insert( std::pair<unsigned int, SOCKET>(id, ClientSocket) );

        ClientSocket = INVALID_SOCKET;
        return true;
    }
    return false;
}

// Receive incoming data
// This function will receive data waiting on the socket for 
// a given client ID and fill the passed buffer (recvbuf) with 
// the data read from the network.
ssize_t ServerNetwork::receiveData(unsigned int client_id, char * recvbuf) {
    if( sessions.find(client_id) != sessions.end() ) {
        SOCKET& currentSocket = sessions[client_id];
        ssize_t iResult = NetworkServices::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);
        
        if(iResult == 0) {
            std::cerr << "Connection closed\n";
            NetworkServices::sockets_close(currentSocket);
            currentSocket = INVALID_SOCKET;
        }
        return iResult;
    }
    return 0;
}

// Send data to all clients
// The client will send an INIT packet, then the server will 
// send an ACTION packet and upon receipt, the client will send 
// an ACTION packet, and then the server will receive the 
// ACTION packet, and send another, and etc.
void ServerNetwork::sendToAll(char * packets, int totalsize) {
    ssize_t iSendResult;

    for(auto& iter : this->sessions) {
        SOCKET &currentSocket = iter.second;
        iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalsize);
        
        if(iSendResult == SOCKET_ERROR) {
            std::cerr << "(sendToAll) send failed with error: " << get_last_error << '\n';
            NetworkServices::sockets_close(currentSocket);
            currentSocket = INVALID_SOCKET;
        }
    }
}
