#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H

#include <map>

#include "NetworkData.h"
#include "NetworkServices.h"

class ServerNetwork {

    public:
        ServerNetwork(void);
        ~ServerNetwork(void);

        // Socket to listen connections
        SOCKET ListenSocket;

        // Socket to give to the clients
        SOCKET ClientSocket;

        // For error checking return values
        int iResult;

        // Table to keep track of each client's socket
        std::map<unsigned int, SOCKET> sessions;

        // Accept new connections
        bool acceptNewClient(unsigned int &id);

        // Receive incoming data
        ssize_t receiveData(unsigned int client_id, char * recvbuff);

        // Send data to all clients
        void sendToAll(char * packets, int totalsize);
};

#endif