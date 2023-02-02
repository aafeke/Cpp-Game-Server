#include <string>
#include <iostream>

#include "client/include/ClientNetwork.h"

// Implement the declared constructor in 'ClientNetwork.h'
ClientNetwork::ClientNetwork(std::string&& address) {
    // Socket (Initially invalid)
    this->ConnectSocket = INVALID_SOCKET;
 
    this->ConnectSocket = NetworkServices::sockets_create(std::forward<std::string>(address), false);

    // If connection failed
    if(ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to the server\n");
        exit(1);
    }
}

ClientNetwork::~ClientNetwork(void) {
    NetworkServices::sockets_close(this->ConnectSocket);
}

ssize_t ClientNetwork::receivePackets(char * recvbuf) {
    
    ssize_t iResult = NetworkServices::receiveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE);

    if (iResult == 0) {
        std::cerr << "(ClientNetwork::receivePackets) Connection closed\n";
        NetworkServices::sockets_close(ConnectSocket);
        exit(1);
    }
    
    return iResult;
}