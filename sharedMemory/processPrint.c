#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27

main()
{
    char c;
    int shmid;
    key_t key;
    char *shm;
    char last[100];

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5678;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
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

    

    /*
     * Now put some things into the memory for the
     * other process to read.
     */

    
    *last = NULL;

    while (1){
        if (strcmp(shm,last)!=0){
            strncpy(last,shm,20);
            last[20]='\0';
            printf("%s\n",shm);
        }
    }
    exit(0);
}