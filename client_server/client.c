#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>

// perror wrapper
void error(const char* msg){
  perror(msg);
  exit(1);
}


//main
int main(int argc, char* argv[]){

  // accepts only one arg
  if(argc != 3) error("no port provided");

  char buffer[255]; //message

  int n, exit;

  // our initial socketfd
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sfd < 0) error("while creating socket");

  // structure to store detials
  struct sockaddr_in serv_addr; //socket address in internet space
  struct hostent *server;
  memset(&serv_addr, 0, sizeof(serv_addr)); //set all instance variables to zero
  // bzero((char *) &serv_addr, sizeof(serv_addr));

  // port
  int portno = atoi(argv[2]);

  server = gethostbyname(argv[1]);
  if(server == NULL) error("no such host");

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *) server->h_addr_list[0], (char *) &serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  if( connect(sfd, (struct sockaddr *)&serv_addr, (socklen_t)sizeof(serv_addr)) < 0 ) error("connection failed") ;

  while(1){
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(sfd, buffer, strlen(buffer));
    if(n < 0) error("could not write");

    bzero(buffer, 255);
    n = read(sfd, buffer, 255);
    if(n < 0) error("could not read");
    printf("Server: %s", buffer);

    exit = strncmp("exit", buffer, 4);
    if(exit == 0) break;   // exit
  }

  close(sfd);
  return 0;
}
