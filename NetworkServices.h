#ifndef NETWORK_SERVICES_H
#define NETWORK_SERVICES_H
// #define _WIN32_WINNT 0x501 // For some reason, MinGW won't compile 
                           // if this is not declared. It must be declared
                           // before including the "windows.h" apparently.

#pragma once
#include <winsock2.h>
#include <windows.h>

class NetworkServices {
    public:
        static int sendMessage(SOCKET curSocket, char * message, int messageSize);
        static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
};

#endif