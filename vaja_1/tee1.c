#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 8192

void main(int argc, char *argv[]) {

    int fd, n;
    char buf[BUFSIZE];
    
    // append
    if (argc == 3 && strcmp(argv[1], "-a") == 0) {
        
        fd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0600);
        if (fd < 0) {
            fprintf( stderr, "Napaka pri odpiranju.\n");
            exit(1);
        }

        while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0) {
            write(fd, buf, n);
            write(STDOUT_FILENO, buf, n);
        } 


    // new file
    } else if (argc == 2) {

        fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);

        while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0) {
            write(fd, buf, n);
            write(STDOUT_FILENO, buf, n);
        }


    } else {
        printf("Napaka.\n");
        exit(1);
    }
}