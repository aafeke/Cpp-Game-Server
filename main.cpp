#include <memory>
// For multi-threading
#include <thread>

#include "ServerGame.h"
#include "ClientGame.h"

// Global variables/functions

void serverLoop(void);
void clientLoop(void);

std::unique_ptr<ServerGame> server;
std::unique_ptr<ClientGame> client;

int main() {
    // Initialize the server.
    // We will run the server on a separate thread, so that 
    // it will always be checking for new clients. It will 
    // also be sending and receiving data.
    // server = new ServerGame();
    server = std::make_unique<ServerGame>();

    // Initialize the client
    // client = new ClientGame();
    client = std::make_unique<ClientGame>();

    // Create thread with arbitrary arguments for the run function
    // _beginthread( serverLoop, 0, (void*)12);

    std::thread servThread(serverLoop);
    std::thread clientThread(clientLoop);

    servThread.join();

    // This is our game loop. It will hold the console so that 
    // we can see the output from ServerNetwork and it can be 
    // extended to do other things.
    // clientLoop();
    clientThread.join();
}

void serverLoop(void) {
    while(true) {
        server->update();
    }
}

void clientLoop(void) {
    while(true) {
        // Do game stuff.
        // Will later call client->update();
        client->update();
    }
}
