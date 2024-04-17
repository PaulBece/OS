#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

void sigchld_handler(int sig);
void sendMessage(int k, int flg,int num,pid_t childpid);
int signalON;

int main(void)
{
    srand(time(NULL));
    signalON = 0;
    signal(SIGINT, sigchld_handler);
    pid_t   childpid;

    if((childpid = fork()) == -1)
    {
            perror("fork");
            exit(1);
    }

    if(childpid == 0) // HIJO
    {
        printf("Child pid: %d\n", getpid());
        while(1){
            if (signalON){
                for (int i =0;i<1000;i++) {
                    int num = rand() % 99 +1;
                    if (num>10){
                        sendMessage(151515,0606,num,childpid);
                    }
                    else{
                        sendMessage(101010,0660,num,childpid);                        
                    }
                }
                exit(0);
            }
            else{
                pause();
            }
        }
    }
    else  // PADRE (REPETIR LO MISMO QUE EN EL HIJO)
    {
        
        printf("Parent pid: %d\n", getpid());
        while (1){
            if (signalON){
                for(int i =0;i<1000;i++){
                    int num = rand() % 99 +1;
                    if (num>10){
                        sendMessage(151515,0606,num,childpid);
                    }
                    else{
                        sendMessage(101010,0660,num,childpid);    
                    }
                }
                exit(0);
            }
            else{
                pause();
            }
        }
    }


    return(0);
}

void sigchld_handler(int sig){
    signalON = 1;
}

void sendMessage(int k, int flg,int num, pid_t childpid)
{
    int msqid;
    int msgflg = IPC_CREAT | flg;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    key = k;

    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
      die("msgget");

    //Message Type
    sbuf.mtype = num;
    if (childpid)
        snprintf(sbuf.mtext, MAXSIZE, "Child number: %d", num);
    else 
        snprintf(sbuf.mtext, MAXSIZE, "Parent number: %d", num);

    buflen = strlen(sbuf.mtext) + 1 ;

    if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
    {
        printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
        die("msgsnd");
    }
}