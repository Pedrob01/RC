#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFLEN 500	// Buffer length
#define PORT 9876	// Port for incoming messages
#define fileName "out.txt"

void erro(char *s)
{
    perror(s);
	exit(1);
}

int main(void)
{
    struct sockaddr_in si_mine, si_other;
	
	int s,filefd;
    socklen_t  slen = sizeof(si_other);
	char buf[BUFLEN];
    
    //opens the file to be written
    if((filefd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        erro("Error opening file");
    }


    // Cria um socket para recepção de pacotes UDP
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
	    erro("Err creating the socket");
    }
    
    si_mine.sin_family = AF_INET;
    si_mine.sin_port = htons(PORT);
    si_mine.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Associates a socket to the address data
    printf("Binding socket to port %d", PORT);
    if(bind(s,(struct sockaddr*)&si_mine, sizeof(si_mine)) == -1)
    {
	    erro("Error in bind");
    }

    int recv_len; // Length of received message
    recv_len = recv(s, buf, BUFLEN, 0);
    printf("\nlength %d\n",recv_len);
    if(recv_len > BUFLEN){
        printf("Error: received more than %d bytes", BUFLEN);
        recv_len = BUFLEN;
    }

    //copy recv_len characters from buf to cbuf
    char cbuf[recv_len]; 
    strncpy(cbuf, buf, recv_len);
    printf("Received %d bytes, sizeof(cbuf) %ld", recv_len, sizeof(cbuf));
    printf("\nReceived %s\n",buf);

    // Writes the received message to the file
    write(filefd, buf, recv_len);

    // Prints the received message
    printf("\nThe message content is: %s\n" , buf);

    // Closes socket, file and ends program
    close(filefd);
    close(s);
    return 0;
}
