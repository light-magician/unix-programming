// overly basic server to practice making socket connections
// server will allow one conection, will serve the client 'hello'
// then close the connection and wait for other connections
//
// compile with:
// clang server.c -o server.exe
//
// run with:
// ./server.exe
//
// test with:
// telnet localhost 8080
//
// cannot use curl as it expects http protocol

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void configure_address(struct sockaddr_in *address, const char *ip, int port) {
  address->sin_family = AF_INET;
  // htons converts a 16-bit number from host byte order to
  // network byte order.
  address->sin_port = htons(port);
  if (ip == NULL) {
    // server route
    address->sin_addr.s_addr = INADDR_ANY;
  } else {
    // client route
    inet_pton(AF_INET, ip, &address->sin_addr);
  }
}

void serve() {
  int server_fd, client_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  // create a socket, get a socket descriptor
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  // address configuration
  configure_address(&address, NULL, 8080);
  // bind socket to address
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  listen(server_fd, 3);

  char *message = "hello\n";

  printf("server listening on port 8080\n");

  while (1) {
    client_fd =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    send(client_fd, message, strlen(message), 0);
    close(client_fd);
  }
  close(server_fd);
}

int main() {
  serve();
  return 0;
}
