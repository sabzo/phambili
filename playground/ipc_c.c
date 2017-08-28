#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
  int i, s, len;
  struct sockaddr_un sun; /* struct for unix addr */

  // Get unix domain socket of type stream
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
      perror("client: socket");
      exit(1);
  }
  // Create address 
  
  sun.sun_family = AF_UNIX;
  strcpy(sun.sun_path, ADDRESS);

  len = sizeof(sun.sun_family) + strlen(sun.sun_path);

  // connect to server if it's listen()ing
  if (connect(s, &sun, len) < 0) {
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

  return 0;
}

