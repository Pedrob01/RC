#include <unistd.h>
#include <stdio.h>

void read_from_fd(int fd, char * process) {
char c;
while(read(fd, &c, 1) != 0) {
printf("Process [%s] reading from descriptor %d has read |%c|\n", process, fd, c);
sleep(1); // so processes can compete for access to file
}
}

int main()
{
    char fname[10] = "txt.txt"
    int f,fd;
    fd = open(fname,O_RDONLY);
    if((f = fork()) == -1 ){
        printf("Error while forking.");exit();
        
    }
    else if(f == 0){
        read_from_fd(fd, "child");
    }
    else{
        read_from_fd(fd, "parent");
    }
}
