#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAXSIZE     128

void die(char *s)
{
  perror(s);
  exit(1);
}

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};


void main()
{
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 151515;

    if ((msqid = msgget(key, 0606)) < 0)
      die("msgget()");

    while (1) {
        if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 0, IPC_NOWAIT) < 0) {
            if (errno == ENOMSG) {
                break;
            } else {
                die("msgrcv");
            }
        }
        printf("%s\n", rcvbuffer.mtext);
    }
    exit(0);
}