#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];
    char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html><head><style>*{  color: #fff;  font-size: 28px;  font-weight: 300;  box-sizing: border-box;  font-family: 'Metrophobic', Arial, serif; font-weight: 400;}body{  background-color: #2d2c2a;  padding: 50px;}header,nav,section,aside,footer{  text-align: center;  background-color: #4d5559;  border-radius: 5px;  padding: 50px;}header,nav,footer{  width: 100%;  padding: 50px;}header{  margin-bottom: 50px;}h1{  margin: 0;}nav,aside{  width: 25%;}section{  float: left;  width: 50%;  margin: 0 50px 50px 50px;}section header,section article,section footer{  width: 100%;  background-color: #6c6a7e;  border-radius: 5px;  margin: 0;  padding: 50px;}section article{  margin: 25px 0;}nav{  float: left;}aside{  float: right;}nav ul{  list-style-type: none;  padding: 0;  margin-bottom: 0;}nav li{  display: inline-block;  margin: 0 0 0 50px;  width: 100px;}nav li:first-child{  margin-left: 0;}footer{  clear: both;}</style></head><body><header>    <h1> Lab Assignment 2</h1>  </header>   <section>    <header>A web server</header>    <article>< article ></article>      </section>  <aside>    < aside >  </aside>  <footer>    &copy poushali  </footer></body></html>\r\n";

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("webserver (socket)");
        return 1;
    }
    printf("socket created successfully\n");

    // Create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
        perror("webserver (bind)");
        return 1;
    }
    printf("socket successfully bound to address\n");

    // Listen for incoming connections
    if (listen(sockfd, SOMAXCONN) != 0) {
        perror("webserver (listen)");
        return 1;
    }
    printf("server listening for connections\n");

    for (;;) {
        // Accept incoming connections
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr,
                               (socklen_t *)&host_addrlen);
        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted\n");

        // Read from the socket
        int valread = read(newsockfd, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("webserver (read)");
            continue;
        }

        // Write to the socket
        int valwrite = write(newsockfd, resp, strlen(resp));
        if (valwrite < 0) {
            perror("webserver (write)");
            continue;
        }

        close(newsockfd);
    }

    return 0;
}
