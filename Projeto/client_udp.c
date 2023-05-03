#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#define BUFLEN 500	// Buffer length
#define PORT 9876	// Port for incoming messages
#define fileName "in.txt"
#define host "localhost"

void erro(char *s){
    perror(s);
	exit(1);
}

int fileSize(int fd) { // gets the file size
   struct stat s;
   if (fstat(fd, &s) == -1) {
      erro("fstat");
   }
   return(s.st_size);
}
int main(int argc,char *argv[]){
    char endServer[100];
    int socketfd, filefd;
    struct sockaddr_in addr;
    struct hostent *hostPtr;
    //opens the file to be sent to the server
    if((filefd = open(fileName, O_RDONLY)) == -1) erro("Unknown file"); // error opening file
    //gets the host address
    if ((hostPtr = gethostbyname(host)) == 0) erro("Could not get an address");
    //creates the socket
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) 9876);//argv 2 should be 9876

    if((socketfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1)
        erro("socket");

    //gets the file size and checks if it is bigger than the max buffer length
    int fsize = fileSize(filefd);
    if(fsize > BUFLEN){
        printf("File too big, sending only %d bytes", BUFLEN);
        fsize = BUFLEN;
    }
    if(fsize == 0) erro("File is empty");
    
    //reads the file contents
    char filecontents[fsize];
    read(filefd, filecontents, fsize);
    
    //prints file contents
    printf("\nSent message with:\nFile contents: %s \nSize: %d\n",filecontents,fsize);
    if( sendto(socketfd,filecontents,fsize,0,(struct sockaddr *)&addr,sizeof(addr)) < 0) // sends the file contents and checks for errors
        erro("Connect");
    close(socketfd);
    close(filefd);
}