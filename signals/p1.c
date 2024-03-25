#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

char* numeroAleatorio() {
    srand(time(NULL));
    int num = rand() % 1000;
    char* str_num = (char*)malloc(4 * sizeof(char));
    sprintf(str_num, "%d", num);
    return str_num;
}

void sigchld_handler(int sig);
int contador=0;
int signalON;
int main()
{
    signalON = 0;
    
    signal(SIGINT, sigchld_handler);
    
    int fd;
    char * myfifo = "/tmp/myfifo";
    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);
    
    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY);
    for(;;){
        if (signalON){
            signalON=0;
            char* n=numeroAleatorio();
            write(fd, n, sizeof(n));
            contador = contador + 1;
            if (contador > 10) { 
                exit(0);
            }
        }
    }
    close(fd);

    /* remove the FIFO */
    unlink(myfifo);

    return 0;
}


void sigchld_handler(int sig){
    signalON = 1;
}
