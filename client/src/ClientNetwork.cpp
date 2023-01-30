#include <string>

#include "client/include/ClientNetwork.h"

// Implement the declared constructor in 'ClientNetwork.h'
ClientNetwork::ClientNetwork(std::string&& address) {
    // Create WSADATA object
    WSADATA wsaData;

    // Socket (Initially invalid)
    this->ConnectSocket = INVALID_SOCKET;
 
    // Struct to hold address info for socket to connect to
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if(iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // Set address info
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve server address and port
    iResult = getaddrinfo(address.c_str(), DEFAULT_PORT, &hints, &result);

    if(iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    // Normally this would be populated with different server addrinfo structs.
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {
        // Define the socket
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if(ConnectSocket == INVALID_SOCKET) {
            printf("connect Socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }
        
        // Connect to the server
        iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);

        if(iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            printf("The server is down\n");
        }
    }

    // No longer need address info
    freeaddrinfo(result);

    // If connection failed
    if(ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to the server\n");
        WSACleanup();
        exit(1);
    }

    // Set the socket non-blocking
    u_long iMode = 1;
    iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);

    if(iResult == SOCKET_ERROR) {
        printf("ioctlsocket failed with error %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }
}

ClientNetwork::~ClientNetwork(void) {
    closesocket(this->ConnectSocket);
};

int ClientNetwork::receivePackets(char * recvbuf) {
    
    iResult = NetworkServices::receiveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE);

    if (iResult == 0) {
        printf("(ClientNetwork::receivePackets) Connection closed\n");
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }
    
    return iResult;
}