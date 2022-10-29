#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// perror wrapper
void error(const char* msg){
  perror(msg);
  exit(1);
}


//main
int main(int argc, char* argv[]){

  // accepts only one arg
  if(argc != 2) error("no port provided");

  char buffer[255]; //message

  int n, exit;

  // our initial socketfd
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sfd < 0) error("while creating socket");

  // structure to store detials
  struct sockaddr_in serv_addr, cli_addr;  //socket address in internet space
  memset(&serv_addr, 0, sizeof(serv_addr)); //set all instance variables to zero
  // bzero((char *) &serv_addr, sizeof(serv_addr));

  // port
  int portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);  // host to network short

  if(bind(sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) != 0) error("Binding failed");
  
  listen(sfd, 5); //listens for max 5 clients

  socklen_t clilen = sizeof(cli_addr);  //size of client addr

  int newsfd = accept(sfd, (struct sockaddr *)&serv_addr, &clilen); //accept the connection

  if(newsfd < 0) error("accept failed");

  while(1){
    bzero(buffer, 255);
    n = read(newsfd, buffer, strlen(buffer)); //read
    if(n < 0) error("could not read");
    printf("Client: %s", buffer);

    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(newsfd, buffer, sizeof(buffer));  //write
    if(n < 0) error("could not write");

    exit = strncmp("exit", buffer, 4);
    if(exit == 0) break;   // exit
  }
  close(newsfd);
  close(sfd);

  return 0;
}
