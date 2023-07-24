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
static const int MAXPENDING = 5; // Maximum outstanding connection requests
int main() {
int SERVER_PORT = 9877;
char buffer[BUFSIZE];
char *pbuffer = buffer;


	// socket address used for the server
struct sockaddr_in server_address;
memset(&server_address, 0, sizeof(server_address));
server_address.sin_family = AF_INET;

// htons: host to network short: transforms a value in host byte
// ordering format to a short value in network byte ordering format
server_address.sin_port = htons(SERVER_PORT);

// htonl: host to network long: same as htons but to long
server_address.sin_addr.s_addr = htonl(INADDR_ANY);
int listen_sock;
if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	printf("could not create listen socket\n");
		return 1;
}
// Local port
// Bind to the local address
if ((bind(listen_sock, (struct sockaddr *)&server_address,sizeof(server_address))) < 0)
DieWithSystemMessage("bind() failed");
// Mark the socket so it will listen for incoming connections
if (listen(listen_sock, MAXPENDING) < 0)
DieWithSystemMessage("listen() failed");
for (;;) { // Run forever
struct sockaddr_in clntAddr; // Client address
// Set length of client address structure (in-out parameter)
socklen_t clntAddrLen = sizeof(clntAddr);
// Wait for a client to connect
int clntSock = accept(listen_sock, (struct sockaddr *) &clntAddr, &clntAddrLen);
if (clntSock < 0)
DieWithSystemMessage("accept() failed");
// clntSock is connected to a client!
char clntName[INET_ADDRSTRLEN]; // String to contain client address
if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,sizeof(clntName)) != NULL)
printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
else
puts("Unable to get client address");
HandleTCPClient(clntSock);
}
// NOT REACHED
}


void HandleTCPClient(int clntSocket) {
char buffer[BUFSIZE]; // Buffer for echo string

// Receive message from client
memset(buffer,0,BUFSIZE);
ssize_t numBytesRcvd = recv(clntSocket, buffer,1, 0);// receiving only single byte.
printf("received: '%s'\n", buffer);
if (numBytesRcvd < 0)
DieWithSystemMessage("recv() failed");
// Send received string and receive again until end of stream
int i=0;
while (numBytesRcvd > 0) { // 0 indicates end of stream
// Echo message back to client
ssize_t numBytesSent=send(clntSocket, buffer, 1, 0); //sending only single byte.

if (numBytesSent < 0)
DieWithSystemMessage("send() failed");
else if (numBytesSent != numBytesRcvd)
DieWithUserMessage("send()", "sent unexpected number of bytes");
 
}

close(clntSocket); // Close client socket
}
