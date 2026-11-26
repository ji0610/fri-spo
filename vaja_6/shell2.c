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

void parse_strtok(char *niz_ukaz, char *args[]) {

    int i = 0;

    args[i] = strtok(niz_ukaz, " \t");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " \t");
    }
}


void main() {
    
    char buf[MAXLINE];

    char *flags[MAXARG+1];
    char *flags2[MAXARG+1];

    pid_t pid, pid2;

    int st_ukaza = 1;
    
    char *pipe_symbol_ptr;


    int fd[2];


    while (fgets(buf, MAXLINE, stdin) != NULL) {
        //printf("%s", buf);

        buf[strlen(buf) - 1] = '\0';
        if (buf[0] == '\0') continue;

        if ((pipe_symbol_ptr = strstr(buf, "=>")) == NULL) {
            if (st_ukaza % 2 == 0) {
                parse(buf, flags);
            } else {
                parse_strtok(buf, flags);
            }
            st_ukaza++;
        } else {
            pipe_symbol_ptr[0] = '\0';
            parse(buf, flags);
            parse(pipe_symbol_ptr + 2, flags2);
        }
        

        if (flags[0] == NULL) continue;


        if (pipe_symbol_ptr == NULL) {

            if ( (pid = fork()) < 0)
                err("fork error");

            if (pid == 0) {
                if (execvp(flags[0], flags) < 0)
                    perror("execvp");
                _exit(1);
            } else {
                wait(NULL);
                continue;
            }
        }

        if (flags2[0] == NULL) {
            fprintf(stderr, "Error: no command after =>.\n");
            continue;
        }

        if (pipe(fd) == -1){
            err("pipe");
        }
        
        // 1st child
        if ( (pid = fork()) < 0) {
            err("fork error");
        }

        if (pid == 0) {

            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            close(fd[0]);

            if (execvp(flags[0], flags) < 0)
                perror("execvp");
            _exit(1);
        
        }

        // 2nd child
        if ( (pid = fork()) < 0) {
            err("fork error");
        }

        if (pid == 0) {

            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);

            if (execvp(flags2[0], flags2) < 0)
                perror("execvp");
            _exit(1);
        
        }

        close(fd[1]);
        close(fd[0]);

        wait(NULL);
        wait(NULL);
            
    }
}

