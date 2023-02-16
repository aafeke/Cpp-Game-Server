# Simple Server - Client Implementation

Just wanted to see how things work under the hood. Tested in Ubuntu and Windows. It supports Win client
communicating with Linux server or the vice-versa.

## Usage
Build the server with:
`make server`

and then the client:
`make client`

Run the server:
`./server` for Windows or `./server_nix` for *nix systems.

Run the client:
`./client <server-address>` for Windows or `./client_nix <server-address>` for *nix systems.

The port is 6881 by default.