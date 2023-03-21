#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFLEN 512	// Buffer length
#define PORT 9876	// Port for incoming messages

void erro(char *s)
{
    perror(s);
	exit(1);
}

int main(int argc,char *argv[]){
    char endServer[100];
    int fd;
    struct sockaddr_in addr;
    struct hostent *hostPtr;

    if (argc != 4) {
        printf("client <host> <port> <string>\n");
        exit(-1);
    }

    strcpy(endServer, argv[1]);// argv 1 should be localhost
    if ((hostPtr = gethostbyname(endServer)) == 0)
        erro("Could not get an address");

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));//argv 2 should be 9876

    if((fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1)
        erro("socket");

    if( sendto(fd,argv[3],1 + sizeof(argv[3]),0,(struct sockaddr *)&addr,sizeof(addr)) < 0)
        erro("Connect");

    
}