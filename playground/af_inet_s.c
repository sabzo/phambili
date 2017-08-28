#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMSTRS 3
#define ADDRESS "usock"

char *strs[NUMSTRS] = {
  "server: string 1\n",
  "server: string 2\n",
  "server: string 3\n"
};

int main() {
  char c;
  FILE *fp;
  int fromlen, i, s, len, ns, status;
  struct sockaddr_in sin;
  struct addrinfo hints, *res;

  // create address
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
 
  if ((s = getaddrinfo(NULL, "3960", &hints, &res)) !=0) { 
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
   exit(1);
 } 
  
  // create socket
  if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
      perror("server: socket");
      exit(1);
  }

  if (bind(s, res->ai_addr, res->ai_addrlen) < 0) {
    perror("sever: bind");
    exit(1);
  }

  // Listen on the socket 
  if (listen(s, 5) < 0) {
      perror("server: listen");
      exit(1);
  }
  printf("Listening for client\n");

  // Accept Connections
  if ((ns = accept(s, (struct sockaddr *) &sin, &fromlen)) < 0) {
      perror("sever: accept");
      exit(1);
  }

  // stdio to read from socket
  fp = fdopen(ns, "r");

  // send to client
  for (i = 0; i < NUMSTRS; i++) 
      send(ns, strs[i], strlen(strs[i]), 0);

  // now read from client
  for (i = 0; i < NUMSTRS; i++) {
    while ((c = fgetc(fp)) != EOF) {
      putchar(c);
      if (c == '\n')
        break;
    }
  }

  freeaddrinfo(res);
  close(s);
  close(ns);
  return 0;
}
