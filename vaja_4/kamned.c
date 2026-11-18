#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define err(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

void main(int argc, char *argv[]) {

    if (argc == 3 && strcmp(argv[1], "-a") != 0) {
        printf("Uporaba: ./kamned -a <filename>\n");
        exit(1);
    } else if (argc == 4 && strcmp(argv[1], "-d") != 0 && strcmp(argv[1], "-i") != 0) {
        printf("Uporaba: ./kamned [-d, -i] <filename>\n");
        exit(1);
    } else if (argc != 3 && argc != 4) {
        printf("Uporaba: ./kamned -a <filename> ali ./kamned [-d | -i] <?> <filename>\n");
        exit(1);
    }

    FILE *f, *tmp;
    char *filename;
    char line[512];

    if (argc == 3) {
        filename = argv[2];
    } else {
        filename = argv[3];
    }

    if (argc == 3 && strcmp(argv[1], "-a") == 0) {

        f = fopen(filename, "a");
        if (f == NULL)    
            err("fopen")

        setbuf(f, NULL);

        while(fgets(line, sizeof(line), stdin) != NULL) {
            if (fputs(line, f) == EOF)
                err("fputs")
        }

        if (fclose(f) == EOF)
            err("fclose")

    } else if (argc == 4) {
        
        int vrstica = atoi(argv[2]);
        int trenutna_vrstica = 1;
        char nova_vrstica[512];

        if (vrstica <= 0) 
            err("vrstica mora biti pozitivna")

        f = fopen(filename, "r");
        if (f == NULL)    
            err("fopen")
        
        tmp = fopen("tmp.txt", "w");
        if (tmp == NULL)    
            err("fopen")
        
        if (strcmp(argv[1], "-i") == 0) {
            if (fgets(nova_vrstica, sizeof(nova_vrstica), stdin) == NULL)
                err("fgets")
        }

        while(fgets(line, sizeof(line), f) != NULL) {
            if (strcmp(argv[1], "-d") == 0) {
                if (vrstica != trenutna_vrstica) {
                    if (fputs(line, tmp) == EOF)
                        err("fputs")
                }
            } else if (strcmp(argv[1], "-i") == 0) {
                if (vrstica == trenutna_vrstica) {
                    if (fputs(nova_vrstica, tmp) == EOF)
                        err("fputs")
                    vrstica = 0;
                }
            
                if (fputs(line, tmp) == EOF)
                    err("fputs")
            }
            
            if (strchr(line, '\n') != NULL)
                trenutna_vrstica++; 
        }

        if (fclose(f) == EOF)
            err("fclose")
        if (fclose(tmp) == EOF)
            err("fclose")

        if (rename("tmp.txt", filename) < 0)
            err("rename")

    }
    
}