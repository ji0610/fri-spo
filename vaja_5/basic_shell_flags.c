#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 64
#define MAXARG  32
#define err(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }


void parse(char *niz_ukaz, char *args[]) {
    
    int i = 0;

    while (*niz_ukaz != '\0') {
        while (*niz_ukaz == ' ' || *niz_ukaz == '\t') {
            niz_ukaz++;
        }

        if (*niz_ukaz == '\0') break;
        
        // flag start
        args[i] = niz_ukaz;
        i++;

        while (!(*niz_ukaz == ' ' || *niz_ukaz == '\t' || *niz_ukaz == '\0')) {
            niz_ukaz++;
        }

        // flag end
        if (*niz_ukaz == ' ' || *niz_ukaz == '\t') {
            *niz_ukaz = '\0';
            niz_ukaz++;
        }
    }

    args[i] = NULL;
}


void main() {
    
    char buf[MAXLINE];
    char *flags[MAXARG+1];
    pid_t pid;

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        //printf("%s", buf);

        buf[strlen(buf) - 1] = '\0';
        if (buf[0] == '\0') continue;

        parse(buf, flags);
        if (flags[0] == NULL) continue;

        if ( (pid = fork()) < 0) {
            err("fork error");
        }

        if (pid == 0) {
            if (execvp(flags[0], flags) < 0)
                err("execvp error"); 
        } else {
            wait(NULL);
        }
            
    }
}