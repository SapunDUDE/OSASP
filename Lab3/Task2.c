#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>


void GetCurrTime() {
   struct timeval te;
   gettimeofday(&te, NULL);

   long msec = te.tv_usec / 1000;
   long sec = te.tv_sec % 60;
   long min = (te.tv_sec / 60) % 60;
   long hours = (te.tv_sec / 3600 + 3) % 24;


    printf("Time is: %02ld:%02ld:%02ld:%03ld\n\n", hours, min, sec, msec);
}


int main() {
    pid_t child;

    printf("Parent fork:\n");
    GetCurrTime();

    printf("Child fork:\n");
    for (int i = 0; i < 2; i++) {
        child = fork();
        // Choosing the child and parent fork
        switch (child) {
            // Output time with child fork
            case 0:
                printf("Child %d fork %d Parent fork %d\n", i, getpid(), getppid());
                GetCurrTime();
                return 0;
            case -1:
                perror("Fork error");
                exit(-1);
            default:
                break;
        }
    }
    system("ps -x");

    while (1) {
        if (wait(NULL) == -1) {
            if (errno == ECHILD)
                break;
        }
    }

    return 0;
}