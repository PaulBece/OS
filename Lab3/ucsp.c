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

    key = 101010;

    if ((msqid = msgget(key, 0660)) < 0)
      die("msgget()");

    for (int i = 1; i <= 10; i += 2) {
        while (1) {
            if (msgrcv(msqid, &rcvbuffer, MAXSIZE, i, IPC_NOWAIT) < 0) {
                if (errno == ENOMSG) {
                    break;
                } else {
                    die("msgrcv");
                }
            }
            printf("%s\n", rcvbuffer.mtext);
        }
    }
    exit(0);
}