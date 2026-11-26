#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int t = 5;
int faza = 1;

void prnt(char *in) {
    write(STDOUT_FILENO, in, strlen(in));
}

void sig_alrm(int signo) {
    
    printf("Alarm v casu t = %d ...\n", t*faza);
    fflush(stdout);

    faza++;
    
    switch(faza) {
        case 2: 
            prnt("Ignoriram signal SIGINT ...\n");
            signal(SIGINT, SIG_IGN);
            alarm(t);
            break;
        case 3:
            prnt("Privzeta akcija za SIGINT ...\n");
            signal(SIGINT, SIG_DFL);
            alarm(t);
            break;
        default:
            exit(0);
    }   
}

void sig_int(int signo) {
    prnt("Prejel sem signal 2.\n");
}


void main () {

    signal(SIGALRM, sig_alrm);
    signal(SIGINT, sig_int);

    prnt("Lovim signal SIGINT ...\n");
    alarm(t);

    for (;;)
        pause();
}