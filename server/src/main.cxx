#include <memory>
#include <thread>

#include "server/include/ServerGame.h"

void server_loop(std::unique_ptr<ServerGame> server_ptr) {
    while (true) {
        server_ptr->update();
    }
}

int main(void) {
    auto server_ptr = std::make_unique<ServerGame>();
    server_loop(std::move(server_ptr));
    return 0;
}