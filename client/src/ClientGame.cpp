#include <string>

#include "client/include/ClientGame.h"

ClientGame::ClientGame(std::string&& address) {
    network = new ClientNetwork(std::forward<std::string>(address));

    // Send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

ClientGame::~ClientGame(void) {
    delete this->network;
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
    ssize_t data_length = network->receivePackets(network_data);

    if(data_length <= 0) {
        // no data received
        return;
    }

    ssize_t i = 0;
    while(i < data_length) {
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