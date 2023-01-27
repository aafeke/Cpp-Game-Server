#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <map>

#include "NetworkServices.h"
#include "NetworkData.h"

// This is for MSVC
// #pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "6881"

#ifndef DEFAULT_BUFLEN
#define DEFAULT_BUFLEN 512
#endif

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
        int receiveData(unsigned int client_id, char * recvbuff);

        // Send data to all clients
        void sendToAll(char * packets, int totalsize);
};

#endif