#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define KEY 0x1111
#define SHMSZ 27

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main() {
    int shmid, shmid2, semid;
    key_t key = 5678, key2 = 8765;
    char *shm, *shm2;
    char last[100] = {0};
    int num = 100, c = 0;

    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shmid2 = shmget(key2, SHMSZ, 0666)) < 0) {
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

    semid = semget(KEY, 1, 0666 | IPC_CREAT);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    sprintf(shm, "%d\0", num);

    while (c < 100) {
        if (semop(semid, &p, 1) < 0) {
            perror("semop p");
            exit(1);
        }

        if (*shm2 && strcmp(last, shm) != 0) {
            sprintf(shm, "%d\0", num++);
            strncpy(last, shm, 20);
            last[20] = '\0';
            printf("%s\n", shm);
            c++;
            *shm2 = '\0';
        }

        if (semop(semid, &v, 1) < 0) {
            perror("semop v");
            exit(1);
        }
    }

    exit(0);
}
