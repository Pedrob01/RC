/*******************************************************************
 * SERVER port 9000 listen for new clients.  When data arrives from *
 * a new client it is read and echoed on the screen.                *
 *******************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

#define SERVER_PORT 9000
#define BUF_SIZE	1024

void process_client(int fd);
void erro(char *msg);

int main() {
    int fd, client;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;

    bzero((void *) &addr, sizeof(addr)); /* fills with zeros */
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);

    if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        erro("in funtion socket");
    if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
        erro("in fucntion bind");
    if( listen(fd, 5) < 0)
        erro("in fucntion listen");

    /* Wait for client connections */
    while (1) {
        client_addr_size = sizeof(client_addr);
        client = accept(fd,(struct sockaddr *)&client_addr,
                        (socklen_t *) &client_addr_size);
        if (client > 0) {
            if (fork() == 0) {
                close(fd); /* closes father process file descriptor */
                process_client(client);
                exit(0);
            }
            close(client);
        } /* no processing done in case of error */

    }
    return 0;
}

void process_client(int client_fd)
{
    int nread = 0;
    char buffer[BUF_SIZE];

    nread = read(client_fd, buffer, BUF_SIZE-1);
    
    if(nread < 0) erro("err reading from client\n");
    
    buffer[nread] = '\0';

    /* done */

    //print the buffer backwards

    int i = 0;
    int j = 0;
    char temp;
    while(buffer[i] != '\0'){
        i++;
    }
    i--;
    while(j < i){
        temp = buffer[j];
        buffer[j] = buffer[i];
        buffer[i] = temp;
        j++;
        i--;
    }

    printf("%s\n", buffer);
    fflush(stdout);
    
    close(client_fd);
}

void erro(char *msg)
{
    fprintf(stderr, "Erro: %s\n", msg);
    exit(-1);
}
