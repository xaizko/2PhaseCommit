#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define IP "127.0.0.1"
#define PORT 6767

int main() {
    int s; 
    struct sockaddr_in sock;
    char buf[512];
    int bytes;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	fprintf(stderr, "failed to create socket");
	return 1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT);
    sock.sin_addr.s_addr = inet_addr(IP);

    if (connect(s, (struct sockaddr *)&sock, sizeof(struct sockaddr_in)) != 0) {
	fprintf(stderr, "Failed to connect to server");
	close(s);
	return 2;
    }

    printf("Connected to coordinator\n");
    printf("Waiting for prepare phase...\n");

    memset(buf, 0, sizeof(buf));
    bytes = recv(s, buf, sizeof(buf) - 1, 0);
    if (bytes > 0) {
	buf[bytes] = '\0';
	printf("Received from coordinator: %s\n", buf);

	srand(time(NULL)); 
	int commit = rand() % 10;
	char *message;
	if (commit == 0) {
	    message = "ABORT";
	} else {
	    message = "COMMIT";
	}
	send(s, message, strlen(message), 0);
    } else if (bytes == 0) {
	printf("Coordinator closed connection\n");
    } else {
	fprintf(stderr, "Error receiving data\n");
    } 

    sleep(2);

    printf("Closing connection\n");
    close(s);
    return 0;
}
