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

    receiveFromClients();
}

void ServerGame::receiveFromClients() {

    Packet packet;

    // Iterate through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter=network->sessions.begin(); iter != network->sessions.end(); iter++) {
        
        // Get the data for that client
        int data_length = network->receiveData(iter->first, network_data);

        if(data_length <= 0) {
            // No data received
            continue;
        }

        int i = 0;
        while(i < (unsigned int)data_length) {
            packet.deserialize( &(network_data[i]) );
            i += sizeof(Packet);

            switch(packet.packet_type) {
                case INIT_CONNECTION:
                    printf("server received init packet from client %d\n", iter->first);
                    break;
                case ACTION_EVENT:
                    printf("server received action event packet from client %d\n", iter->first);
                    break;
                default:
                    printf("error in packet types\n");
            }
        }
    }
}