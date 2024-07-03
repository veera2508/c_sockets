#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "string.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "sys/_types/_socklen_t.h"
#include "netdb.h"

void error(const char *msg) {
  perror(msg);
  exit(1);
}


int main(int argc, char* argv[]) {

  if (argc < 3) {
    fprintf(stderr, "Port number is required. Usage: client <server_ip> <port>\n");
    exit(1);
  }
  // Socket File Descriptor - AF_INET = IPV4 SOCK_STREAM = TCP Protocol = 0
  int sockfd, portno, n;
  struct hostent *server;
  char buffer[255];

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("Error opening a socket!");

  memset(&serv_addr, 0, sizeof(serv_addr));
  portno = atoi(argv[2]);
  
  server = gethostbyname(argv[1]);
  if (server == NULL)
    error("No such host!");

  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("Connection Failed");

  while (1) {
    memset(buffer, 0, 255 * sizeof(buffer[0]));
    printf("Client: ");
    fgets(buffer, 255, stdin);

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
      error("Error during writing!");

    memset(buffer, 0, 255 * sizeof(buffer[0]));
    n = read(sockfd, buffer, 255);
    if (n < 0)
      error("Error during reading!");
    printf("Server: %s", buffer);
    
    
    if (strncmp("Bye", buffer, 3) == 0)
      break;
  }

  close(sockfd);
  return 0;
 
}
