#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#include <winsock2.h>
#include <windows.h>
#include "NetworkServices.h"
#include <ws2tcpip.h>
#include <stdio.h>

// Size of the buffer
#ifndef DEFAULT_BUFLEN
#define DEFAULT_BUFLEN 512
#endif

// Port to connect sockets
#define DEFAULT_PORT "6881"

// Needs to be linked with 'Ws2_32.lib', 'Mswsock.lib' and 'Advapi32.lib'
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "Advapi32.lib")

class ClientNetwork {
    public:
        // For error checking function calls in Winsock library
        int iResult;

        // Socket for client to connect server
        SOCKET ConnectSocket;

        // Constructor / Destructor
        ClientNetwork(void);
        ~ClientNetwork(void);
};

#endif