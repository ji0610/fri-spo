#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uporaba: %s <dovoljenja> <datoteka>\n", argv[0]);
        exit(1);
    }

    /*
    Ob zagonu programa podamo dva argumenta:
    ▪ nova dovoljenja (mode) in
    ▪ ime datoteke, katere dovoljenja spreminjamo.
    */

    struct stat buf;
    char mode[10];
    long int dovoljenja;

    if (lstat(argv[2], &buf) < 0) {
        printf("lstat error.\n");
        exit(1);
    }

    if (isdigit(argv[1][0])) {

        
        dovoljenja = strtol(argv[1], NULL, 8);
        
        if (dovoljenja > 07777) {
            printf("Napačna dovoljenja!\n");
            exit(1);
        }
        chmod(argv[2], dovoljenja);

         

    } else {
        /*
        ▪ u+s (postavi bit "Set-User-ID")
        ▪ u-s (pobriši bit "Set-User-ID")
        ▪ g+s (postavi bit "Set-Group-ID")
        ▪ a+rX (dodaj vsem bit r, če ga kdo že ima – glej man chmod - Pazi: X ni x)
        */
        
        dovoljenja = buf.st_mode & 07777;

        if (strcmp(argv[1], "u+s") == 0) {
            dovoljenja = dovoljenja | S_ISUID;
        } else if (strcmp(argv[1], "u-s") == 0) {
            dovoljenja = dovoljenja & ~S_ISUID;
        } else if (strcmp(argv[1], "g+s") == 0) {
            dovoljenja = dovoljenja | S_ISGID;
        } else if (strcmp(argv[1], "a+rX") == 0) {
            if (dovoljenja & S_IRUSR | dovoljenja & S_IRGRP | dovoljenja & S_IROTH) {
                dovoljenja = dovoljenja | S_IRUSR | S_IRGRP | S_IROTH;
            }
        }

        if (lstat(argv[2], &buf) < 0) {
        printf("lstat error.\n");
        exit(1);
        }
        
    
    }

    if (lstat(argv[2], &buf) < 0) {
        printf("lstat error.\n");
        exit(1);
    }

    char* test = "rwxrwxrwx";

    for(int i = 0; i < 9; i++) {
        if (buf.st_mode & 1 << (8-i)) {
            mode[i] = test[i];
        } else {
            mode[i] = '-';
        }
    }
        
    mode[9] = '\0';

    if (buf.st_mode & S_ISUID) {
        if (mode[2] == 'x') {
            mode[2] = 's';
        } else {
            mode[2] = 'S';
        }
    }
    if (buf.st_mode & S_ISGID) {
        if (mode[5] == 'x') {
            mode[5] = 's';
        } else {
            mode[5] = 'S';
        }
    }
    if (buf.st_mode & S_ISVTX) {
        if (mode[8] == 'x') {
            mode[8] = 't';
        } else {
            mode[8] = 'T';
        }
    }
    printf("Nova dovoljenja: %s\n", mode);
    //printf("%o\n", buf.st_mode & 07777);

}