#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

    INIT_CONNECTION = 0,
    ACTION_EVENT = 1,
};

struct Packet {
    unsigned int packet_type;

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};

// The packet_type field will be filled using the values
// in the enumerator we just created.

// The serialize function is used to convert the packet_type
// data into bytes that we can send over the network.

// The deserialize function is used to convert bytes received 
// over the network back into packet_type data that we can interpret.

// You will need to include NetworkData.h inside your 
// ServerGame, ServerNetwork, ClientGame, and ClientNetwork classes!

#endif