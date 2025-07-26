#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 6767
#define MAX_CLIENTS 10
#define MIN_CLIENTS 3

int main() {
    int s, c;
    struct sockaddr_in sock, client;
    socklen_t addrlen = sizeof(client);
    char buf[512];
    int bytes;
    int client_sockets[MAX_CLIENTS];
    int num_clients = 0;

    // Initialize client sockets
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = -1; // -1 indicates an unused slot
    }

    //create socket 
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	fprintf(stderr, "Failed to create socket\n");
	close (s);
	return 1;
    }

    //fill out socket 
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = INADDR_ANY; // local host
    sock.sin_port = htons(PORT);

    //makes sure port is open
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //binds socket 
    if (bind(s, (struct sockaddr *)&sock, sizeof(sock))) {
	fprintf(stderr, "Failed to bind");
	close (s);
	return 2;
    }

    if (listen(s, 5)) {
	fprintf(stderr, "Failed to listen on socket");
	close(s);
	return 3;
    }

    printf("2PC coordinator listening on: %s:%d\n", inet_ntoa(sock.sin_addr), PORT);
    printf("Waiting for participants to connect...\n");
}


