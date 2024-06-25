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

int main()
{
    int shmid;
    key_t key;
    char *shm;

    int shmid2;
    key_t key2;
    char *shm2;

    char last[100];
    int num = 100;
    int c=0;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;
    key2= 8765;
    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shmid2 = shmget(key2, SHMSZ, 0666)) < 0) {
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
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    *last='\0';
    //sprintf(shm,"%d\0",num);

    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }

    while (c<100){
        if (semop(id, &p, 1) < 0) {
            perror("semop p");
            exit(13);
        }
        if (*shm2 && strcmp(last,shm)!=0){
            sprintf(shm,"%d\0",num++);
            strncpy(last,shm,20);
            last[20]='\0';
            printf("%s\n",shm);
            c++;
            *shm2='\0';       
        }
        if (semop(id, &v, 1) < 0)
            {
            perror("semop p");
            exit(14);
        }
    }

    

    exit(0);
}