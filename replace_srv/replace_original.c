#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MY_PORT   9999

struct header {
  char from;
  char to;
  unsigned short len;
};

void handle_request(int clientfd) {
  struct header hdr;
  char string[100];
  int i;

  read(clientfd, &hdr, sizeof(struct header));
  read(clientfd, (char*)string, hdr.len);

  for (i=0; i<hdr.len; i++) {
    if (string[i] == hdr.from)
      string[i] = hdr.to;
  }

  write(clientfd, (char*)string, (size_t)hdr.len);
}

int main(int Count, char *Strings[])
{   
  int sockfd;
  struct sockaddr_in self;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket");
    exit(errno);
  }

  memset(&self, 0, sizeof(self));
  self.sin_family = AF_INET;
  self.sin_port = htons(MY_PORT);
  self.sin_addr.s_addr = INADDR_ANY;

  int enable = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
  {
    perror("setsockopt(SO_REUSEADDR) failed");
    exit(errno);
  }

  if (bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0)
  {
    perror("socket--bind");
    exit(errno);
  }

  if (listen(sockfd, 20) != 0 )
  {
    perror("socket--listen");
    exit(errno);
  }

  /*---Forever... ---*/
  while (1)
  { 
    int clientfd;
    struct sockaddr_in client_addr;
    unsigned int addrlen=sizeof(client_addr);

    clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
    handle_request(clientfd);
    close(clientfd);
  }

  close(sockfd);
  return 0;
}
