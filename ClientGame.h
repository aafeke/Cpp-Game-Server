#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <winsock2.h>
#include <windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

class ClientGame {

    public:
        ClientGame();
        ~ClientGame(void);
        
        void sendActionPackets();

        char network_data[MAX_PACKET_SIZE];

        void update();

        ClientNetwork * network;
};

#endif