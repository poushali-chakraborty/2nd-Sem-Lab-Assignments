#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"
#define BUFSIZE 100
int main() {

char echoString[BUFSIZE];
const char* server_name = "localhost";
const int server_port = 9877;

struct sockaddr_in server_address;

printf("\nenter a string:\n");
scanf("%s",echoString);
memset(&server_address, 0, sizeof(server_address));
server_address.sin_family = AF_INET;
// IPv4 address family
// Convert address
inet_pton(AF_INET, server_name, &server_address.sin_addr);
// htons: port in network order format
server_address.sin_port = htons(server_port);
// open a stream socket
int sock;
if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	printf("could not create socket\n");
	return 1;
}

// Server port
// Establish the connection to the echo server
if (connect(sock, (struct sockaddr*)&server_address,sizeof(server_address)) < 0)
DieWithSystemMessage("connect() failed");

size_t echoStringLen = strlen(echoString); // Determine input length
// Send the string to the server
ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
if (numBytes < 0)
DieWithSystemMessage("send() failed");
else if (numBytes != echoStringLen)
DieWithUserMessage("send()", "sent unexpected number of bytes");
// Receive the same string back from the server
unsigned int totalBytesRcvd = 0; // Count of total bytes received

// Setup to print the echoed string
while (totalBytesRcvd < echoStringLen) {
char buffer[BUFSIZE]; // I/O buffer
/* Receive up to the buffer size (minus 1 to leave space for
a null terminator) bytes from the sender */
numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
if (numBytes < 0)
DieWithSystemMessage("recv() failed");
else if (numBytes == 0)
DieWithUserMessage("recv()", "connection closed prematurely");
totalBytesRcvd += numBytes; // Keep tally of total bytes
buffer[numBytes] = '\0';
// Terminate the string!
printf("received: '%s'\n", buffer);
// Print the echo buffer
}

close(sock);
exit(0);
}
