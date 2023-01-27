#include <stdio.h>

#include "server/include/ServerNetwork.h"

ServerNetwork::ServerNetwork(void) {
    // Create WSADATA object
    WSADATA wsaData;

    // Sockets for the server
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    // Address info for the server to listen
    struct addrinfo * result = NULL;
    struct addrinfo hints;

    // Init winsock
    iResult = WSAStartup(MAKEWORD(2,2 ), &wsaData);

    if(iResult != 0) {
        printf("Winsock failed with error %d\n", iResult);
        exit(1);
    }

    // Set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the address and port
    // Server won't need an address since it'll be running
    // on the local machine
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if(iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1); 
    }

    // Define socket for connecting to the server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("listen socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the socket mode to non-blocking
    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // No longer need the address info
    freeaddrinfo(result);

    // Start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
}

ServerNetwork::~ServerNetwork(void) {
    closesocket(this->ClientSocket);
    this->ClientSocket = INVALID_SOCKET;

    // Close session sockets 
    for (auto& i : this->sessions) {
        if (i.second != INVALID_SOCKET) {
            closesocket(i.second);
            i.second = INVALID_SOCKET;
        }
    }

    closesocket(this->ListenSocket);
    this->ListenSocket = INVALID_SOCKET;
};

// Accept new connections
bool ServerNetwork::acceptNewClient(unsigned int &id) {
    // If client is waiting accept the connection and save the socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    
    if(ClientSocket != INVALID_SOCKET) {

        // NAGLE -----
        // Insert new client into session id table
        sessions.insert( std::pair<unsigned int, SOCKET>(id, ClientSocket) );
        return true;
    }
    return false;
}

// Receive incoming data
// This function will receive data waiting on the socket for 
// a given client ID and fill the passed buffer (recvbuf) with 
// the data read from the network.
int ServerNetwork::receiveData(unsigned int client_id, char * recvbuf) {
    if( sessions.find(client_id) != sessions.end() ) {
        SOCKET currentSocket = sessions[client_id];
        iResult = NetworkServices::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);
        
        if(iResult == 0) {
            printf("Connection closed\n");
            closesocket(currentSocket);
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
    SOCKET currentSocket;
    int iSendResult;

    for(auto& iter : this->sessions) {
        currentSocket = iter.second;
        iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalsize);
        
        if(iSendResult == SOCKET_ERROR) {
            printf("(sendToAll) send failed with error: %d\n", WSAGetLastError());
            closesocket(currentSocket);
        }
    }
}
