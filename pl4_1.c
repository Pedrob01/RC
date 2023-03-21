#include <unistd.h>
#include <stdio.h>


int main()
{
int f;
if((f = fork()) == -1 ){
    printf("Error while forking.");exit();
    
}
else if(f == 0){
    printf("I am the child process.");
}
else{
    printf("I am the father process.");
}
}
