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

void handle_request(int clientfd) {
  char out[100], in[100], *i, *o;
  memset(in, 0, 100);
  memset(out, 0, 100);

  // Leave last byte for null-terminator
  read(clientfd, in, 99);

  // Write characters from in to out and escape double-quotes and
  // backslashes
  // out[99] should contain '\0'
  // out[98] should be saved to accomodate an extra slash in case we
  // find a character that must be escaped.
  for (i = in, o = out; *i != 0 && o < &out[98]; i++, o++) {
    if (*i == '"' || *i == '\\') {
      *o = '\\'; 
      ++o;
      *o = *i; 
    }
    *o = *i;
  }

  write(clientfd, (char*)out, strlen(out) + 1);
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
