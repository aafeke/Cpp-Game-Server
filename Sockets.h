#ifndef _SOCKETS_H
#define _SOCKETS_H

#include <string>

#ifndef _WIN32
typedef int SOCKET;
#define INVALID_SOCKET -1
#endif

namespace Sockets {

// Size of the buffer
#ifndef DEFAULT_BUFLEN
#define DEFAULT_BUFLEN 512
#endif

// Port to connect sockets
#define DEFAULT_PORT "6881"


SOCKET sockets_create(std::string&& address, bool listen);
SOCKET sockets_accept(SOCKET s);

} // namespace Sockets

#endif
