#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <string.h>

#define CHILD_COUNT 2

int childInd = 1;
int msgNumber;


// Getting current time
void getCurrTime() {
    struct timeval te;
    errno = 0;
    gettimeofday(&te, NULL);
    if (errno != 0) {
        perror("gettimeofday");
    }

    long msec = te.tv_usec / 1000;
    long sec = te.tv_sec % 60;
    long min = (te.tv_sec / 60) % 60;
    long hours = (te.tv_sec / 3600 + 3) % 24;

    printf("Time is: %02ld:%02ld:%02ld:%03ld ", hours, min, sec, msec);
}

void getInfo(int ind, char *person, char* action, int sigInd, int childPid) {
    printf("%3d PID:%5d PPID:%5d ", msgNumber, getpid(), getppid());
    msgNumber++;
    getCurrTime();
    if (strcmp(person, "child") == 0) {
        printf(" CHILD%d %s SIGUSR%d\n", ind, action, sigInd);
    }
    else if (strcmp(person, "parent") == 0 && strcmp(action, "get") == 0) {
        printf(" PARENT %s SIGUSR%d from %d\n", action, sigInd, childPid);
    }
    else if (strcmp(person, "parent") == 0 && strcmp(action, "put") == 0) {
        printf(" PARENT %s SIGUSR%d\n", action, sigInd);
    }
}

void parentSigAct(int sig, siginfo_t *sginf, void *_) {
    getInfo(0, "parent", "get", 2, sginf->si_pid);

    if (usleep(1000*100) == -1) 
        perror("esleep");

    kill(0, SIGUSR1);

    getInfo(0, "parent", "put", 1, sginf->si_pid);

}

void childSigAct(int sig) {
    getInfo(childInd, "child","get" ,1, 0);
    if (kill(getppid(), SIGUSR2) == -1) 
        perror("kill");
    else
        getInfo(childInd, "child","put" ,2, 0);
}

int main() {

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = parentSigAct;
    int err_code = sigaction(SIGUSR2, &sa, NULL);

    if (err_code == -1) {
        perror("sigaction");
    }

    signal(SIGUSR1, SIG_IGN);

    pid_t pid = 1;
    for (int i = 0; i < CHILD_COUNT && pid > 0; i++) {
        switch (pid = fork()) {
            case 0:
                printf("Child %d, my pid is %d, my ppid is %d\n", childInd, getpid(), getppid());

                signal(SIGUSR1, childSigAct);
                break;
            case -1:
                perror("fork");
                exit(-1);
            default:
                childInd++;
                break;
        }
    }

    if (pid > 0) {
        sleep(1);
        kill(0, SIGUSR1);
    }

    while (1);
}