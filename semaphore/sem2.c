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
    int shmid,shmid2;
    key_t key,key2;
    char *shm,*shm2;

    key = 5678; key2=8765;

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shmid2 = shmget(key2, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    int pos=2;

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
    

    char *s = "ABCDEFGH";
    int l = strlen(s);
    for (int i = 0; i < l; ++i)
    {
        if (semop(id, &p, 1) < 0)
        {
            perror("semop p");
            exit(13);
        }
        if (*shm=='1'){
            shm2[pos++]=s[i];
            shm2[pos++]=s[i];
            pos+=2;
            *shm='0';
        }
        else i--;
        
        if (semop(id, &v, 1) < 0)
        {
            perror("semop p");
            exit(14);
        }
    }
}