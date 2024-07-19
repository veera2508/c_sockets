#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "string.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "stdlib.h"

void error(const char *msg) {
  perror(msg);
  exit(1);
}


int main(int argc, char* argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Port number is required. Usage: server <port>\n");
    exit(1);
  }
  // Socket File Descriptor - AF_INET = IPV4 SOCK_STREAM = TCP Protocol = 0
  int sockfd, newsockfd, portno, n;
  char buffer[255];

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("Error opening a socket!");

  memset(&serv_addr, 0, sizeof(serv_addr));
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("Binding Failed.");

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0)
    error("Error on Accept");

  while (1) {
    memset(buffer, 0, 255 * sizeof(buffer[0]));
    n = read(newsockfd, buffer, 255);
    if (n < 0) {
      error("Error during reading!");
    }
    printf("Client : %s", buffer);
    memset(buffer, 0, 255 * sizeof(buffer[0]));
    
    printf("Server: ");
    fgets(buffer, 255, stdin);

    n = write(newsockfd, buffer, strlen(buffer));
    if (n < 0) {
      error("Error during writing!");
    }

    if (strncmp("Bye", buffer, 3) == 0)
      break;

  }

  close(newsockfd);
  close(sockfd);
  return 0;

}
