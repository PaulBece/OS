#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int signalON=1;
int contador=0;

void sigchld_handler(int sig){
    signalON = 1;
}


int main (int argc, char * argv[]) {
   __pid_t pid;
   int signal1;

   pid = atoi(argv[1]);
   signal1 =atoi(argv[2]);

   signal(SIGINT, sigchld_handler);

   while(1){
      if(signalON){
         signalON=0;
         kill(pid,signal1);
         contador=contador+1;
         if (contador>10){
            exit(0);
         }
      }
   }

   
   return(0);
}