#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


#define MAX_BUF 1024

int contador=0;

int main(int argc, char * argv[]){
    __pid_t pid;
    int signal1;

    pid = atoi(argv[1]);
    signal1 =atoi(argv[2]);

    
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];

    /* open, read, and display the message from the FIFO */
    fd = open(myfifo, O_RDONLY);
    while(1){
        read(fd, buf, MAX_BUF);
        printf("Received: %s\n", buf);
        contador = contador + 1;
        kill(pid,signal1);
        sleep(2);
        if (contador > 10) { 
            exit(0);
        }
    }
    close(fd);

    return 0;
}