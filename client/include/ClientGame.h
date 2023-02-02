#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <string>

#include "ClientNetwork.h"
#include "NetworkData.h"

class ClientGame {

    public:
        ClientGame(std::string&& address);
        ~ClientGame(void);
        
        void sendActionPackets();

        char network_data[MAX_PACKET_SIZE];

        void update();

        ClientNetwork * network;
};

#endif