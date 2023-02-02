#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#include <string>

#include "NetworkData.h"
#include "NetworkServices.h"

// Needs to be linked with 'Ws2_32.lib', 'Mswsock.lib' and 'Advapi32.lib'
// This is for MSVC
// #pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "Advapi32.lib")

class ClientNetwork {
    public:
        // For error checking function calls in Winsock library
        // int iResult;

        // Socket for client to connect server
        SOCKET ConnectSocket;

        // Constructor / Destructor
        ClientNetwork(std::string&& address);
        ~ClientNetwork(void);

        // Receive action packets
        ssize_t receivePackets(char *);
};

#endif