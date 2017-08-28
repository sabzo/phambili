#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
  int fromlen, i, s, len, ns;
  struct sockaddr_un sun, fsun;

  // create socket
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
      perror("server: socket");
      exit(1);
  }
  // create address to bind to
  sun.sun_family = AF_UNIX;
  strcpy(sun.sun_path, ADDRESS);
  // name socket (bind)
  unlink(ADDRESS);
  len = sizeof(sun.sun_family) + strlen(sun.sun_path);

  if (bind(s, (struct sockaddr *) &sun, len) < 0) {
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
  if ((ns = accept(s, (struct sockaddr *) &fsun, &fromlen)) < 0) {
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

  close(s);
  return 0;
}
