#include "ServerGame.h"
#include "ClientGame.h"

// For multi-threading
#include <process.h>

// Global variables/functions

void serverLoop(void *);
void clientLoop(void);

ServerGame * server;
ClientGame * client;

int main() {
    
    // Initialize the server.
    // We will run the server on a separate thread, so that 
    // it will always be checking for new clients. It will 
    // also be sending and receiving data.
    server = new ServerGame();

    // Create thread with arbitrary arguments for the run function
    _beginthread( serverLoop, 0, (void*)12);

    // Initialize the client
    client = new ClientGame();

    // This is our game loop. It will hold the console so that 
    // we can see the output from ServerNetwork and it can be 
    // extended to do other things.
    clientLoop();
}

void serverLoop(void * arg) {
    while(true) {
        server->update();
    }
}

void clientLoop(void) {
    while(true) {
        // Do game stuff.
        // Will later call client->update();
    }
}
