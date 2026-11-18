#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>

void main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Uporaba: %s <datoteka>\n", argv[0]);
        exit(1);
    }

    struct stat buf;
    char tip;
    char mode[11];

    if (lstat(argv[1], &buf) < 0) {
        printf("lstat error.\n");
        exit(1);
    }

    /*
    • S_ISREG
    • S_ISDIR
    • S_ISCHR
    • S_ISBLK
    • S_ISFIFO
    • S_ISLNK
    • S_ISSOCK
    */

    if (S_ISREG(buf.st_mode)) {
        tip = '-';
    } else if (S_ISDIR(buf.st_mode)) {
        tip = 'd';
    } else if (S_ISCHR(buf.st_mode)) {
        tip = 'c';
    } else if (S_ISBLK(buf.st_mode)) {
        tip = 'b';
    } else if (S_ISFIFO(buf.st_mode)) {
        tip = 'p';
    } else if (S_ISLNK(buf.st_mode)) {
        tip = 'l';
    } else if (S_ISSOCK(buf.st_mode)) {
        tip = 's';
    }

    mode[0] = tip;

    char* test = "rwxrwxrwx";

    for(int i = 1; i < 10; i++) {
        if (buf.st_mode & 1 << (9-i)) {
            mode[i] = test[i-1];
        } else {
            mode[i] = '-';
        }
    }
    mode[10] = '\0';

    if (buf.st_mode & S_ISUID) {
        if (mode[3] == 'x') {
            mode[3] = 's';
        } else {
            mode[3] = 'S';
        }
    }
    if (buf.st_mode & S_ISGID) {
        if (mode[6] == 'x') {
            mode[6] = 's';
        } else {
            mode[6] = 'S';
        }
    }
    if (buf.st_mode & S_ISVTX) {
        if (mode[9] == 'x') {
            mode[9] = 't';
        } else {
            mode[9] = 'T';
        }
    }

    int povezave = buf.st_nlink;
    int velikost = buf.st_size;


    printf("%s %d %s %d %s\n", mode, povezave, getpwuid(buf.st_uid)->pw_name, velikost, argv[1]);
    //printf("%d\n", povezave);


    //printf("%o\n", buf.st_mode & 0777);
}