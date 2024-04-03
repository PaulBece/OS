#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


int main(void)
{
        int     pipe1[2],pipe2[2], nbytes, numero=99;
        pid_t   childpid;
        char    readbuffer[80];

        pipe(pipe1);
        pipe(pipe2);

        printf("Main program:\n"); 

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }
 
        if(childpid == 0) // HIJO
        {
                for(int i =0;i<10;i++)
                {
                    close(pipe1[1]);
                    nbytes = read(pipe1[0], readbuffer, sizeof(readbuffer));               
                    printf("Child reads: %s", readbuffer);
                    numero = atoi(readbuffer);
                    numero = numero +3;

                    sprintf(readbuffer, "%d\n",numero);
                    close(pipe2[0]);
                    write(pipe2[1], readbuffer, (strlen(readbuffer)));
                    }
                exit(0);
        }
        else  // PADRE (REPETIR LO MISMO QUE EN EL HIJO)
        {

                for(int i =0;i<10;i++){

                    close(pipe1[0]);
                    sprintf(readbuffer, "%d\n",numero);
                    write(pipe1[1], readbuffer, (strlen(readbuffer)+1));

                    close(pipe2[1]);
                    nbytes = read(pipe2[0], readbuffer, sizeof(readbuffer));           
                    printf("Parent reads: %s", readbuffer);
                    numero = atoi(readbuffer);
                    numero = numero +7;
                }
        }
    

        return(0);
}