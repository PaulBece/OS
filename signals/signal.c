#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int cont =0;

void sighandler(int);
void f_usr_1(int);
void f_usr_2(int);

int main () {
   signal(SIGINT, sighandler);
   signal(SIGUSR1,f_usr_1);
   signal(SIGUSR2,f_usr_2);

   while(1) {
      printf("Going to sleep for a second...\n");
      sleep(1); 
   }
   return(0);
}

void sighandler(int signum) {
   printf("S.O. \t");
   cont=cont +1;
   printf("%d \n",cont);
}

void f_usr_1(int signum) {
   printf("Arquitectura \t");
   cont=cont +1;
   printf("%d \n",cont);
}

void f_usr_2(int signum) {
   printf("Redes \t");
   cont=cont +1;
   printf("%d \n",cont);
}