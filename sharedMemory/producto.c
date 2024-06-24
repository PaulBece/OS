/*
 * shm-client - client program to demonstrate shared memory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/shm.h>

#define KEY 0x1111
#define SHMSZ     27

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};



struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

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

    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }
    union semun u;
    u.val = 1;
    if (semctl(id, 0, SETVAL, u) < 0)
    {
        perror("semctl");
        exit(12);
    }

    while (c<100){
        if (strcmp(last,shm)!=0){

            if (semop(id, &p, 1) < 0) {
            perror("semop p");
            exit(13);
            }
            num=atoi(shm);
            sprintf(shm,"%d,%d\0",num,num*2);
            strncpy(last,shm,20);
            last[20]='\0';
            printf("%s\n",shm);
            c++;        
            if (semop(id, &v, 1) < 0)
            {
                perror("semop p");
                exit(14);
            }
        }
    }
    exit(0);
}