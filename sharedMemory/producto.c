/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27

main()
{
    int shmid;
    key_t key;
    char *shm;

    char last[100];
    int num;
    int c=0;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    *last=NULL;
    while (c<100){
        if (strcmp(last,shm)!=0){
            num=atoi(shm);
            sprintf(shm,"%d,%d\0",num,num*2);
            strncpy(last,shm,20);
            last[20]='\0';
            printf("%s\n",shm);
            c++;
        }
    }
    exit(0);
}