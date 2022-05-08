#ifndef NETWORK_SERVICES_H
#define NETWORK_SERVICES_H

#pragma once
#include <winsock2.h>
#include <windows.h>

class NetworkServices {
    public:
        static int sendMessage(SOCKET curSocket, char * message, int messageSize);
        static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
};

#endif