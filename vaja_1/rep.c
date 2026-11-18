#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 8192

void main(int argc, char *argv[]) {
    
    int n_of_lines = 5;
    int n = 0;
    int fd, len;

    char buf[BUFSIZE];

    if (argc == 4 && strcmp(argv[1], "-n") == 0) {

        n_of_lines = atoi(argv[2]);
        if ((fd = open(argv[3], O_RDONLY)) < 0) {
            fprintf( stderr, "Napaka pri odpiranju.\n");
            exit(1);
        }

    } 
    
    if (argc != 2 && argc != 4) {
        fprintf( stderr, "Napacno stevilo argumentov.\n");
        exit(1);
    }
    
    if (argc == 2) {
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            fprintf( stderr, "Napaka pri odpiranju.\n");
            exit(1);
        }
    }

    off_t curr_pos = lseek(fd, 0, SEEK_END);

    //printf("%ld --", curr_pos);

    char c;

    for (int i = 1; i < curr_pos; i++) {

        lseek(fd, -1-i, SEEK_END);
        read(fd, &c, 1);
        //write(STDOUT_FILENO, &c, 1);

        

        if (c == '\n') {
            n++;
            if (n == n_of_lines) {
                read(fd, buf, i);
                write(STDOUT_FILENO, buf, i);

                break;  
            }
        }

        if (i == curr_pos - 1) {
            lseek(fd, -1, SEEK_CUR);
            read(fd, buf, i);
            write(STDOUT_FILENO, buf, i);

            break; 
        }
    }

    
}