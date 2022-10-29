#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error(char* msg){
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[]){
  // accepts only one arg
  if(argc != 3) error("no port provided");

  char buffer[255] = "1"; //message

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));

  int portno = atoi(argv[2]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
  serv_addr.sin_port = htons(portno);

  //create a socket
  int sfd = socket(AF_INET, SOCK_STREAM, 0);  

  struct hostent *server_addr;
  server_addr = gethostbyname(argv[1]);
  if(server_addr == NULL) error("no such host");
  bcopy((char *) server_addr->h_addr_list[0], (char *) &serv_addr.sin_addr.s_addr, server_addr->h_length);
  serv_addr.sin_port = htons(portno);
  if( connect(sfd, (struct sockaddr *)&serv_addr, (socklen_t)sizeof(serv_addr)) < 0 ) error("connection failed") ;
  
  while(1){
    fgets(buffer, 255, stdin);
    if( write(sfd, buffer, strlen(buffer)) < 0) error("could not write");
  }

  return 0;
}
