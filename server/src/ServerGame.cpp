#include <stdio.h>

#include "server/include/ServerGame.h"

unsigned int ServerGame::client_id;

ServerGame::ServerGame(void) {
    // IDs to assign clients for our table
    client_id = 0;

    // Set up the server network to listen
    network = new ServerNetwork();
}

ServerGame::~ServerGame(void) {
    delete this->network;
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
    for (auto& iter : this->network->sessions) {    
        // Get the data for that client
        int data_length = network->receiveData(iter.first, network_data);

        if(data_length <= 0) {
            // No data received
            continue;
        }

        int i = 0;
        while(i < data_length) {
            packet.deserialize( &(network_data[i]) );
            i += sizeof(Packet);

            switch(packet.packet_type) {
                case INIT_CONNECTION:
                    printf("server received init packet from client %d\n", iter.first);
                    sendActionPackets();
                    break;
                case ACTION_EVENT:
                    printf("server received action event packet from client %d\n", iter.first);
                    sendActionPackets();
                    break;
                default:
                    printf("(ServerGame) error in packet types\n");
            }
        }
    }
}

// Send action packet
void ServerGame::sendActionPackets() {

    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
}
