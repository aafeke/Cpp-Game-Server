#include <memory>
#include <iostream>

#include "client/include/ClientGame.h"

void client_loop(std::unique_ptr<ClientGame> client_ptr) {
    while (true) {
        client_ptr->update();
    }
}

int main(int argc, char **argv) {

    std::cout << "argc: " << argc << '\n';

    if (argc != 2) {
        std::cout << "usage: '.main/ <server address>'\n";
        return -1;
    }

    std::cout << "argv[1]: " << argv[1] << '\n';

    auto client_ptr = std::make_unique<ClientGame>(argv[1]);
    client_loop(std::move(client_ptr));
    return 0;
}
