#include "ClientGame.h"

ClientGame::ClientGame(void) {
    network = new ClientNetwork();

    // Send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendActionPackets() {

    // Send action packets to the server
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

// The update() function which will continuously 
// receive action packets from the server and 
// send action packets in repsonse

void ClientGame::update() {

    Packet packet;
    int data_length = network->receivePackets(network_data);

    if(data_length <= 0) {
        // no data received
        return;
    }

    int i = 0;
    while(i < (unsigned int)data_length) {
        packet.deserialize( &(network_data[i]) );
        i += sizeof(Packet);

        switch(packet.packet_type) {
            case ACTION_EVENT:
                printf("client received action event packet from the server\n");
                sendActionPackets();
                break;
            
            default:
                printf("(ClientGame) error in packet types\n");
                break;
        }
    }
}