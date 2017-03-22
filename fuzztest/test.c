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
  char i[4];
  ssize_t j; 
	
  recv(clientfd, i, 4, 0);

  if (i[0] <= 'Q') {
    //j = recv(clientfd, i, 4, 0);
    if (i[2] >= 'O') {
      send(clientfd, "A", 1, 0);
    } else {
      send(clientfd, "B", 1, 0);
    }
  } else if (i[0] <= 'f'){
    //j = recv(clientfd, i, 4, 0);
    if (i[3] >= 'd') {
      send(clientfd, "v", 1, 0);
    } else {
      send(clientfd, "L", 1, 0);
    }
  } else if (i[0] <= 'p'){
    //recv(clientfd, i, 4, 0);
    if (i[2] >= 'M') {
      send(clientfd, "6", 1, 0);
    } else {
      send(clientfd, "i", 1, 0);
    }
  } else {
    //recv(clientfd, i, 4, 0);
    if (i[3] >= 'f') {
      send(clientfd, "Z", 1, 0);
    } else {
      send(clientfd, "f", 1, 0);
    }
  } 
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
    printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    handle_request(clientfd);
    close(clientfd);
  }

  close(sockfd);
  return 0;
}
