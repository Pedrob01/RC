/***********************************************************************
* The CLIENT connects to the server (in argv[1]) through the specified *
* port  (in argv[2]), writing a predefined word (in argv[3]):          *
*  ./client <Server_addr>  <port>  <word>                              *
 ***********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

void erro(char *msg);

int main(int argc, char *argv[]) {

    //do-while loop that reads an input from the user and sets the opt variable to the corresponding value
    int opt;
    do{
        printf("1. Capital of Portugal\n2. Capital of Spain\n3. Capital of France\n");
        scanf("%d", &opt);
    }while(opt < 1 || opt > 3);
    // Prints the corresponding capital to the user's input
    char capital[100];
    switch(opt){
        case 1:
            strcpy(capital, "Lisbon");
            break;
        case 2:
            strcpy(capital, "Madrid");
            break;
        case 3:
            strcpy(capital, "Paris");
            break;
    }


    char endServer[100];
    int fd;
    struct sockaddr_in addr;
    struct hostent *hostPtr;

    if (argc != 3) {
        printf("client <host> <port>\n");
        exit(-1);
    }

    strcpy(endServer, argv[1]);
    if ((hostPtr = gethostbyname(endServer)) == 0)
        erro("Could not get an address");

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]  ));

    if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        erro("socket");
    if( connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0)
        erro("Connect");
    write(fd, capital, 1 + strlen(capital));

    /* done */
    
    close(fd);
    exit(0);
}

void erro(char *msg)
{
    fprintf(stderr, "Erro: %s\n", msg);
    exit(-1);
}
