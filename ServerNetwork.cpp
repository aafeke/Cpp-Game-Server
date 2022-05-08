#include "ServerNetwork.h"

ServerNetwork::ServerNetwork(void) {
    // Create WSADATA object
    WSADATA wsaData;

    // Sockets for the server
    ListenSocket = NULL;
    ClientSocket = NULL;

    // Address info for the server to listen
    struct addrinfo * result = NULL;
    struct addrinfo hints;

    // Init winsock
    iResult = WSAStartup(MAKEWORD(2,2 ), &wsaData);

    if(iResult != 0) {
        printf("Winsock failed with error %d\n", iResult);
        exit(1);
    }

    // Set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the address and port
    // Server won't need an address since it'll be running
    // on the local machine
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if(iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1); 
    }

    // Define socket for connecting to the server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_socktype);

    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the socket mode to non-blocking
    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // No longer need the address info
    freeaddrinfo(result);

    // Start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
}

// Accept new connections
bool ServerNetwork::acceptNewClient(unsigned int &id) {
    // If client is waiting accept the connection and save the socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    
    if(ClientSocket != INVALID_SOCKET) {

        // NAGLE -----
        // Insert new client into session id table
        sessions.insert( std::pair<unsigned int, SOCKET>(id, ClientSocket) );
        return true;
    }
    return false;
}