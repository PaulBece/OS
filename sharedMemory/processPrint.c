#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHMSZ 27

int main() {
    int shmid, shmid2;
    key_t key = 5678, key2 = 8765;
    char *shm, *shm2;
    int c = 0;

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

    *shm2 = '1';

    while (c < 200) {
        if (!(*shm2)) {
            printf("%s\n", shm);
            c++;
            *shm2 = '1';
        }
    }

    exit(0);
}
