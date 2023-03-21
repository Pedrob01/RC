#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512	// Buffer length
#define PORT 9876	// Port for incoming messages

void erro(char *s)
{
    perror(s);
	exit(1);
}

int main(void)
{
    struct sockaddr_in si_mine, si_other;
	
	int s;
    size_t recv_len;
    socklen_t  slen = sizeof(si_other);
	char buf[BUFLEN];
    
    // Cria um socket para recepção de pacotes UDP
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
	    erro("Err creating the socket");
    }
    
    si_mine.sin_family = AF_INET;
    si_mine.sin_port = htons(PORT);
    si_mine.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Associates a socket to the address data
    if(bind(s,(struct sockaddr*)&si_mine, sizeof(si_mine)) == -1)
    {
	    erro("Error in bind");
    }

    // Waits for an incoming message 
    if((recv_len = recvfrom(s, buf, BUFLEN, 0, 
       (struct sockaddr *) &si_other, &slen)) == -1)
    {
	erro("Error in recvfrom");
    }
    // To ignore the remaining content (prior content of the buffer)
    buf[recv_len]='\0';
	    
    printf("I received a message from the system with address %s and port %d\n",
	inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
    printf("The message conten is: %s\n" , buf);

    // Closes socket and ends program
    close(s);
    return 0;
}
