#include "ServerGame.h"
#include <stdio.h>

unsigned int ServerGame::client_id;

ServerGame::ServerGame(void) {
    // IDs to assign clients for our table
    client_id = 0;

    // Set up the server network to listen
    network = new ServerNetwork();
}

void ServerGame::update() {

    // Get new clients
    if( network->acceptNewClient(client_id) ) {
        printf("Client %d has been connected to the server\n", client_id);

        client_id++;
    }
}