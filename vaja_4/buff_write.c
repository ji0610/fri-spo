#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define err(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

void main(int argc, char *argv[]) {
    
    if (argc != 3) {
        err("Uporaba ./buff_write < B > < N >")
    }

    long B = atoi(argv[1]);
    long N = atoi(argv[2]);

    if (B <= 0 || N <= 0) {
        err("B in N morata biti pozitivna.")
    }


    char buf[B];

    FILE *f;

    f = fopen("izhod.txt", "w");
    if (f == NULL) err("fopen")

    if (setvbuf(f, buf, _IOFBF, B) != 0)
        err("setvbuf")

    char *c = "to je prava stvar.";

    struct timespec start, stop;
    
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < N; i++) {
        if (fputs(c, f) == EOF)
            err("fputs")
    }
    if (fclose(f) == EOF)
        err("fclose")

    clock_gettime(CLOCK_MONOTONIC, &stop);

    

    double cas = stop.tv_sec - start.tv_sec + (stop.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("B: %ld, N: %ld\n", B, N);
    printf("Koncni cas: %lf\n", cas);
    
    
}