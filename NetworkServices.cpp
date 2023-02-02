#include <iostream>
#include <string>
#include <limits.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <cstring> // std::memset()
#endif

#include "NetworkServices.h"

void NetworkServices::sockets_close(SOCKET s) {
    #ifdef _WIN32
    closesocket(s);
    #else
    close(s);
    #endif
    std::cout << "socket is closed\n";
}

void sockets_emitError(SOCKET s, addrinfo *result, std::string&& err, bool close_sock) {
    std::cerr << std::forward<std::string>(err) << "\nLast Error: " << get_last_error << '\n';
    freeaddrinfo(result);
    if (close_sock)
        NetworkServices::sockets_close(s);
    #ifdef _WIN32
    WSACleanup();
    #endif
    return;
}

// send() and recv() are OS agnostic
ssize_t NetworkServices::sendMessage(SOCKET curSocket, char * message, int messageSize) {
    return send(curSocket, message, messageSize, 0);
}

ssize_t NetworkServices::receiveMessage(SOCKET curSocket, char * buffer, int bufSize) {
    return recv(curSocket, buffer, bufSize, 0);
}

int NetworkServices::sockets_bind(SOCKET s, const sockaddr *addr, size_t namelen) {
    // OS agnostic
    // Returns 0 on success, SOCKET_ERROR on fail. Check get_last_error.
    return bind(s, addr, (int)namelen);
}

int NetworkServices::sockets_listen(SOCKET s, int backlog) {
    // OS agnostic
    // Returns 0 on success, SOCKET_ERROR on fail. Check get_last_error.
    return listen(s, backlog);
}

#ifdef _WIN32
    SOCKET NetworkServices::sockets_create(std::string&& address, bool listen_) {
        int iResult;
        WSADATA wsaData;
        SOCKET ret = INVALID_SOCKET;

        struct addrinfo *result, hints;

        // Initialize Winsock
        if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData))) {
            std::cout << "WSAStartup failed with error: " 
                << iResult << '\n';
            return INVALID_SOCKET;
        }

        // Set address info
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = listen_ ? AF_INET : AF_UNSPEC;
        hints.ai_flags = listen_ ? AI_PASSIVE : 0x00000000;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        if ((iResult = getaddrinfo(address.c_str(), DEFAULT_PORT, &hints, &result))) {
            std::cout << "getaddrinfo failed with error: " << iResult
                << '\n';
            WSACleanup();
            return INVALID_SOCKET;
        }

        if (listen_) {
            ret = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (ret == INVALID_SOCKET) {
                sockets_emitError(ret, result, "socket() failed", false);
                return INVALID_SOCKET;
            }

        } else { 
            ret = sockets_connect(result);
            if(ret == INVALID_SOCKET) {
                sockets_emitError(ret,  result, "connect() failed", false);
                return INVALID_SOCKET;
            }
        }

        if (listen_) {
            // bind
            iResult = sockets_bind(ret, result->ai_addr, (int)result->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                sockets_emitError(ret, result, "bind() failed", true);
                return INVALID_SOCKET;
            }

            // listen
            iResult = sockets_listen(ret, SOMAXCONN);
            if (iResult == SOCKET_ERROR) {
                sockets_emitError(ret, result, "listen() failed.", true);
                return INVALID_SOCKET;
            }
        }

        // Set non-blocking
        u_long i_mode = 1;
        iResult = ioctlsocket(ret, FIONBIO, &i_mode);

        if (iResult == SOCKET_ERROR) {
            sockets_emitError(ret, result, "ioctlsocket() failed", true);
            return INVALID_SOCKET;
        }

        freeaddrinfo(result);
        return ret;
    }

    SOCKET NetworkServices::sockets_connect(const addrinfo *a) {
        // Closes the socket on error.
        const struct addrinfo *i;
        SOCKET s = INVALID_SOCKET;
        for (i = a; i != nullptr; i = i->ai_next) {
            s = socket(i->ai_family, i->ai_socktype, i->ai_protocol);

            if (s == INVALID_SOCKET) {
                continue;
            }

            if(connect(s, a->ai_addr, (int)a->ai_addrlen) != -1)
                break;

            sockets_close(s);
            s = INVALID_SOCKET;
        }
        return s;
    }

    SOCKET NetworkServices::sockets_accept(SOCKET listen_socket) {
        SOCKET ret = INVALID_SOCKET;
        ret = accept(listen_socket, nullptr, nullptr);
        
        if (ret == INVALID_SOCKET)
            return INVALID_SOCKET;
        
        return ret;
    }

#else
    SOCKET NetworkServices::sockets_create(std::string&& address, bool listen_) {
        SOCKET ret = INVALID_SOCKET;

        struct addrinfo hints, *result;
        std::memset(&hints, 0, sizeof(hints));

        hints.ai_family = listen_ ? AF_INET : AF_UNSPEC;
        hints.ai_flags = listen_ ? AI_PASSIVE : 0x00000000;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        if (getaddrinfo(address.c_str(), DEFAULT_PORT, &hints, &result)) {
            sockets_emitError(INVALID_SOCKET, result, "getaddrinfo() failed", false);
            return INVALID_SOCKET;
        }

        if (listen_) {
            ret = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (ret == INVALID_SOCKET) {
                sockets_emitError(ret, result, "socket() failed", false);
                return INVALID_SOCKET;
            }

        } else { 
            ret = sockets_connect(result);
            if(ret == INVALID_SOCKET) {
                sockets_emitError(ret,  result, "connect() failed", false);
                return INVALID_SOCKET;
            }
        }

        if (listen_) {
            // bind()
            int err = 0;
            err = sockets_bind(ret, result->ai_addr, (int)result->ai_addrlen);
            if (err) {
                sockets_emitError(ret, result, "bind() failed", true);
                return INVALID_SOCKET;
            }

            // listen()
            err = sockets_listen(ret, SOMAXCONN);
            if (err) {
                sockets_emitError(ret, result, "listen() failed.", true);
                return INVALID_SOCKET;
            }
        }

        // Set non-blocking
        int err = fcntl(ret, F_SETFL, fcntl(ret, F_GETFL, 0) | O_NONBLOCK);

        if (err) {
            sockets_emitError(ret, result, "fcntl() error, can't be set non-blocking", true);
            return INVALID_SOCKET;
        }

        freeaddrinfo(result);
        //freeaddrinfo(&hints);

        // hints.ai_addr = htonl(inet_addr(address.c_str()));

        //int port  = std::stoi(DEFAULT_PORT);
        //if (port > 65535 || port < 0) {
        //    std::cerr << "Port is out of range\n";
        //    exit(1);
        //}

        // hints.sin_port = htons((uint16_t)port);

        return ret;
    }

    SOCKET NetworkServices::sockets_accept(SOCKET s) {
        s = accept(s, nullptr, nullptr);
        if (s < 0)
            return INVALID_SOCKET;
        return s;
    }

    SOCKET NetworkServices::sockets_connect(const struct addrinfo *result) {
        const struct addrinfo *ptr;
        SOCKET sfd = INVALID_SOCKET;
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
            sfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (sfd == INVALID_SOCKET)
                continue;

            if (connect(sfd, ptr->ai_addr, ptr->ai_addrlen) != -1)
                break;   // success.

            sockets_close(sfd);
            sfd = INVALID_SOCKET;
        }
        return sfd;
    }
#endif
