#include <iostream>
#include <string>
#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <cstring> // std::memset()
#endif

#include "Sockets.h"


#ifdef _WIN32
int Sockets_bind(SOCKET s, addrinfo *ai) {
    int iResult = bind(s, ai->ai_addr, (int)ai->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind() failed with error: " << WSAGetLastError()
            << '\n';
        freeaddrinfo(ai);
        closesocket(s);
        WSACleanup();
        exit(1);
    }
    return s;
}

SOCKET Socket::sockets_create(std::string&& address, bool listen) {
    int iResult;
    WSADATA wsaData;
    SOCKET ret;

    ret = INVALID_SOCKET;

    struct addrinfo *result, hints;

    // Initialize Winsock
    if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData))) {
        std::cout << "WSAStartup failed with error: " 
            << iResult << '\n';
        exit(1);
    }

    // Set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (iResult = getaddrinfo(address.c_str(), DEFAULT_PORT, &hints, &result)) {
        std::cout << "getaddrinfo failed with error: " << iResult
            << '\n';
        WSACleanup();
        exit(1);
    }

    struct addrinfo *i;
    for (i = result; i != nullptr; i = i->ai_next) {
        ret = socket(i->ai_family, i->ai_socktype, i->ai_protocol);

        if (ret == INVALID_SOCKET) {
            std::cout << "socket() failed with error: " << WSAGetLastError()
                << '\n';
            WSACleanup();
            exit(1);
        }

        iResult = connect(ret, i->ai_addr, i->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ret);
            ret = INVALID_SOCKET;
            std::cout << "The server is down \n";
        }
    }

    if (ret == INVALID_SOCKET) {
        std::cout << "Unable to connect the server\n";
        WSACleanup();
        exit(1);
    }

    // Set non-blocking
    u_long i_mode = 1;
    iResult = ioctlsocket(ret, FIONBIO, &i_mode);

    if (iResult == SOCKET_ERROR) {
        std::cout << "ioctlsocket failed with error: " << iResult << '\n';
        closesocket(ret);
        WSACleanup();
        exit(1);
    }

    if (listen)
        ret = Sockets_bind(ret, result);

    freeaddrinfo(result);
    freeaddrinfo(&hints);

    return ret;
}

SOCKET Sockets::sockets_accept(SOCKET s) {
    return accept(s, nullptr, nullptr); // INVALID_SOCKET is already defined in winsock
}

#else // BSD sockets for nix systems

SOCKET Sockets::sockets_create(std::string&& address, bool listen) {
    SOCKET ret = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.sin_family = AF_INET;
    hints.sin_addr.s_addr = htonl(inet_addr(address.c_str()));

    int port  = std::stoi(DEFAULT_PORT);
    if (port > 65535 || port < 0) {
        std::cerr << "Port is out of range\n";
        exit(1);
    }

    hints.sin_port = htons((uint16_t)port);

    // bind
    if (listen) {
        if (bind(ret, (sockaddr *) &hints, sizeof(hints)) < 0) {
            std::cerr << "Error binding\n";
            exit(1);
        }
    }

    return ret;
}

SOCKET Sockets::sockets_accept(SOCKET s) {
    s = accept(s, nullptr, nullptr);
    if (s < 0)
        return INVALID_SOCKET;
    return s;
}
#endif