#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUMSTRS 3
#define ADDRESS "usock" /* Unix addr to connect to */

char *strs[3] = {
    "Client: Sending string 1.\n",
    "Client: Sending string 2.\n",
    "Client: Sending string 3.\n"
};

int main() {
  char c;
  FILE *fp = NULL;
  int i, s, len, status;
  struct sockaddr_in sin; /* struct for internet addr */
  struct addrinfo hints;
  struct addrinfo *svrinfo;

  // Create address 
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // ipv4 or ipv6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; 

  if ((status = getaddrinfo(NULL, "3960", &hints, &svrinfo)) !=0) {
     fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
     exit(1);
  } 
  
  // Get internet socket of type stream
  if ((s = socket(svrinfo->ai_family, svrinfo->ai_socktype, svrinfo->ai_protocol)) < 0) {
      perror("client: socket");
      exit(1);
  }

  // connect to server if it's listen()ing
  if (connect(s, svrinfo->ai_addr, svrinfo->ai_addrlen) < 0) {
      perror("client: connect");
      exit(1);
  }

  // stdio to read from string

  fp = fdopen(s, "r");
  for (i = 0; i < NUMSTRS; i++) {
      while ((c = fgetc(fp)) != EOF) {
        putchar(c);
        if (c == '\n')
            break;
      }
  }

  // send strings to server
  for (i = 0; i < NUMSTRS; i++) 
      send(s, strs[i], strlen(strs[i]), 0);

  close(s);
  freeaddrinfo(svrinfo);

  return 0;
}

