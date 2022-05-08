#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H

#include <winsock2.h>
#include <windows.h>
#include "NetworkServices.h"
#include <ws2tcpip.h>
#include <map>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN = 512
#define DEFAULT_PORT "6881"

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
};

#endif