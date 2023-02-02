#ifndef NETWORK_SERVICES_H
#define NETWORK_SERVICES_H

#include <string>

#ifndef _WIN32
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define get_last_error errno
#include <sys/socket.h>

#else
#include <winsock2.h>
#include <ws2tcpip.h>
#define get_last_error WSAGetLastError()
#endif

// Port to connect sockets
#define DEFAULT_PORT "6881"

// Size of the buffer
#ifndef DEFAULT_BUFLEN
#define DEFAULT_BUFLEN 512
#endif

namespace NetworkServices {

SOCKET sockets_create(std::string&& address, bool listen);
SOCKET sockets_accept(SOCKET s);
SOCKET sockets_connect(const struct addrinfo *a);
int sockets_bind(SOCKET s, const sockaddr *addr, size_t namelen);
int sockets_listen(SOCKET s, int backlog);
void sockets_close(SOCKET s);
ssize_t sendMessage(SOCKET curSocket, char * message, int messageSize);
ssize_t receiveMessage(SOCKET curSocket, char * buffer, int bufSize);

} // namespace NetworkServices

#endif