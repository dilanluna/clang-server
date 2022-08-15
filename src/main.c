#include <poll.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 3000
#define NO_TIMEOUT -1

int main() {
  struct sockaddr_in addr, peeraddr;
  socklen_t peeraddr_len;
  struct pollfd sockpfd;
  int sockfd, peerfd, ready;

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));

  listen(sockfd, SOMAXCONN);

  memset(&sockpfd, 0, sizeof(struct pollfd));
  sockpfd.fd = sockfd;
  sockpfd.events = POLLIN;

  printf("Server is running on port %d\n", PORT);

  while((ready = poll(&sockpfd, 1, NO_TIMEOUT)) != -1) {
    if(sockpfd.revents & POLLIN) {
      peerfd = accept(sockfd, (struct sockaddr *) &peeraddr, &peeraddr_len);

      printf("incomming request from %s\n", inet_ntoa(peeraddr.sin_addr));

      char response[200];
      char body[50] = "Hello world!";
      sprintf(response, "HTTP/1.1 200 Ok\nContent-Length: %ld\nContent-Type: text/plain\n\n%s", strlen(body), body);
      send(peerfd, response, sizeof(response), 0);
    } else {
      close(sockfd);
      break;
    }
  }

  return 0;
}
