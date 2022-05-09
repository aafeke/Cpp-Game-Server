/*
We will create a ServerGame class. In there we will declare and 
initialize a ServerNetwork object which will be used as our 
network. In the larger scope of the game, ServerGame should 
hold every information about the game on the server, including 
the information about the clients on the network.
*/

#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include "ServerNetwork.h"
#include "NetworkData.h"

class ServerGame {

    public:
        ServerGame(void);
        ~ServerGame(void);

        void update();
        void receiveFromClients();
        void sendActionPackets();
    
    private:
        // IDs for the clients connecting for the table (map) in ServerNetwork
        // We will increment it for every client, therefore it's static
        // to achieve singularity.
        static unsigned int client_id;

        // The ServerNetwork object
        ServerNetwork * network;

        // Data buffer
        char network_data[MAX_PACKET_SIZE];
};

#endif
