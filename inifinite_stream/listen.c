#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>


void error(char* msg){
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[]){
  // accepts only one arg
  if(argc != 2) error("no port provided");

  char buffer[255]; //message

  struct sockaddr_in serv_addr, cli_addr;
  bzero(&serv_addr, sizeof(serv_addr));

  int portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  //create a socket
  int sfd = socket(AF_INET, SOCK_STREAM, 0);  

  //bind the socket to the port
  if(bind(sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) != 0) error("Binding failed");

  //only accept 1 client
  listen(sfd, 2);

  socklen_t clilen = sizeof(cli_addr);

  //accept connection from original serv_addr and return newsfd
  int newsfd = accept(sfd, (struct sockaddr *)&serv_addr, &clilen);
  
  bzero(buffer, 255);

  while(1){
    if( read(newsfd, buffer, 255) < 0) error("could not read");
    printf("%s", buffer);
    bzero(buffer, 255);
    
  }

  return 0;
}
