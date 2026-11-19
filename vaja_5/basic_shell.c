#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 64
#define err(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

void main() {
    
    char buf[MAXLINE];
    pid_t pid;

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        //printf("%s", buf);

        buf[strlen(buf) - 1] = '\0';
        if (buf[0] == '\0') continue;

        if ( (pid = fork()) < 0) {
            err("fork error");
        }

        if (pid == 0) {
            if (execlp(buf, buf, (char *) 0) < 0)
                err("execlp error"); 
        } else {
            wait(NULL);
        }
            
    }
}