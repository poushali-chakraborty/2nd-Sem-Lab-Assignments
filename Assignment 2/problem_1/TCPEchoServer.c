/**
 * 
 *  TCPEchoServer.c
 *
 * @details
 * The program is based on the simple TCP client and server model. Server
 * runs an infinite loop,  client takes user input string and sends it to the server. 
 * The server receives the string and print and send it to client one byte at a time,
 * sleeping one second between each byte. Client require multiple recieve to get the full string.
 * In this way, ECHO program
 * using TCP is shown using the server-client model & socket programming
 */
#include <arpa/inet.h>  /// For the type in_addr_t and in_port_t
#include <netinet/in.h>  /// For in_addr and sockaddr_in structures
#include <stdio.h>  /// Variable types, several macros, and various functions for performing input and output
#include <stdlib.h>  /// Variable types, several macros, and various functions for performing general functions
#include <string.h>  /// Various functions for manipulating arrays of characters
#include <sys/socket.h>  /// For macro definitions related to the creation of sockets
#include <sys/types.h>  /// For definitions to allow for the porting of BSD programs
#include <unistd.h>  /// For miscellaneous symbolic constants and types, and miscellaneous functions
#include "Practical.h" ///For DieWithMessage
#define BUFSIZE 100
static const int MAXPENDING = 5; /// Maximum outstanding connection requests
int main() {
int SERVER_PORT = 9877;  /// Define port over which communication will take place
char buffer[BUFSIZE];   /// character arrays to read
                        /// and store string data

									// socket address used for the server
struct sockaddr_in server_address; /// basic structures for all syscalls and functions that
                      /// deal with internet addresses. Structures for handling
                      /// internet addresses

memset(&server_address, 0, sizeof(server_address));  ///memset() is used to fill a block of memory with a particular value.
//void *memset(void *ptr, int x, size_t n);	 ptr ==> Starting address of memory to be filled
// x   ==> Value to be filled
// n   ==> Number of bytes to be filled starting 




server_address.sin_family = AF_INET; ///AF_INET (Family) - it is an address family that is used to designate the
     ///type of addresses that our socket can communicate with

// htons: host to network short: transforms a value in host byte
// ordering format to a short value in network byte ordering format
server_address.sin_port = htons(SERVER_PORT);

// htonl: host to network long: same as htons but to long
server_address.sin_addr.s_addr = htonl(INADDR_ANY);

///These functions are necessary so that the binding of address and port
/// takes place with data in the correct format

/**
     * The TCP socket is created using the socket function.
     *
     * PF_INET (Family) - it is an address family that is used to designate the
     * type of addresses that your socket can communicate with
     *
     * SOCK_STREAM (Type) - Indicates TCP Connection - TCP require the
     * source and destination to establish a three-way handshake before
     * transmission takes place. Additionally, there is need for an
     * end-to-end connection
     *
     * 0 (Protocol) - Specifies a particular protocol to be used with the
     * socket. Specifying a protocol of 0 causes socket() to use an unspecified
     * default protocol appropriate for the requested socket type.
     */


int listen_sock;
if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	printf("could not create listen socket\n");
		return 1;
}
// Local port
// Bind to the local address

/**
     * This binds the socket descriptor to the server thus enabling the server
     * to listen for connections and communicate with other clients
     */
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
char temp[2];
temp[1]='\0';
// Receive message from client
ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
printf("received: '%s'\n", buffer);
if (numBytesRcvd < 0)
DieWithSystemMessage("recv() failed");
// Send received string and receive again until end of stream
int i=0;
while (numBytesRcvd > 0) { // 0 indicates end of stream
// Echo message back to client
ssize_t numBytesSent;
for(i=0 ;i<numBytesRcvd;i++){
temp[0]=buffer[i];
 numBytesSent += send(clntSocket, temp, 1, 0); //sending only single byte.


//sleep for one second

sleep(1);
}

if (numBytesSent < 0)
DieWithSystemMessage("send() failed");
else if (numBytesSent != numBytesRcvd)
DieWithUserMessage("send()", "sent unexpected number of bytes");
// See if there is more data to receive
numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
printf("received: '%s'\n", buffer);
if (numBytesRcvd < 0)
DieWithSystemMessage("recv() failed");
}

close(clntSocket); // Close client socket
}
