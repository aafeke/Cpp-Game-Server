#include "NetworkServices.h"

int NetworkServices::sendMessage(SOCKET curSocket, char * message, int messageSize) {

    // From winsock library, flag is set to 0
    return send(curSocket, message, messageSize, 0);
}

int NetworkServices::receiveMessage(SOCKET curSocket, char * buffer, int bufSize) {
    return recv(curSocket, buffer, bufSize, 0);
}